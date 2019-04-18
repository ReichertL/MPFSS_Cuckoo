#pragma once

typedef struct {
    int t; // number of points in mpfss
    size_t size; //size of input field
    int max_loop;
    int w; //number of hashfunctions
    int b; //number of buckets
    int cp; //ocCurrentParty
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

mpfss_cuckoo * new_mpfss_cuckoo( int t, size_t size, int w, int b, int max_loop,int cp );
