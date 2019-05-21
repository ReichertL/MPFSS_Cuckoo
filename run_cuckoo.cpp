#include <string>
#include <iostream>
#include <thread>
#include <vector> 
#include <math.h>       /* pow */

#include "code_master/mpfss_cuckoo/mpfss_cuckoo.h"

using namespace std;

int main(int argc, char *argv[]) {

	const char* s_host = "localhost";
    char* remote_host = new char[15];
    strncpy(remote_host, s_host, 15);

    const char* s_port = "5555";
    char* port = new char[15];
    strncpy(port, s_port, 15);


    bool do_benchmark=true;
    int set_beta=0;
    bool exists=false;
    bool buckets=false;
    int indices_set=-1;
	//vector<int> indices{ 3,5}; 

    #ifdef TESTING
		vector<int> t_vals{ 2 }; 
		vector<int> n_vals_pow{4}; 
		int no_runs=2;
    #else
	   // vector<int> t_vals{ 74,192,382,741,1422,5205 }; 
	   // vector<int> n_vals_pow{ 11,14,16,18,20,24}; 
	  	vector<int> t_vals{ 5205 }; 
	    vector<int> n_vals_pow{24}; 
		int no_runs=30;
  	#endif


	


    for (int i = 0; i < (int) t_vals.size(); ++i){
    	int t= t_vals.at(i);
    	int size=pow(2,n_vals_pow.at(i));

	    mpfss_cuckoo_args<int> *mc_args1=(mpfss_cuckoo_args<int> *)calloc(1, sizeof(mpfss_cuckoo_args<int>));
	    mc_args1->cp=1;
	    mc_args1->do_benchmark=do_benchmark;    

	    mc_args1->port=port;
	    mc_args1->host=remote_host;
		mc_args1->connection_already_exists=exists;

		mc_args1->buckets_set=buckets;
	    mc_args1->set_beta=set_beta;
	    mc_args1->indices_set=indices_set;
	    //mc_args1->indices=indices;
	    mc_args1->print_stdout=true;

	    mpfss_cuckoo_args<int> *mc_args2=(mpfss_cuckoo_args<int> *)calloc(1, sizeof(mpfss_cuckoo_args<int>));
	    mc_args2->cp=2;
	    mc_args2->do_benchmark=do_benchmark;

	    mc_args2->port=port;
	    mc_args2->host=remote_host;
	    mc_args2->connection_already_exists=exists;
		mc_args2->buckets_set=buckets;
	    mc_args2->set_beta=set_beta;
	   	mc_args2->indices_set=indices_set;
	    mc_args2->print_stdout=false;

    	for (int j = 0; j < no_runs; ++j){
    		printf("--------------------------------t %d, size %d, run %d----------------------------------------------------------------------\n", t, size, j);
			if(j>0){
				mc_args1->buckets_set=true;
				mc_args2->buckets_set=true;
				mc_args1->indices_set=true;
				mc_args2->indices_set=true;
			}    		

			thread t1(run_mpfss_cuckoo, t,size,mc_args1);
			//t1.detach();
			thread t2(run_mpfss_cuckoo, t,size,mc_args2 );
			t1.join();
			t2.join();
			this_thread::sleep_for(chrono::seconds(2));
		}

		free_mc_args(mc_args1);
    	free_mc_args(mc_args2);

	}
}

