#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>

#include "includes/dbg.h"
#include "parallel_dpf_naive.h"
//#include "mpfss_naive.h"


void parallize_naive(split_fn fn, void ** list_pd_split, int threads, int t, void * params){
        
    int base= t/threads;
    int rest= t-base*threads; 
    int *min_arr=calloc(threads,sizeof(int));
    int *max_arr=calloc(threads,sizeof(int));

    int min=0;
    int max=0;
    for (int i = 0; i < threads; ++i){
        int max=max+base;
        if(rest!=0){
            max++;
            rest--;
        }
        min_arr[i]=min;
        max_arr[i]=max;
        min=max;

    }

   	#pragma omp parallel for
    for (int i = 0; i < threads; ++i){
        fn(params,min_arr[i],max_arr[i],list_pd_split[i]);
    }

    //printf("finisehd parallel  %d \n", omp_get_num_threads());
    
}