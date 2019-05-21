#include <string>
#include <iostream>
#include <thread>
#include <vector> 
#include <math.h>       /* pow */
#include <omp.h>

#include "code_master/mpfss_cuckoo/mpfss_cuckoo.h"

using namespace std;

int main(int argc, char *argv[]) {

	omp_set_num_threads(1);


    const char *remote_host = strtok(argv[1], ":");
    const char *port = strtok(NULL, ":");
	int cp = (argv[2][0]=='1'? 1 : 2);


    bool do_benchmark=true;
    int set_beta=0;
    bool exists=false;
    bool buckets=false;
    int indices_set=-1;

    #ifdef TESTING
		vector<int> t_vals{ 2 }; 
		vector<int> n_vals_pow{4}; 
		int no_runs=2;
    #else
	   // vector<int> t_vals{ 74,192,382,741,1422,5205 }; 
	   // vector<int> n_vals_pow{ 11,14,16,18,20,24}; 
	  	vector<int> t_vals{ 5205 }; 
	    vector<int> n_vals_pow{ 24};
	//	int no_runs=10;
  		int no_runs=3;
  	#endif

    for (int i = 0; i < (int) t_vals.size(); ++i){
    	int t= t_vals.at(i);
    	int size=pow(2,n_vals_pow.at(i));

	    mpfss_cuckoo_args<int> *mc_args=(mpfss_cuckoo_args<int> *)calloc(1, sizeof(mpfss_cuckoo_args<int>));
	    mc_args->cp=cp;
	    mc_args->do_benchmark=do_benchmark;    

	    mc_args->port=port;
	    mc_args->host=remote_host;
		mc_args->connection_already_exists=exists;

		mc_args->buckets_set=buckets;
	    mc_args->set_beta=set_beta;
	    mc_args->indices_set=indices_set;
	    mc_args->print_stdout=true;
	    mc_args->cprg=true;

    	for (int j = 0; j < no_runs; ++j){
    		printf("--------------------------------t %d, size %d, run %d----------------------------------------------------------------------\n", t, size, j);
			if(j>0){
				mc_args->buckets_set=true;
				mc_args->indices_set=true;
			}    		

			thread t1(run_mpfss_cuckoo, t,size,mc_args);
			//t1.detach();
			t1.join();
			this_thread::sleep_for(chrono::seconds(cp*4));
		}
	}
}

