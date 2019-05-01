#pragma once


extern "C" {
    #include "obliv.h"
}

template <typename T>
struct  mpfss_cuckoo_args{
	int cp; 			//needs to be set
	bool do_benchmark; //needs to be set

	ProtocolDesc pd; //set either pd or port and host
	const char *host;
    const char *port;
    bool connection_already_exists; //need to be set

	int *bucket_lenghts;
	bool buckets_set;
	int ** all_buckets_array;
	std::vector<std::vector<int>> all_buckets;

	std::vector<T> mpfss_output;
	std::vector<bool> mpfss_bit_output;
	uint8_t **mpfss_output_raw;
	bool * mpfss_bit_output_raw;
	
	std::vector<T> beta_vector; //has to have length m 
	int set_beta;	//need to be set, 0: no beta, 1: take beta from vector, -1: beta will be 50 for each t

};

ProtocolDesc prepare_connection(int cp,const char *remote_host, const char *port );

void run_mpfss_cuckoo(int t, int size, mpfss_cuckoo_args<int> *mc_args);
void free_mc_args( mpfss_cuckoo_args<int> *mc_args);