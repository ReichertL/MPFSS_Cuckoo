#pragma once


typedef struct  {

	mpfss_cuckoo *m;
	int *bucket_lenghts;
	match **matches;
	int ** all_buckets_array;
	uint8_t *dpf_output;
	bool * dpf_bit_output;
	uint8_t **beta_value_vector;
	bool cprg;
	int bucket_no;
} yao_arguments_new;

mpfss_cuckoo * new_mpfss_cuckoo( int t, size_t size, int w, int b, int max_loop,int cp, int do_benchmark );
