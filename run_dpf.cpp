#include <string>
#include <iostream>
#include <thread>
#include <vector> 
#include <math.h>       /* pow */
#include <string.h>
#include <omp.h>

extern "C"{
	#include "includes/mpfss_naive/mpfss_naive.h"
}

using namespace std;

int main(int argc, char *argv[]) {


    const char *remote_host = strtok(argv[1], ":");
    const char *port = strtok(NULL, ":");
	int cp = (argv[2][0]=='1'? 1 : 2);


    #ifdef TESTING
		vector<int> n_vals_pow{4}; 
		int no_runs=2;
		vector<int> thread_vals{1, 2, 4, 8, 16, 32};

    #else

	  	vector<int> n_vals_pow{25}; 
	    //vector<int> thread_vals{ 8, 4,2};
		vector<int> thread_vals{ 2};
		int no_runs=4;
  	#endif

	for (int k = 0; k < (int) thread_vals.size(); ++k)
	{
		omp_set_num_threads(thread_vals.at(k));
	
	    for (int i = 0; i < (int) n_vals_pow.size(); ++i){
	    	int t= 1;
	    	int size=pow(2,n_vals_pow.at(i));
	    	//int size=1000000;

	    	for (int j = 0; j < no_runs; ++j){
	    		printf("--------------------------------t %d, size %d, threads %d, run %d----------------------------------------------------------------------\n"
	    			, t, size, thread_vals.at(k),j);

				run_mpfss_naive(remote_host,port,cp, t,size, true,thread_vals.at(k) );
				//thread t1(run_mpfss_naive, remote_host,port,cp, t,size);
				//t1.join();
				//printf("npow:%d\n",n_vals_pow.at(i) );
				printf("size:%d\n", size );
				printf("threads:%d\n", thread_vals.at(k));
				this_thread::sleep_for(chrono::seconds(cp*4));

			}
		}
	}
}

