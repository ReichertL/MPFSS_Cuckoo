#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>

#include "includes/dbg.h"
#include "parallel.h"

void create_mpfss_vector_cuckoo(    bool *mpfss_bit_vector,uint8_t **mpfss_value_vector,
                                    bool **dpf_vectors, uint8_t ***dpf_value_vectors, int ** all_buckets_array,
                                    int *bucket_lenghts,  int size, int b, int memblocksize){
    /*breaks code*/
    //#pragma omp parallel for
        for(int i=0;i<b; ++i){
        	int *bucket=all_buckets_array[i];
            int len=bucket_lenghts[i];

            for (int j=0; j<len;j++){

                bool is_set=dpf_vectors[i][j];
                int position=bucket[j];
                mpfss_bit_vector[position]=mpfss_bit_vector[position]^is_set;

                for (int k = 0; k < memblocksize; ++k){
                	uint8_t *vec=dpf_value_vectors[i];
                	uint8_t value=vec[j*memblocksize +k ];
            		mpfss_value_vector[position][k]^=value;
            	}
            }
        }
}


void parallize(split_fn fn, void ** list_pd_split, int threads, int b, void * params){

    int * min_arr=calloc(b, sizeof(int));
    int * max_arr=calloc(b, sizeof(int));

    int last_max=0;
    int min=0;
    int base=b/threads;
    int rest=b-base*threads;

    for (int i = 0; i <  b; ++i){
        int val=base;
        if(rest>0){
            val=val+1;
            rest=rest-1;
        }
        int max=last_max+val;
        min_arr[i]=min;
        max_arr[i]=max;
        min=max;
        last_max=max;

    }

    int used_threads=threads;
    if(b<threads){
        used_threads=b;
    }

    #pragma omp parallel for
    for (int i = 0; i < used_threads; ++i){
        if(min_arr[i]<max_arr[i]){
            debug("share %d, %d \n",min_arr[i],max_arr[i] );
            fn(params,min_arr[i],max_arr[i],list_pd_split[i]);
        }
    }
    free(min_arr);
    free(max_arr);
    
}



