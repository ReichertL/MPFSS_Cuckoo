//ensures header file is only included once
#pragma once



typedef struct {
    int t; // number of points in mpfss
    size_t size; //size of input field
    int max_loop;
    int w; //number of hashfunctions
    int b; //number of buckets

} mpfss_cuckoo;

typedef struct  {

	int val;
	int batch;
	int index_in_batch;
} match;


typedef struct  {

	mpfss_cuckoo *m;
	int *bucket_lenghts;
	match **matches;
	int ** all_buckets_array;
} yao_arguments;

void mpfss_batch_cuckoo(void* args);