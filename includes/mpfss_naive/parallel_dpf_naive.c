#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>

#include "includes/dbg.h"
#include "parallel_dpf_naive.h"
//#include "mpfss_naive.h"


void parallize_naive(split_fn fn, void ** list_pd_split, int threads, int t, void * params){
        
    int * min_arr=calloc(t, sizeof(int));
    int * max_arr=calloc(t, sizeof(int));

    int last_max=0;
    int min=0;
    int base=t/threads;
    int rest=t-base*threads;

    for (int i = 0; i <  t; ++i){
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
    if(t<threads){
        used_threads=t;
    }


    #pragma omp parallel for
    for (int i = 0; i < used_threads; ++i){
    	if(min_arr[i]<max_arr[i]){
            log_info("min %d max %d\n", min_arr[i], max_arr[i]);
        	fn(params,min_arr[i],max_arr[i],list_pd_split[i]);
    	}
    }
    
}