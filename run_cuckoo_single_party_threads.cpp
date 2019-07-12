#include <string>
#include <iostream>
#include <thread>
#include <vector> 
#include <math.h>       /* pow */
#include <omp.h>


#include "code_master/mpfss_cuckoo/mpfss_cuckoo.h"

using namespace std;

int main(int argc, char *argv[]) {



    const char *remote_host = strtok(argv[1], ":");
    const char *port = strtok(NULL, ":");
	int cp = (argv[2][0]=='1'? 1 : 2);

	bool cprg=true;

    #ifdef TESTING
		vector<int> t_vals{ 4 }; 
		vector<int> n_vals_pow{4}; 
		int no_runs=2;
	    vector<int> v_threads{ 4,6, 8};
    #else
	   	//vector<int> t_vals{ 74,192,382,741,1422,5205 }; 
	   	//vector<int> n_vals_pow{ 11,14,16,18,20,24}; 
		vector<int> t_vals{ 741 }; 
	   	vector<int> n_vals_pow{ 18};
	    vector<int> v_threads{8,4,2,1};
	    //vector<int> v_threads{2,4,8};
		int no_runs=10;
		//int no_runs=1;
  	#endif

	for (int k = 0; k < (int)v_threads.size(); ++k){

		omp_set_num_threads(v_threads.at(k));
	    for (int i = 0; i < (int) t_vals.size(); ++i){
	    	int t= t_vals.at(i);
	    	int size=pow(2,n_vals_pow.at(i));

		    mpfss_cuckoo_args<int> mc_args;
		    mc_args.cp=cp;
		    mc_args.do_benchmark=true;
		    mc_args.size=size;
		    mc_args.t=t;
		    mc_args.port=port;
		    mc_args.host=remote_host;
		    mc_args.connection_already_exists=false;
		    mc_args.print_stdout=true;
		    mc_args.buckets_set=false;
		    mc_args.cprg=cprg;
		    MPFSS_Cuckoo<int> mc;
		    mc.mc_args=mc_args;
		    absl::Span<int> span_output;
		    srand (time(NULL));

			absl::Span<const int64_t> indices;
			absl::Span<const int> y;

	    	for (int j = 0; j < no_runs; ++j){
	    		printf("--------------------------------t %d, size %d, run %d----------------------------------------------------------------------\n", t, size, j);
				printf("threads:%d\n", v_threads.at(k));
				printf("cprg:%d\n", cprg);
				
				mc.mc_args.threads= v_threads.at(k);
				if(j>0){
					mc.mc_args.buckets_set=true;
				}    		

				if (cp==1){
					#ifdef DEBUG
			        debug("indices: ");
			        #endif
			        std::vector<int64_t> v;
			        for (int i = 0; i < t; ++i){
			            int r=rand()%size;
			            while ((std::find(v.begin(), v.end(), r) != v.end())or r==0){
			                r=rand()%size;
			            }
			            v.push_back((int64_t)(r));     
			            #ifdef DEBUG
			            printf("%d ", r);
			            #endif
			        }
			        #ifdef DEBUG
			        printf("\n");
			        #endif

			        indices=absl::Span<const int64_t>(v);

			        #ifdef DEBUG
			        printf("beta: ");
			        #endif
			        std::vector<int>v_y;
			        for (int i = 0; i < t; ++i){
			            int r=( int) (rand()%size);
			            while(r==0){
			                r=( int) (rand()%size);
			            }
			            v_y.push_back(r);
			            #ifdef DEBUG
			            printf("%d ",r );
			            #endif
			        }
			        y=absl::Span<const int>(v_y);
			        #ifdef DEBUG
			        printf("\n");
			        #endif	
        			mpc_utils::Status stat=mc.RunIndexProvider(y, indices, span_output);
					this_thread::sleep_for(chrono::seconds(cp*1));
				}else{
					#ifdef DEBUG
			        printf("beta: ");
			        #endif
			        std::vector<int>v_y;
			        for (int i = 0; i < t; ++i){
			            int r=( int) (rand()%255);
			            while(r==0){
			                r=( int) (rand()%255);
			            }
			            v_y.push_back(r);
			        }
					y=absl::Span<const int>(v_y);
        			mpc_utils::Status stat=mc.RunValueProvider(y, span_output);

					this_thread::sleep_for(chrono::seconds(cp*1));
				}
			}
		}	
	}
}

