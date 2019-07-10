#pragma once

typedef struct {
    int t; // number of points in mpfss
    size_t size; //size of input field
    int max_loop;
    int w; //number of hashfunctions
    int b; //number of buckets
    int cp; //ocCurrentParty
    int do_benchmark;
    int threads;
} mpfss_cuckoo;

typedef struct  {

	int val;
	int bucket;
	int index_in_bucket;
} match;


typedef struct  {

	mpfss_cuckoo *m;
	int *bucket_lenghts;
	match **matches;
	int ** all_buckets_array;
	uint8_t **mpfss_output;
	bool * mpfss_bit_output;
	uint8_t **beta_value_vector;
	bool set_beta;
	uint8_t *skalar;
	bool cprg;
	
} yao_arguments;

mpfss_cuckoo * new_mpfss_cuckoo( int t, size_t size, int w, int b, int max_loop,int cp, int do_benchmark, int threads);
