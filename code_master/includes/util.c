#include<obliv.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"util.h"

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
        sprintf(filename, "../benchmark/%s/resutls_t:%d_size:%d", type, t, (int) size);


        FILE *fptr;
        fptr = fopen(filename,"a+");

        time_t rawtime;
        struct tm * timeinfo;
        char current_time[80];
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        strftime (current_time,80,"%d.%m.%Y-%H:%M",timeinfo);

        if(fptr == NULL){
              printf("Error!");   
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
       // fprintf(fptr,"%u/n", yaoGateCount());
        fclose(fptr);

}