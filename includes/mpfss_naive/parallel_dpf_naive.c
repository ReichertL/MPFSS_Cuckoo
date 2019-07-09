#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>

#include "includes/dbg.h"
#include "parallel_dpf_naive.h"
//#include "mpfss_naive.h"


void parallize_naive(split_fn fn, void ** list_pd_split, int t, void * params){
        

   	#pragma omp parallel for
    for (int i = 0; i < t; ++i){
        fn(params,i,list_pd_split[i]);
    }

    //printf("finisehd parallel  %d \n", omp_get_num_threads());
    
}