
#include <string>
#include <string.h>
#include <iostream>
#include <thread>
#include <vector> 
#include <math.h>       /* pow */
#include <omp.h>

extern "C" {
	#include "includes/mpfss_naive/mpfss_naive.h"
}
using namespace std;

int main(int argc, char *argv[]) {


	if (argc == 3) {

	    const char *remote_host = strtok(argv[1], ":");
	    const char *port = strtok(NULL, ":");
		int cp = (argv[2][0]=='1'? 1 : 2);
		bool cprg=true;

	    #ifdef TESTING
			vector<int> t_vals{ 4 }; 
			vector<int> n_vals_pow{4}; 
			int no_runs=2;
			std::vector<int> v_threads{2,4};
	    #else
			//vector<int> t_vals{ 74,192,382,741,1422,5205 }; 
			//vector<int> t_vals{3,9,15,21,30,300}; 
			//vector<int> t_vals{3,4,5,6,7,8,9,10,15,21,30,300}; 
			//double scale= 0.0003;
	   	vector<int> t_vals{10,15}; 

		   	//vector<int> n_vals_pow{ 24}; 
			//vector<int> t_vals{3,30,300,3000}; 
			//int no_runs=10;
	  		int no_runs=10;
	  		//std::vector<int> v_threads{8};
	  		std::vector<int> v_threads{8};
	  	#endif


		for (int k = 0; k < (int) v_threads.size(); ++k){
		
			omp_set_num_threads(v_threads.at(k));

		    for (int i = 0; i < (int) t_vals.size(); ++i){
		    	int t= t_vals.at(i);
		    	//int size=pow(2,n_vals_pow.at(i));
	    		int size=1000000;
		    	//int size=ceil((double)t_vals.at(i)/scale);

		    	for (int j = 0; j < no_runs; ++j){
		    		printf("--------------------------------t %d, size %d, run %d----------------------------------------------------------------------\n", t, size, j);
					printf("threads:%d\n", v_threads.at(k));
					printf("cprg:%d\n",cprg );
				
					run_mpfss_naive(remote_host, port, cp, t, size,cprg,v_threads.at(k));
        			if (cp==2){
						this_thread::sleep_for(chrono::seconds(2));        
        			}
				}
			}
		}
	} else {
        printf("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
                 "\tHostname usage:\n" 
                 "\tlocal -> 'localhost' remote -> IP address or DNS name\n", argv[0]);
    }
}

