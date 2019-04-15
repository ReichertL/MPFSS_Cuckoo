#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include<obliv.h>
#include "bcrandom.h"

#include"dbg.h"

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
int create_indices( int *indices_notobliv, int t , int size){
  
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

   releaseBCipherRandomGen(random_gen);

  return 1;

}


