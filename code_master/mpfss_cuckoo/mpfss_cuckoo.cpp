#include <vector> 
#include <string>
#include <fstream>
#include<iostream>
#include <chrono>
#include <ctime> 

extern "C" {
    #include "obliv.h"
    #include "includes/util.h"
    #include "includes/dbg.h"
    #include "mpfss_cuckoo.oh"
    #include "create_structs.h"
}
#include "includes/benchmark.h"
#include "buckets.h"
#include "includes/cuckoo/cuckoo.h"
#include "mpfss_cuckoo.h"
using namespace std;



ProtocolDesc prepare_connection(int cp,const char *remote_host, const char *port ){

        ProtocolDesc pd;
        
        // Make connection between two shells
        // Modified ocTestUtilTcpOrDie() function from obliv-c/test/oblivc/common/util.c
        if(cp == 1) {
            log_info("Waiting for connection from %s on port %s ...\n", remote_host, port);
            if(protocolAcceptTcp2P(&pd,port)!=0) {
                log_err("TCP accept from %s failed\n", remote_host);
                exit(1);
            }
        } else {
            log_info("Connecting to %s on port %s ...\n", remote_host, port);
            if(protocolConnectTcp2P(&pd,remote_host,port)!=0) {
                log_err("TCP connect to %s failed\n", remote_host);
                exit(1);
            }
        }

        // Final initializations before entering protocol

        setCurrentParty(&pd, cp); // only checks for a '1'   

        return pd;

}



/*void run_mpfss_cuckoo(int t, int size, mpfss_cuckoo_args<int> *mc_args){

    if(mc_args->print_stdout)log_info("Party %d \n", mc_args->cp);
    if(!mc_args->connection_already_exists){
    	mc_args->pd=prepare_connection(mc_args->cp, mc_args->host, mc_args->port);
	}

        //--------------------Setting Parameters ------------------------------------------------------

            //following Angle et al. : PIR..
            int w= 3;   //Number of hash functions
            int b= 1.5*t; //Number of buckets --> TODO: AB oder AUF runden
            
            //Making max_loop dependent on the size of the input field
            //TODO: find good value for max_loop
            int max_loop=0.5*size;
            mpfss_cuckoo *m=new_mpfss_cuckoo(t, size, w, b, max_loop,mc_args->cp, (int)mc_args->do_benchmark);
            int (*func)( int, int)=hashfunc_absl;
            int memblocksize=16; //TODO

            
        //--------------------Create Indices----------------------------------------------------------------------
            auto start_indices = std::chrono::system_clock::now();
            int *indices_notobliv;
     		if(mc_args->cp==1){
	            if(mc_args->indices_set==1){
		            	if(mc_args->print_stdout)log_info("Indices provided by caller\n");
			       	    indices_notobliv = mc_args->indices.data();
	        	}else if(mc_args->indices_set==-1){
		       	    indices_notobliv = (int *)calloc(t, sizeof(int ));
		       	    std::vector<int> indices_notobliv_vec;
		       	    if(mc_args->cp==1){
		       	    	if(mc_args->print_stdout)log_info("Creating Indices iteratively\n");
		       	    	for (int i = 0; i < t; ++i){
		       	    		indices_notobliv[i]=i;
		       	    		indices_notobliv_vec.push_back(i);
		       	    	}
		       	    }
		       	    mc_args->indices=indices_notobliv_vec;

	        	}else{
		       	    indices_notobliv = (int *)calloc(t, sizeof(int ));

		            if(mc_args->print_stdout)log_info("Creating Indices randomly\n");
		            bool created=false;
		            int count=0;
		            while(!created && count<100000){ 
		                created=create_indices(indices_notobliv, t , size);
		                count++;
		            }
		            if(created==false){
		                log_err("Error: Could not create Indices.\n" );
		                exit(1);
		            }	              
		           std::vector<int> indices_notobliv_vec(indices_notobliv, indices_notobliv+t);
		           // vector<int> indices_notobliv_vec(std::begin(*indices_notobliv), std::end(indices_notobliv));
		           	mc_args->indices=indices_notobliv_vec;
	
	        	}
	        	#ifdef TESTING
			        if(mc_args->print_stdout)log_info("Write Indices to file\n");
			        string filename="results_debug_mpfss_cuckoo_indices.txt";
			        ofstream file;
			        try{
			            file.open (filename);
			            for (int i = 0; i < t; ++i){
			                file << indices_notobliv[i]<<"\n";
			            }

			        	file.close();
			        }catch(ios_base::failure& e){
			            if(mc_args->print_stdout)log_err("%s",e.what());
			        }
		        #endif
	    	}else{
	    		indices_notobliv= (int *)calloc(t, sizeof(int ));
	    	}

    		auto end_indices = std::chrono::system_clock::now();
    		std::chrono::duration<double> runtime_indices = end_indices-start_indices;
            if(mc_args->print_stdout)log_info("Time to create indices : %lf seconds\n", runtime_indices.count());


        //--------------------Create Buckets----------------------------------------------------------------------
            auto start_buckets = std::chrono::system_clock::now();

            if(!mc_args->buckets_set){
                if(mc_args->print_stdout) log_info("Creating Buckets\n");
                
                std::vector<int> rands=create_rand_vector(w);
                #if defined(INFO) || defined(DEBUG)
                    printf("rands Party %d :", mc_args->cp);
              		print_vector(rands);
 				#endif
                mc_args->rands=rands;
                int *bucket_lenghts=(int *) calloc(b, sizeof(int));
                vector<vector<int>> all_buckets = preparations(m,bucket_lenghts,func, mc_args->rands);
                mc_args->all_buckets=all_buckets;
                mc_args->bucket_lenghts=bucket_lenghts;

                int ** all_buckets_array=(int **) calloc(b, sizeof(int *));
                for (int i = 0; i < b; ++i){
                    all_buckets_array[i]=mc_args->all_buckets.at(i).data();
                }
                mc_args->all_buckets_array=all_buckets_array;    
            }else{
            	if(mc_args->print_stdout) log_info("Buckets were provided by caller\n");
            }
            

    		auto end_buckets = std::chrono::system_clock::now();
    		std::chrono::duration<double> runtime_buckets = end_buckets-start_buckets;
            if(mc_args->print_stdout)log_info("Time to create Buckets : %lf seconds\n", runtime_buckets.count());

        //--------------------Create Assignment----------------------------------------------------------------------
            auto start_proto = std::chrono::system_clock::now();
            if(mc_args->print_stdout) log_info("Creating Assignment\n");
            
            match **matches = (match **) calloc(b, sizeof(match*));
            int evictions_logging=0;
            bool succ=create_assignement(m, indices_notobliv, matches, func, mc_args->all_buckets, &evictions_logging, mc_args->rands );
            if(!succ){
                exit(1);
            }

			std::chrono::duration<double>  runtime_assignment;
            auto end_assi = std::chrono::system_clock::now();
    		runtime_assignment = end_assi-start_proto;
            if(mc_args->print_stdout)log_info("Time to create Assignment : %lf seconds\n", runtime_assignment.count());

            yao_arguments *y_args= (yao_arguments *) calloc(1, sizeof(yao_arguments));
            y_args->m=m;
            y_args->bucket_lenghts=mc_args->bucket_lenghts;
            y_args->matches=matches;
            y_args->all_buckets_array=mc_args->all_buckets_array;
            y_args->set_beta=abs(mc_args->set_beta);
            y_args->cprg=mc_args->cprg;

            int lim=memblocksize;
            if(sizeof(int)<(size_t)memblocksize){
                lim=sizeof(int);
            }
            if(mc_args->set_beta==1){
                
                    for (int i = 0; i < b; ++i){
                        uint8_t *this_beta= (uint8_t *)calloc(16, sizeof(uint8_t));
                        int beta_int=mc_args->beta_vector.at(i);
                        for (int j = 0; j < lim; ++j){
                            uint8_t x = ((uint8_t *)(&beta_int))[j];
                            this_beta[j]=x;
                        } 
                        //TODO              
                    }
            }else if(mc_args->set_beta==-1){
                for (int i = 0; i < b; ++i){
                    uint8_t *this_beta= (uint8_t *)calloc(16, sizeof(uint8_t));
                    int beta_int=50;
                    for (int j = 0; j < lim; ++j){
                        uint8_t x = ((uint8_t *)(&beta_int))[j];                            this_beta[j]=x;
                       }      
                }
            }

        //--------------------Execute Yao's protocol and cleanup----------------------------------------------------------------------
            
            if(mc_args->print_stdout) log_info("Executing Yao Protocol\n");
           
            execYaoProtocol(&mc_args->pd, mpfss_batch_cuckoo, y_args);
            cleanupProtocol(&mc_args->pd);
            
        //--------------------Print results----------------------------------------------------------------------
            auto end_proto = std::chrono::system_clock::now();
    		std::chrono::duration<double> runtime = end_proto-start_proto;
            if(mc_args->print_stdout)log_info("Time to execute Yao Protocol : %lf seconds\n", runtime.count());


            if (mc_args->do_benchmark && mc_args->cp==1){
                std::vector<string> list_of_names={"runtime","t","size","no_buckets b", "no_hashfunctions w", "max_loop", "max_loop_reached", "evictions", "runtime_indices","runtime_buckets", "runtime_assignment", };
                std::vector<string> list_of_values={to_string(runtime.count()),to_string(t),to_string(size),to_string(b),to_string(w),
                    to_string(max_loop), "no", to_string(evictions_logging),to_string(runtime_indices.count()),
                    to_string(runtime_buckets.count()), to_string(runtime_assignment.count()) };
                benchmark_list("cuckoo", list_of_names.size(), list_of_names, list_of_values);
            }
            
                if(mc_args->print_stdout)printf("t:%d\n"                     , t);
                if(mc_args->print_stdout)printf("size:%d\n"                  , size);
                if(mc_args->print_stdout)printf("b:%d\n"                     , b);
                if(mc_args->print_stdout)printf("w:%d\n"                     , w);
                if(mc_args->print_stdout)printf("max_loop:%d\n"              , max_loop);
                if(mc_args->print_stdout)printf("evictions:%d\n"             , evictions_logging);
                if(mc_args->print_stdout)printf("cprg:%d\n"                  , (int)mc_args->cprg);
                if(mc_args->print_stdout)printf("runtime:%lf\n"              , runtime.count());
                if(mc_args->print_stdout)printf("runtime_indices:%lf\n"      , runtime_indices.count());
                if(mc_args->print_stdout)printf("runtime_buckets:%lf\n"      , runtime_buckets.count());
                if(mc_args->print_stdout)printf("runtime_assignment:%lf\n"   , runtime_assignment.count());

        //--------------------Prepare results for c++----------------------------------------------------------------------     
            std::vector<int> v;
            std::vector<bool> v_bit;
            for (int i = 0; i < size; ++i){

                uint8_t *this_value_vector=y_args->mpfss_output[i];
                int value;
                for (int i = 0; i < lim; ++i){
                    value = ((int)value << 8*i) | this_value_vector[i];
                }
                v.push_back(value);
                v_bit.push_back(y_args->mpfss_bit_output[i]);
            }
            mc_args->mpfss_output=v;
            mc_args->mpfss_bit_output=v_bit;
            mc_args->mpfss_output_raw=y_args->mpfss_output;
            mc_args->mpfss_bit_output_raw=y_args->mpfss_bit_output;

    free(m);
    if(!(mc_args->indices_set==1 && mc_args->cp==1)){
    	free(indices_notobliv);
    }

    for (int i = 0; i < b; ++i){
        free(matches[i]);   
    }
    free(matches);
    free(y_args);
}*/

void free_mc_args( mpfss_cuckoo_args<int> *mc_args){

    for (int i = 0; i < (int)mc_args->rands.size(); ++i){
        free(mc_args->all_buckets_array[i]);
    }
    free(mc_args->all_buckets_array);
    free(mc_args->bucket_lenghts);
    free(mc_args->mpfss_output_raw);
    free(mc_args->mpfss_bit_output_raw);

    free(mc_args);

}

void free_mc_args_content( mpfss_cuckoo_args<int> mc_args){

    for (int i = 0; i < (int)mc_args.rands.size(); ++i){
        free(mc_args.all_buckets_array[i]);
    }
    free(mc_args.all_buckets_array);
    free(mc_args.bucket_lenghts);
    for (int i = 0; i < mc_args.size; ++i){
        free(mc_args.mpfss_output_raw[i]);
    }
    free(mc_args.mpfss_output_raw);
    free(mc_args.mpfss_bit_output_raw);


}

