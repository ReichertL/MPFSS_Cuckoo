#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <obliv.h>
#include <bcrandom.h>
#include"dbg.h"


double wallClock()
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME,&t);
  return t.tv_sec+1e-9*t.tv_nsec;
}

void ocTestUtilTcpOrDie(ProtocolDesc* pd,const char* remote_host,
                        const char* port)
{
  if(!remote_host)
  { if(protocolAcceptTcp2P(pd,port)!=0)
    { fprintf(stderr,"TCP accept failed\n");
      exit(1);
    }
  }
  else 
    if(protocolConnectTcp2P(pd,remote_host,port)!=0) 
    { fprintf(stderr,"TCP connect failed\n");
      exit(1);
    }
}

/*  For benchmarking purposes. 
    Creates/ appends to a file the run time of the execution.
    Files are stored in the subfolder "benchmark".
*/
void benchmark(double runtime, size_t size, int t, int cp, char type[]){

        char filename[80];
        sprintf(filename, "/home/turing/TI/benchmark/%s/resutls_t:%d_size:%d", type, t, (int) size);

        char path[80];
        sprintf(path, "/home/turing/TI/benchmark/%s", type);

        time_t rawtime;
        struct tm * timeinfo;
        char current_time[80];
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        strftime (current_time,80,"%d.%m.%Y-%H:%M",timeinfo);
        
        //create dir if not exist
        struct stat st = {0};

        if (stat(path, &st) == -1) {
            mkdir(path, 0700);
        }
    
        FILE *fptr;
        fptr = fopen(filename,"a+");


        if(fptr == NULL){
              printf("Error while logging runtime result!");   
              exit(1);             
        }

        fseek (fptr, 0, SEEK_END);
        int len = ftell(fptr);
        if (0 == len) {
            fprintf(fptr,"Time, Party number, Runtime in Seconds\n" );
        }
        
        fprintf(fptr,"%s,",current_time);
        fprintf(fptr,"%d,",cp);
        fprintf(fptr,"%lf\n",runtime);
        fclose(fptr);

}


/*  Uses the obliv-c random number generator to fill the array indices_notobliv
  with integer values. No value will be larger than "size".
  No duplicate values!

  Input 
    random_gen:     Random number generator.
    indices_notobliv: Array of length t, empty at beginning. 
              After termination of this function, it will contain the indices selected by Party 1.
              Each index represents a position x where y =/= 0.
    t:          Number of selected indices.
    size:       Length of the input interval. Interval defined as [0,size)

*/
int create_indices(int *indices_notobliv, int t , int size){
  
  BCipherRandomGen *random_gen= newBCipherRandomGen();    
  debug("Creating %d distinct indices from [0,%d)\n",t,size);
  unsigned long long rand; 
  int i=0;
  int sanity=0;
  int limit=t*100000;
  bool already_exists=false;
  while(i<t && sanity<limit){
    rand= bcRandomInt(random_gen, (unsigned long long) size);
    sanity++;
    for (int j = 0; j < i; ++j)
    {
      if(indices_notobliv[j]==(int)rand){
        already_exists=true;
      }
    }
    if(!already_exists){
      indices_notobliv[i]=(int) rand;
      i++;
      already_exists=false;
    }
  }
  if(sanity>=limit){
    debug("TEST_create_indices: Creation of distinct indices was not possible with t= %d and size =%d.", t, size);
    return 0;
  }

  #ifdef DEBUG
    char *buff =calloc(t + 100, sizeof(int));
    sprintf(buff, "Indices: ");  
    for(int i = 0; i <t ; i++) {
      char *r = calloc(2, sizeof(int));
      sprintf(r, " %d", indices_notobliv[i]);   
      strcat(buff, r );  
      free(r);
    }
    debug("%s\n",buff);
    free(buff);
  #endif
  return 1;

}