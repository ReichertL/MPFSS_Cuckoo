
#include <string>
#include <string.h>
#include <iostream>
#include <thread>
#include <vector> 
#include <math.h>       /* pow */
#include <omp.h>

#include "includes/mpfss_naive/mpfss_naive.h"

using namespace std;

int main(int argc, char *argv[]) {


	if (argc == 3) {

	    const char *remote_host = strtok(argv[1], ":");
	    const char *port = strtok(NULL, ":");
		int cp = (argv[2][0]=='1'? 1 : 2);
		bool cprg=true;

	    #ifdef TESTING
			vector<int> t_vals{ 2 }; 
			vector<int> n_vals_pow{4}; 
			int no_runs=2;
			std::vector<int> v_threads{2};
	    #else
			//vector<int> t_vals{ 74,192,382,741,1422,5205 }; 
			//vector<int> n_vals_pow{ 11,14,16,18,20,24}; 
		  	vector<int> t_vals{74}; 
		    vector<int> n_vals_pow{11};
			int no_runs=10;
	  		//int no_runs=3;
	  		//std::vector<int> v_threads{1,2,4,8};
	  		std::vector<int> v_threads{8};
	  	#endif


		for (int k = 0; k < (int) v_threads.size(); ++k){
		
			omp_set_num_threads(v_threads.at(k));

		    for (int i = 0; i < (int) t_vals.size(); ++i){
		    	int t= t_vals.at(i);
		    	int size=pow(2,n_vals_pow.at(i));


		    	for (int j = 0; j < no_runs; ++j){
		    		printf("--------------------------------t %d, size %d, run %d----------------------------------------------------------------------\n", t, size, j);
					printf("threads:%d\n", v_threads.at(k));
					printf("cprg:%d\n",cprg );
				
					run_mpfss_naive(remote_host, port, cp, t, size,cprg);

				}
			}
		}
	} else {
        printf("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
                 "\tHostname usage:\n" 
                 "\tlocal -> 'localhost' remote -> IP address or DNS name\n", argv[0]);
    }
}

