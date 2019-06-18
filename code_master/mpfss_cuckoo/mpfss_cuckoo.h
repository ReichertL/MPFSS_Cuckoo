#pragma once

#include <vector> 
#include <string>
#include <fstream>
#include<iostream>
#include <chrono>
#include <thread>
#include <ctime> 

extern "C" {
    #include "obliv.h" 
    #include <obliv_common.h>   
    #include "includes/util.h"
    #include "includes/dbg.h"
    #include "mpfss_cuckoo.oh"
    #include "includes/create_structs/create_structs.h"
}
#include "mpc_utils/status_macros.h"
#include "absl/types/span.h"
#include "includes/benchmark.h"
#include "buckets.h"
#include "includes/cuckoo/cuckoo.h"

using namespace std;


template <typename T>
struct  mpfss_cuckoo_args{
	int cp; 			//needs to be set
	int t;				//needs to be set 
    int b;
    int w;
	int size;			//needs to be set
	bool do_benchmark; //needs to be set
	bool print_stdout=true; 
    bool cprg;

	ProtocolDesc pd; //set either pd or port and host
	const char *host;
    const char *port;
    bool connection_already_exists; //need to be set

	std::vector<int> rands;
	int *bucket_lenghts;
	bool buckets_set;
	int ** all_buckets_array;
	std::vector<std::vector<int>> all_buckets; //sollte das hier nicht auch T sein

	std::vector<T> mpfss_output;
	std::vector<bool> mpfss_bit_output;
	uint8_t **mpfss_output_raw;
	bool * mpfss_bit_output_raw;
	
	int set_beta;	//need to be set, 0: no beta, 1: take beta from vector

};

ProtocolDesc prepare_connection(int cp,const char *remote_host, const char *port );
void run_mpfss_cuckoo(int t, int size, mpfss_cuckoo_args<int> *mc_args);
void free_mc_args( mpfss_cuckoo_args<int> *mc_args);
void free_mc_args_content( mpfss_cuckoo_args<int> mc_args);


template <class T>
class MPFSS_Cuckoo {

public:
mpfss_cuckoo_args<T> mc_args;

// x is skalar so that output vector = x*(y_vector)
mpc_utils::Status RunValueProviderVectorOLE(T x, int y_len, absl::Span<T> span_output) {

    //cp should be 2
    if(mc_args.print_stdout)log_info("Party %d \n", mc_args.cp);
    if(!mc_args.connection_already_exists){
    	mc_args.pd=prepare_connection(mc_args.cp, mc_args.host, mc_args.port);
	}

    //--------------------Setting Parameters ------------------------------------------------------

    //following Angle et al. : PIR..
    mc_args.w= 3;   //Number of hash functions
    mc_args.b= 1.5*mc_args.t; //Number of buckets --> TODO: AB oder AUF runden
   
    //Making max_loop dependent on the size of the input field
    //TODO: find good value for max_loop
    int max_loop=0.5*y_len;
    mpfss_cuckoo *m=new_mpfss_cuckoo(mc_args.t, y_len, mc_args.w, mc_args.b, max_loop,2, (int)mc_args.do_benchmark);
    int (*func)( int, int)=hashfunc_absl;
    int memblocksize=16; //TODO
    int lim=memblocksize;
    if((int)sizeof(T)<lim){
        lim=sizeof(T);
    }

    uint8_t *skalar=(uint8_t *)calloc(memblocksize, sizeof(uint8_t));
    for (int i = 0; i < lim; ++i){
        skalar[memblocksize -1 - i] = (uint8_t)(x >> (i * 8));
    }

    std::vector<int> indices_no;


    //--------------------Create Buckets----------------------------------------------------------------------
    auto start_buckets = std::chrono::system_clock::now();

    if(!mc_args.buckets_set){
    	if(mc_args.print_stdout) log_info("Creating Buckets\n");
                
        std::vector<int> rands=create_rand_vector(mc_args.w);
        #if defined(INFO) || defined(DEBUG)
            printf("rands Party %d :", mc_args.cp);
            print_vector(rands);
 		#endif
	    mc_args.rands=rands;
	    int *bucket_lenghts=(int *) calloc(mc_args.b, sizeof(int));
	    vector<vector<int>> all_buckets = preparations(m,bucket_lenghts,func, mc_args.rands);
	    mc_args.all_buckets=all_buckets;
	    mc_args.bucket_lenghts=bucket_lenghts;

	    int ** all_buckets_array=(int **) calloc(mc_args.b, sizeof(int *));
	    for (int i = 0; i < mc_args.b; ++i){
	        all_buckets_array[i]=mc_args.all_buckets.at(i).data();
	    }
	    
	    mc_args.all_buckets_array=all_buckets_array;    
    }else{
        if(mc_args.print_stdout) log_info("Buckets were provided by caller\n");
    }
            

    auto end_buckets = std::chrono::system_clock::now();
    std::chrono::duration<double> runtime_buckets = end_buckets-start_buckets;
    if(mc_args.print_stdout)log_info("Time to create Buckets : %lf seconds\n", runtime_buckets.count());

    
    //--------------------Create Assignment----------------------------------------------------------------------
    auto start_proto = std::chrono::system_clock::now();
    if(mc_args.print_stdout) log_info("Creating Assignment\n");
            
    match **matches = (match **) calloc(mc_args.b, sizeof(match*));
    int evictions_logging=0;
    bool succ;
    orecv(&mc_args.pd,2,&succ,sizeof(bool));
    if(!succ ){
        return mpc_utils::Status{mpc_utils::StatusCode::kFailedPrecondition, "Assignment could not be created."};
    }

	std::chrono::duration<double>  runtime_assignment;
    auto end_assi = std::chrono::system_clock::now();
    runtime_assignment = end_assi-start_proto;
    if(mc_args.print_stdout)log_info("Time to create Assignment : %lf seconds\n", runtime_assignment.count());

    yao_arguments *y_args= (yao_arguments *) calloc(1, sizeof(yao_arguments));
    y_args->m=m;
    y_args->bucket_lenghts=mc_args.bucket_lenghts;
    y_args->matches=matches;
    y_args->all_buckets_array=mc_args.all_buckets_array;
    y_args->set_beta=1;
    y_args->beta_value_vector=NULL;
    y_args->skalar=skalar;
    y_args->cprg=mc_args.cprg;

    //--------------------Execute Yao's protocol and cleanup----------------------------------------------------------------------
        
    if(mc_args.print_stdout) log_info("Executing Yao Protocol\n");   
    execYaoProtocol(&mc_args.pd, mpfss_vole_batch_cuckoo, y_args);
    cleanupProtocol(&mc_args.pd);
        
    //--------------------Print results----------------------------------------------------------------------
    auto end_proto = std::chrono::system_clock::now();
    std::chrono::duration<double> runtime = end_proto-start_proto;
    if(mc_args.print_stdout)log_info("Time to execute Yao Protocol : %lf seconds\n", runtime.count());

    if (mc_args.do_benchmark){
        std::vector<string> list_of_names={"runtime","t","size","no_buckets b", "no_hashfunctions w", "max_loop", "max_loop_reached", "evictions","runtime_buckets", "runtime_assignment", };
        std::vector<string> list_of_values={to_string(runtime.count()),to_string(mc_args.t),to_string(mc_args.size),to_string(mc_args.b),to_string(mc_args.w),
        to_string(max_loop), "no", to_string(evictions_logging),
        to_string(runtime_buckets.count()), to_string(runtime_assignment.count()) };
        benchmark_list("cuckoo_vole", list_of_names.size(), list_of_names, list_of_values);
    }
            
    if(mc_args.print_stdout)printf("t:%d\n"                     , mc_args.t);
    if(mc_args.print_stdout)printf("size:%d\n"                  , mc_args.size);
    if(mc_args.print_stdout)printf("b:%d\n"                     , mc_args.b);
    if(mc_args.print_stdout)printf("w:%d\n"                     , mc_args.w);
    if(mc_args.print_stdout)printf("max_loop:%d\n"              , max_loop);
    if(mc_args.print_stdout)printf("evictions:%d\n"             , evictions_logging);
    if(mc_args.print_stdout)printf("cprg:%d\n"                  , (int)mc_args.cprg);
    if(mc_args.print_stdout)printf("runtime:%lf\n"              , runtime.count());
    if(mc_args.print_stdout)printf("runtime_buckets:%lf\n"      , runtime_buckets.count());
    if(mc_args.print_stdout)printf("runtime_assignment:%lf\n"   , runtime_assignment.count());

    
    //--------------------Prepare results for c++----------------------------------------------------------------------     

    std::vector<T> v(mc_args.size);
    std::vector<bool> v_bit(mc_args.size);
    for (int i = 0; i < mc_args.size; ++i){

        uint8_t *this_value_vector=y_args->mpfss_output[i];
        T value;
        
        for (int ii = memblocksize-lim; ii < memblocksize; ++ii){
            value = ((T)value << 8) | this_value_vector[ii];
        }
        
        v.at(i)=value;
        bool b =y_args->mpfss_bit_output[i];
        v_bit.at(i)=b;

    }
    
    span_output=absl::Span<T>(v);
    mc_args.mpfss_output=v;
    mc_args.mpfss_bit_output=v_bit;
    mc_args.mpfss_output_raw=y_args->mpfss_output;
    mc_args.mpfss_bit_output_raw=y_args->mpfss_bit_output;
    


    free(m);
    for (int i = 0; i < mc_args.b; ++i){
        free(matches[i]);   
    }
    free(matches);
    free(y_args);

    return mpc_utils::OkStatus();
}

mpc_utils::Status RunIndexProviderVectorOLE(absl::Span<const T> y, absl::Span<const int64_t> indices, absl::Span<T> span_output){


    if(mc_args.print_stdout)log_info("Party %d \n", mc_args.cp);
    if(!mc_args.connection_already_exists){
    	mc_args.pd=prepare_connection(mc_args.cp, mc_args.host, mc_args.port);
	}

    //--------------------Setting Parameters ------------------------------------------------------

        //following Angle et al. : PIR..
        mc_args.w= 3;   //Number of hash functions
        mc_args.b= 1.5*mc_args.t; //Number of buckets --> TODO: AB oder AUF runden


        //Making max_loop dependent on the size of the input field
        //TODO: find good value for max_loop
        int max_loop=0.5*mc_args.size;
        mpfss_cuckoo *m=new_mpfss_cuckoo(mc_args.t, mc_args.size, mc_args.w, mc_args.b, max_loop,1, (int)mc_args.do_benchmark);
        int (*func)( int, int)=hashfunc_absl;
        int memblocksize=16; //TODO
        int lim=memblocksize;
        if((int)sizeof(T)<memblocksize){
            lim=sizeof(T);
        }

        const long int* temp=indices.data();
        std::vector<int> indices_no;
        indices_no.assign(temp, temp+mc_args.t);

        uint8_t **beta_value_vector=(uint8_t **)calloc(mc_args.t, sizeof(uint8_t *) );
        for (int i = 0; i < mc_args.t; ++i){
            uint8_t *this_beta= (uint8_t *)calloc(memblocksize, sizeof(uint8_t));
            T beta_int=y.at(i);
            for (int j = 0; j < lim; ++j){
                this_beta[memblocksize -1 - j] = (uint8_t)(beta_int >> (j * 8));
            }
            beta_value_vector[i]=this_beta;
        }
    	
    //--------------------Create Buckets----------------------------------------------------------------------
        auto start_buckets = std::chrono::system_clock::now();

        if(!mc_args.buckets_set){
        	if(mc_args.print_stdout) log_info("Creating Buckets\n");
                    
            std::vector<int> rands=create_rand_vector(mc_args.w);
            #if defined(INFO) || defined(DEBUG)
                printf("rands Party %d :", mc_args.cp);
                print_vector(rands);
     		#endif
    	    mc_args.rands=rands;
    	    int *bucket_lenghts=(int *) calloc(mc_args.b, sizeof(int));
    	    vector<vector<int>> all_buckets = preparations(m,bucket_lenghts,func, mc_args.rands);
    	    mc_args.all_buckets=all_buckets;
    	    mc_args.bucket_lenghts=bucket_lenghts;

    	    int ** all_buckets_array=(int **) calloc(mc_args.b, sizeof(int *));
    	    for (int i = 0; i < mc_args.b; ++i){
    	        all_buckets_array[i]=mc_args.all_buckets.at(i).data();
    	    }
    	    
    	    mc_args.all_buckets_array=all_buckets_array;    
        }else{
            if(mc_args.print_stdout) log_info("Buckets were provided by caller\n");
        }
                

        auto end_buckets = std::chrono::system_clock::now();
        std::chrono::duration<double> runtime_buckets = end_buckets-start_buckets;
        if(mc_args.print_stdout)log_info("Time to create Buckets : %lf seconds\n", runtime_buckets.count());

        
    //--------------------Create Assignment----------------------------------------------------------------------
        auto start_proto = std::chrono::system_clock::now();
        if(mc_args.print_stdout) log_info("Creating Assignment\n");
                
        match **matches = (match **) calloc(mc_args.b, sizeof(match*));
        int evictions_logging=0;
        bool succ=create_assignement(m, indices_no, matches, func, mc_args.all_buckets, &evictions_logging, mc_args.rands );
        osend(&mc_args.pd,2,&succ,sizeof(bool));
        if(!succ ){
            return mpc_utils::Status{mpc_utils::StatusCode::kFailedPrecondition, "Assignment could not be created."};
        }

    	std::chrono::duration<double>  runtime_assignment;
        auto end_assi = std::chrono::system_clock::now();
        runtime_assignment = end_assi-start_proto;
        if(mc_args.print_stdout)log_info("Time to create Assignment : %lf seconds\n", runtime_assignment.count());

    yao_arguments *y_args= (yao_arguments *) calloc(1, sizeof(yao_arguments));
    y_args->m=m;
    y_args->bucket_lenghts=mc_args.bucket_lenghts;
    y_args->matches=matches;
    y_args->all_buckets_array=mc_args.all_buckets_array;
    y_args->set_beta=1;
    y_args->beta_value_vector=beta_value_vector; 
    y_args->cprg=mc_args.cprg;


    //--------------------Execute Yao's protocol and cleanup----------------------------------------------------------------------
        
        if(mc_args.print_stdout) log_info("Executing Yao Protocol\n");   
        execYaoProtocol(&mc_args.pd, mpfss_vole_batch_cuckoo, y_args);
        cleanupProtocol(&mc_args.pd);
        
    //--------------------Print results----------------------------------------------------------------------
        auto end_proto = std::chrono::system_clock::now();
        std::chrono::duration<double> runtime = end_proto-start_proto;
        if(mc_args.print_stdout)log_info("Time to execute Yao Protocol : %lf seconds\n", runtime.count());

        if (mc_args.do_benchmark){
            std::vector<string> list_of_names={"runtime","t","size","no_buckets b", "no_hashfunctions w", "max_loop", "max_loop_reached", "evictions","runtime_buckets", "runtime_assignment", };
            std::vector<string> list_of_values={to_string(runtime.count()),to_string(mc_args.t),to_string(mc_args.size),to_string(mc_args.b),to_string(mc_args.w),
            to_string(max_loop), "no", to_string(evictions_logging),
            to_string(runtime_buckets.count()), to_string(runtime_assignment.count()) };
            benchmark_list("cuckoo_vole", list_of_names.size(), list_of_names, list_of_values);
        }
                
        if(mc_args.print_stdout)printf("t:%d\n"                     , mc_args.t);
        if(mc_args.print_stdout)printf("size:%d\n"                  , mc_args.size);
        if(mc_args.print_stdout)printf("b:%d\n"                     , mc_args.b);
        if(mc_args.print_stdout)printf("w:%d\n"                     , mc_args.w);
        if(mc_args.print_stdout)printf("max_loop:%d\n"              , max_loop);
        if(mc_args.print_stdout)printf("evictions:%d\n"             , evictions_logging);
        if(mc_args.print_stdout)printf("runtime:%lf\n"              , runtime.count());
        if(mc_args.print_stdout)printf("runtime_buckets:%lf\n"      , runtime_buckets.count());
        if(mc_args.print_stdout)printf("runtime_assignment:%lf\n"   , runtime_assignment.count());

    
    //--------------------Prepare results for c++----------------------------------------------------------------------    

        std::vector<T> v(mc_args.size);
        std::vector<bool> v_bit(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            uint8_t *this_value_vector=y_args->mpfss_output[i];
            T value;
            for (int ii = memblocksize-lim; ii < memblocksize; ++ii){
                value = ((T)value << 8) | this_value_vector[ii];
            }
            v.at(i)=value;
            bool b =y_args->mpfss_bit_output[i];
            v_bit.at(i)=b;
        }
        
        span_output=absl::Span<T>(v);
        mc_args.mpfss_output=v;
        mc_args.mpfss_bit_output=v_bit;
        mc_args.mpfss_output_raw=y_args->mpfss_output;
        mc_args.mpfss_bit_output_raw=y_args->mpfss_bit_output;
    
    free(m);
    for (int i = 0; i < mc_args.b; ++i){
        free(matches[i]);   
    }
    free(matches);
    free(y_args);
    for (int i = 0; i < mc_args.t; ++i){
        free(beta_value_vector[i]);
    }
    free(beta_value_vector);

    return mpc_utils::OkStatus();
}

mpc_utils::Status RunValueProvider(absl::Span<const T> y, absl::Span<T> span_output) {

    //cp should be 2
    if(mc_args.print_stdout)log_info("Party %d \n", mc_args.cp);
    if(!mc_args.connection_already_exists){
        mc_args.pd=prepare_connection(mc_args.cp, mc_args.host, mc_args.port);
    }


    //--------------------Setting Parameters ------------------------------------------------------

        //following Angle et al. : PIR..
        mc_args.w= 3;   //Number of hash functions
        mc_args.b= 1.5*mc_args.t; //Number of buckets --> TODO: AB oder AUF runden
       
        //Making max_loop dependent on the size of the input field
        //TODO: find good value for max_loop
        int max_loop=0.5*y.size();
        mpfss_cuckoo *m=new_mpfss_cuckoo(mc_args.t, mc_args.size, mc_args.w, mc_args.b, max_loop,2, (int)mc_args.do_benchmark);
        int (*func)( int, int)=hashfunc_absl;
        int memblocksize=16; //TODO
        int lim=memblocksize;
        if((int)sizeof(T)<lim){
            lim=sizeof(T);
        }


        uint8_t **beta_value_vector=(uint8_t **)calloc(mc_args.t, sizeof(uint8_t *) );
        for (int i = 0; i < mc_args.t; ++i){
            uint8_t *this_beta= (uint8_t *)calloc(memblocksize, sizeof(uint8_t));
            T beta_int=y.at(i);
            for (int j = 0; j < lim; ++j){
                this_beta[memblocksize -1 - j] = (uint8_t)(beta_int >> (j * 8));
            }
            beta_value_vector[i]=this_beta;
        }

        std::vector<int> indices_no;


    //--------------------Create Buckets----------------------------------------------------------------------
        auto start_buckets = std::chrono::system_clock::now();

        if(!mc_args.buckets_set){
            if(mc_args.print_stdout) log_info("Creating Buckets\n");
                    
            std::vector<int> rands=create_rand_vector(mc_args.w);
            #if defined(INFO) || defined(DEBUG)
                printf("rands Party %d :", mc_args.cp);
                print_vector(rands);
            #endif
            mc_args.rands=rands;
            int *bucket_lenghts=(int *) calloc(mc_args.b, sizeof(int));
            vector<vector<int>> all_buckets = preparations(m,bucket_lenghts,func, mc_args.rands);
            mc_args.all_buckets=all_buckets;
            mc_args.bucket_lenghts=bucket_lenghts;

            int ** all_buckets_array=(int **) calloc(mc_args.b, sizeof(int *));
            for (int i = 0; i < mc_args.b; ++i){
                all_buckets_array[i]=mc_args.all_buckets.at(i).data();
            }
            
            mc_args.all_buckets_array=all_buckets_array;    
        }else{
            if(mc_args.print_stdout) log_info("Buckets were provided by caller\n");
        }
                

        auto end_buckets = std::chrono::system_clock::now();
        std::chrono::duration<double> runtime_buckets = end_buckets-start_buckets;
        if(mc_args.print_stdout)log_info("Time to create Buckets : %lf seconds\n", runtime_buckets.count());

    
    //--------------------Create Assignment----------------------------------------------------------------------
        auto start_proto = std::chrono::system_clock::now();
        if(mc_args.print_stdout) log_info("Creating Assignment\n");
                
        match **matches = (match **) calloc(mc_args.b, sizeof(match*));
        for (int i = 0; i < mc_args.b; ++i){
            match *p=(match *) calloc(1, sizeof(match)); 
            matches[i]=p;
        }
        
        int evictions_logging=0;
        bool succ;
        orecv(&mc_args.pd,2,&succ,sizeof(bool));
        if(!succ ){
            return mpc_utils::Status{mpc_utils::StatusCode::kFailedPrecondition, "Assignment could not be created."};
        }

        std::chrono::duration<double>  runtime_assignment;
        auto end_assi = std::chrono::system_clock::now();
        runtime_assignment = end_assi-start_proto;
        if(mc_args.print_stdout)log_info("Time to create Assignment : %lf seconds\n", runtime_assignment.count());

    yao_arguments *y_args= (yao_arguments *) calloc(1, sizeof(yao_arguments));
    y_args->m=m;
    y_args->bucket_lenghts=mc_args.bucket_lenghts;
    y_args->matches=matches;
    y_args->all_buckets_array=mc_args.all_buckets_array;
    y_args->set_beta=1;
    y_args->beta_value_vector=beta_value_vector; 

    y_args->cprg=mc_args.cprg;

    //--------------------Execute Yao's protocol and cleanup----------------------------------------------------------------------
        
        if(mc_args.print_stdout) log_info("Executing Yao Protocol\n");   
        execYaoProtocol(&mc_args.pd, mpfss_batch_cuckoo, y_args);
        
    //--------------------Print results----------------------------------------------------------------------
        auto end_proto = std::chrono::system_clock::now();
        std::chrono::duration<double> runtime = end_proto-start_proto;
        if(mc_args.print_stdout)log_info("Time to execute Yao Protocol : %lf seconds\n", runtime.count());

        if (mc_args.do_benchmark){
            std::vector<string> list_of_names={"runtime","t","size","no_buckets b", "no_hashfunctions w", "max_loop", "max_loop_reached", "evictions","runtime_buckets", "runtime_assignment", };
            std::vector<string> list_of_values={to_string(runtime.count()),to_string(mc_args.t),to_string(mc_args.size),to_string(mc_args.b),to_string(mc_args.w),
            to_string(max_loop), "no", to_string(evictions_logging),
            to_string(runtime_buckets.count()), to_string(runtime_assignment.count()) };
            benchmark_list("cuckoo", list_of_names.size(), list_of_names, list_of_values);
        }
                
        if(mc_args.print_stdout)printf("t:%d\n"                     , mc_args.t);
        if(mc_args.print_stdout)printf("size:%d\n"                  , mc_args.size);
        if(mc_args.print_stdout)printf("b:%d\n"                     , mc_args.b);
        if(mc_args.print_stdout)printf("w:%d\n"                     , mc_args.w);
        if(mc_args.print_stdout)printf("max_loop:%d\n"              , max_loop);
        if(mc_args.print_stdout)printf("evictions:%d\n"             , evictions_logging);
        if(mc_args.print_stdout)printf("cprg:%d\n"                  , (int)mc_args.cprg);
        if(mc_args.print_stdout)printf("runtime:%lf\n"              , runtime.count());
        if(mc_args.print_stdout)printf("runtime_buckets:%lf\n"      , runtime_buckets.count());
        if(mc_args.print_stdout)printf("runtime_assignment:%lf\n"   , runtime_assignment.count());

    
    //--------------------Prepare results for c++----------------------------------------------------------------------     

        std::vector<T> v(mc_args.size);
        std::vector<bool> v_bit(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){

            uint8_t *this_value_vector=y_args->mpfss_output[i];
            T value;
            
            for (int ii = memblocksize-lim; ii < memblocksize; ++ii){
                value = ((T)value << 8) | this_value_vector[ii];
            }
            
            v.at(i)=value;
            bool b =y_args->mpfss_bit_output[i];
            v_bit.at(i)=b;

        }
        printf("\n");
    
        span_output=absl::Span<T>(v);
        mc_args.mpfss_output=v;
        mc_args.mpfss_bit_output=v_bit;
        mc_args.mpfss_output_raw=y_args->mpfss_output;
        mc_args.mpfss_bit_output_raw=y_args->mpfss_bit_output;
        
    #ifdef DEBUG
       
        int other_p;
        if (mc_args.cp==1){
            other_p=2;
        }else{
            other_p=1;
        }
        std::vector<bool> v_bit_op(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            bool b;
            orecv(&mc_args.pd,1 ,&b,sizeof(bool));
            v_bit_op.at(i)=b;
        }

        for (int i = 0; i < mc_args.size; ++i){
            bool b=mc_args.mpfss_bit_output.at(i);
            osend(&mc_args.pd,2,&b,sizeof(bool));
        }

        printf("MPFSS bit results\n");
        for (int i = 0; i < mc_args.size; ++i){
            bool v=mc_args.mpfss_bit_output.at(i)^v_bit_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

        std::vector<T> v_value_op(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            orecv(&mc_args.pd,1 ,&v_value_op.at(i),sizeof(T));
        }

        for (int i = 0; i < mc_args.size; ++i){
            osend(&mc_args.pd,2,&mc_args.mpfss_output.at(i),sizeof(T));
        }

        printf("MPFSS results \n");
        for (int i = 0; i < mc_args.size; ++i){
            T v=mc_args.mpfss_output.at(i)^v_value_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

    #endif

    cleanupProtocol(&mc_args.pd);
    free(m);
    for (int i = 0; i < mc_args.b; ++i){
        free(matches[i]);   
    }
    free(matches);
    free(y_args);
    for (int i = 0; i < mc_args.t; ++i){
        free(beta_value_vector[i]);
    }
    free(beta_value_vector);
    return mpc_utils::OkStatus();

}


mpc_utils::Status RunIndexProvider(absl::Span<const T> y, absl::Span<const int64_t> indices, absl::Span<T> span_output){


    if(mc_args.print_stdout)log_info("Party %d \n", mc_args.cp);
    if(!mc_args.connection_already_exists){
        mc_args.pd=prepare_connection(mc_args.cp, mc_args.host, mc_args.port);
    }


    //--------------------Setting Parameters ------------------------------------------------------

        //following Angle et al. : PIR..
        mc_args.w= 3;   //Number of hash functions
        mc_args.b= 1.5*mc_args.t; //Number of buckets --> TODO: AB oder AUF runden


        //Making max_loop dependent on the size of the input field
        //TODO: find good value for max_loop
        int max_loop=0.5*mc_args.size;
        mpfss_cuckoo *m=new_mpfss_cuckoo(mc_args.t, mc_args.size, mc_args.w, mc_args.b, max_loop,1, (int)mc_args.do_benchmark);
        int (*func)( int, int)=hashfunc_absl;
        int memblocksize=16; //TODO
        int lim=memblocksize;
        if((int)sizeof(T)<memblocksize){
            lim=sizeof(T);
        }

        const long int* temp=indices.data();
        std::vector<int> indices_no;
        indices_no.assign(temp, temp+mc_args.t);

        uint8_t **beta_value_vector=(uint8_t **)calloc(mc_args.t, sizeof(uint8_t *) );
        for (int i = 0; i < mc_args.t; ++i){
            uint8_t *this_beta= (uint8_t *)calloc(memblocksize, sizeof(uint8_t));
            T beta_int=y.at(i);
            for (int j = 0; j < lim; ++j){
                this_beta[memblocksize -1 - j] = (uint8_t)(beta_int >> (j * 8));
            }
            beta_value_vector[i]=this_beta;
        }

        /*for (int i = 0; i < mc_args.size; ++i)
        {
            uint8_t * this_beta=beta_vector[i];
            printf("%d : ", i );
            for (int j = 0; j < memblocksize; ++j)
            {
                printf("%d ", this_beta[j]);
            }
            printf("\n\n");
        }*/
    
    //--------------------Create Buckets----------------------------------------------------------------------
        auto start_buckets = std::chrono::system_clock::now();

        if(!mc_args.buckets_set){
            if(mc_args.print_stdout) log_info("Creating Buckets\n");
                    
            std::vector<int> rands=create_rand_vector(mc_args.w);
            #if defined(INFO) || defined(DEBUG)
                printf("rands Party %d :", mc_args.cp);
                print_vector(rands);
            #endif
            mc_args.rands=rands;
            int *bucket_lenghts=(int *) calloc(mc_args.b, sizeof(int));
            vector<vector<int>> all_buckets = preparations(m,bucket_lenghts,func, mc_args.rands);
            mc_args.all_buckets=all_buckets;
            mc_args.bucket_lenghts=bucket_lenghts;

            int ** all_buckets_array=(int **) calloc(mc_args.b, sizeof(int *));
            for (int i = 0; i < mc_args.b; ++i){
                all_buckets_array[i]=mc_args.all_buckets.at(i).data();
            }
            
            mc_args.all_buckets_array=all_buckets_array;    
        }else{
            if(mc_args.print_stdout) log_info("Buckets were provided by caller\n");
        }
                

        auto end_buckets = std::chrono::system_clock::now();
        std::chrono::duration<double> runtime_buckets = end_buckets-start_buckets;
        if(mc_args.print_stdout)log_info("Time to create Buckets : %lf seconds\n", runtime_buckets.count());

    
    //--------------------Create Assignment----------------------------------------------------------------------
        auto start_proto = std::chrono::system_clock::now();
        if(mc_args.print_stdout) log_info("Creating Assignment\n");
                
        match **matches = (match **) calloc(mc_args.b, sizeof(match*));
        int evictions_logging=0;
        bool succ=create_assignement(m, indices_no, matches, func, mc_args.all_buckets, &evictions_logging, mc_args.rands );
        osend(&mc_args.pd,2,&succ,sizeof(bool));
        if(!succ ){
            return mpc_utils::Status{mpc_utils::StatusCode::kFailedPrecondition, "Assignment could not be created."};
        }

        std::chrono::duration<double>  runtime_assignment;
        auto end_assi = std::chrono::system_clock::now();
        runtime_assignment = end_assi-start_proto;
        if(mc_args.print_stdout)log_info("Time to create Assignment : %lf seconds\n", runtime_assignment.count());

    yao_arguments *y_args= (yao_arguments *) calloc(1, sizeof(yao_arguments));
    y_args->m=m;
    y_args->bucket_lenghts=mc_args.bucket_lenghts;
    y_args->matches=matches;
    y_args->all_buckets_array=mc_args.all_buckets_array;
    y_args->set_beta=1;
    y_args->beta_value_vector=beta_value_vector; 
    y_args->cprg=mc_args.cprg;


    //--------------------Execute Yao's protocol and cleanup----------------------------------------------------------------------
        
        if(mc_args.print_stdout) log_info("Executing Yao Protocol\n");   
        execYaoProtocol(&mc_args.pd, mpfss_batch_cuckoo, y_args);
        
    //--------------------Print results----------------------------------------------------------------------
        auto end_proto = std::chrono::system_clock::now();
        std::chrono::duration<double> runtime = end_proto-start_proto;
        if(mc_args.print_stdout)log_info("Time to execute Yao Protocol : %lf seconds\n", runtime.count());

        if (mc_args.do_benchmark){
            std::vector<string> list_of_names={"runtime","t","size","no_buckets b", "no_hashfunctions w", "max_loop", "max_loop_reached", "evictions","runtime_buckets", "runtime_assignment", };
            std::vector<string> list_of_values={to_string(runtime.count()),to_string(mc_args.t),to_string(mc_args.size),to_string(mc_args.b),to_string(mc_args.w),
            to_string(max_loop), "no", to_string(evictions_logging),
            to_string(runtime_buckets.count()), to_string(runtime_assignment.count()) };
            benchmark_list("cuckoo", list_of_names.size(), list_of_names, list_of_values);
        }
                
        if(mc_args.print_stdout)printf("t:%d\n"                     , mc_args.t);
        if(mc_args.print_stdout)printf("size:%d\n"                  , mc_args.size);
        if(mc_args.print_stdout)printf("b:%d\n"                     , mc_args.b);
        if(mc_args.print_stdout)printf("w:%d\n"                     , mc_args.w);
        if(mc_args.print_stdout)printf("max_loop:%d\n"              , max_loop);
        if(mc_args.print_stdout)printf("evictions:%d\n"             , evictions_logging);
        if(mc_args.print_stdout)printf("runtime:%lf\n"              , runtime.count());
        if(mc_args.print_stdout)printf("runtime_buckets:%lf\n"      , runtime_buckets.count());
        if(mc_args.print_stdout)printf("runtime_assignment:%lf\n"   , runtime_assignment.count());

    
    //--------------------Prepare results for c++----------------------------------------------------------------------    

        std::vector<T> v(mc_args.size);
        std::vector<bool> v_bit(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){

            uint8_t *this_value_vector=y_args->mpfss_output[i];
            T value;

            for (int ii = memblocksize-lim; ii < memblocksize; ++ii){
                value = ((T)value << 8) | this_value_vector[ii];
            }
            
            v.at(i)=value;
            bool b =y_args->mpfss_bit_output[i];
            v_bit.at(i)=b;

        }
        
        span_output=absl::Span<T>(v);
        mc_args.mpfss_output=v;
        mc_args.mpfss_bit_output=v_bit;
        mc_args.mpfss_output_raw=y_args->mpfss_output;
        mc_args.mpfss_bit_output_raw=y_args->mpfss_bit_output;
    

    #ifdef DEBUG

        int other_p;
        if (mc_args.cp==1){
            other_p=2;
        }else{
            other_p=1;
        }

        for (int i = 0; i < mc_args.size; ++i){
            bool b= mc_args.mpfss_bit_output.at(i);
            osend(&mc_args.pd,1,&b,sizeof(bool));
        }

        std::vector<bool> v_bit_op(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            bool b;
            orecv(&mc_args.pd,2,&b,sizeof(bool));
            v_bit_op.at(i)=b;
        }

        printf("MPFSS bit results\n");
        for (int i = 0; i < mc_args.size; ++i){
            bool v=mc_args.mpfss_bit_output.at(i)^v_bit_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

        for (int i = 0; i < mc_args.size; ++i){
            osend(&mc_args.pd,1,&mc_args.mpfss_output.at(i),sizeof(T));
        }

        std::vector<T> v_value_op(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            orecv(&mc_args.pd,2,&v_value_op.at(i),sizeof(T));
        }

        printf("MPFSS results\n");
        for (int i = 0; i < mc_args.size; ++i){
            T v=mc_args.mpfss_output.at(i)^v_value_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

    #endif

    cleanupProtocol(&mc_args.pd);
    free(m);
    for (int i = 0; i < mc_args.b; ++i){
        free(matches[i]);   
    }
    free(matches);
    free(y_args);
    for (int i = 0; i < mc_args.t; ++i){
        free(beta_value_vector[i]);
    }
    free(beta_value_vector);

    return mpc_utils::OkStatus();
}
};



