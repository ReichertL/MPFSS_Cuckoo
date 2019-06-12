#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>

#include "includes/dbg.h"


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


