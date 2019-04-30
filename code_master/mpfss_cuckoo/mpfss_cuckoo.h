#pragma once


template <typename T>
struct  mpfss_cuckoo_args{
	int cp;
	bool do_benchmark;
	mpfss_cuckoo *m;
	int *bucket_lenghts;
	match **matches;
	int ** all_buckets_array;
	std::vector<T> mpfss_output;
	std::vector<bool> mpfss_bit_output;
	uint8_t **mpfss_output_raw;
	bool * mpfss_bit_output_raw;
	ProtocolDesc pd;
	std::vector<T> beta_vector; //has to have length m 
	bool set_beta;
};

ProtocolDesc prepare_connection(int cp,const char *remote_host, const char *port );
template <typename T>
void run_mpfss_cuckoo(int t, int size,mpfss_cuckoo_args<T> *mc_args);