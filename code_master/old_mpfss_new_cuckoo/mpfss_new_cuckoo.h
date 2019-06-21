#pragma once

#include <vector> 
#include <string>
#include <fstream>
#include<iostream>
#include <chrono>
#include <thread>
#include <ctime> 
#include <omp.h>

extern "C" {
    #include "obliv.h" 
    #include <obliv_common.h>   
    #include "includes/util.h"
    #include "includes/dbg.h"
    #include "mpfss_new_cuckoo.oh"
    #include "includes/create_structs/create_structs.h"
}

#include "mpc_utils/status_macros.h"
#include "absl/types/span.h"
#include "includes/benchmark.h"
#include "code_master/mpfss_cuckoo/buckets.h"
#include "includes/cuckoo/cuckoo.h"
#include "create_structs_new.h"

using namespace std;


template <typename T>
struct  mpfss_cuckoo_args_new{
	int cp; 			//needs to be set
	int t;				//needs to be set 
    int b;
    int w;
	int size;			//needs to be set
	bool do_benchmark; //needs to be set
	bool print_stdout=true; 
    bool cprg;
    mpfss_cuckoo *m;
    int (*func)( int, int);

	ProtocolDesc pd; //set either pd or port and host
	const char *host;
    const char *port;
    bool connection_already_exists; //need to be set

	std::vector<int> rands;
	int *bucket_lenghts;
	bool buckets_set;
	int ** all_buckets_array;
	std::vector<std::vector<int>> all_buckets; //sollte das hier nicht auch T sein
    match **matches;
    std::vector<yao_arguments_new*> y_args_all;
	
    std::vector<T> mpfss_output;
	std::vector<bool> mpfss_bit_output;
	uint8_t **mpfss_output_raw;
	bool * mpfss_bit_output_raw;
	
	int set_beta;	//need to be set, 0: no beta, 1: take beta from vector

};

ProtocolDesc prepare_connection(int cp,const char *remote_host, const char *port );
void run_mpfss_cuckoo(int t, int size, mpfss_cuckoo_args_new<int> *mc_args);
void free_mc_args( mpfss_cuckoo_args_new<int> *mc_args);
void free_mc_args_content_new( mpfss_cuckoo_args_new<int> mc_args);


template <class T>
class MPFSS_Cuckoo_New {

public:
int memblocksize=16; //TODO: get definitions from elsewhere
int lim=16;
mpfss_cuckoo_args_new<T> mc_args;
std::chrono::duration<double> runtime_assignment=  std::chrono::duration<double>::zero();
std::chrono::duration<double> runtime_buckets=     std::chrono::duration<double>::zero();
std::chrono::duration<double> runtime=              std::chrono::duration<double>::zero();
int evictions_logging=0;
std::vector<ProtocolDesc> pd_vec;
#ifdef DEBUG
ProtocolDesc pd_dbg;
#endif

void establish_connections(){
    #ifdef new_connect
            std::vector<ProtocolDesc> pd_temp(mc_args.b);
            pd_temp.at(0)=prepare_connection(mc_args.cp, mc_args.host, mc_args.port);

            for (int i = 1; i < mc_args.b; ++i){
                ProtocolDesc pd_split;
                ocSplitProto(&pd_split, &pd_vec.at(0));
                pd_temp.at(i)=pd_split;
                //this_thread::sleep_for(chrono::seconds(1));
            }
            pd_vec=pd_temp;
            #ifdef DEBUG
                ocSplitProto(&pd_dbg, &pd_vec.at(0));
            #endif
        #else
            std::vector<ProtocolDesc> pd_temp(mc_args.b);

            for (int i = 0; i < mc_args.b; ++i){
                pd_temp.at(i)=prepare_connection(mc_args.cp, mc_args.host, mc_args.port);
                this_thread::sleep_for(chrono::seconds(1));
                int a=5;
                osend(&pd_temp.at(i),1,&a,sizeof(int));

            }
            pd_vec=pd_temp;
            #ifdef DEBUG
                pd_dbg=prepare_connection(mc_args.cp, mc_args.host, mc_args.port);
            #endif
        #endif    
}
void create_buckets(){
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
            vector<vector<int>> all_buckets = preparations(mc_args.m,bucket_lenghts,mc_args.func, mc_args.rands);
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
        runtime_buckets = end_buckets-start_buckets;
        if(mc_args.print_stdout)log_info("Time to create Buckets : %lf seconds\n", runtime_buckets.count());
}

bool  run_create_assignement(std::vector<int> indices_no){
        auto start_assi = std::chrono::system_clock::now();
        
        if(mc_args.print_stdout) log_info("Creating Assignment\n");
        int succ=-1;        
        match **matches = (match **) calloc(mc_args.b, sizeof(match*));
        if(mc_args.cp==1){
            succ=create_assignement(mc_args.m, indices_no, matches, mc_args.func, mc_args.all_buckets, &evictions_logging, mc_args.rands );
            osend(&pd_vec.at(0),1,&succ,sizeof(int));
            oflush(&pd_vec.at(0));

        }else{
            for (int i = 0; i < mc_args.b; ++i){
                match *p=(match *) calloc(1, sizeof(match)); 
                matches[i]=p;
            }
            orecv(&pd_vec.at(0),1,&succ,sizeof(int));
        }
        mc_args.matches=matches;
        auto end_assi = std::chrono::system_clock::now();
        runtime_assignment = end_assi-start_assi;
        if(mc_args.print_stdout)log_info("Time to create Assignment : %lf seconds\n", runtime_assignment.count());
        return succ;
}

void run_exec_yao(uint8_t **beta_value_vector){
  auto start_proto = std::chrono::system_clock::now();

    std::vector<yao_arguments_new *> y_args_all(mc_args.b);
    for (int i = 0; i < mc_args.b; ++i){
        yao_arguments_new *y_args= (yao_arguments_new *) calloc(1, sizeof(yao_arguments_new));
        y_args_all.at(i)=y_args;
    }
    mc_args.y_args_all=y_args_all;

  // #pragma omp parallel for
    for (int i = 0; i < mc_args.b; ++i){
        //printf("Work started by tid %d/%d\n", omp_get_thread_num(), omp_get_num_threads());
        yao_arguments_new *y_args = y_args_all.at(i);
        y_args->m=mc_args.m;
        y_args->bucket_lenghts=mc_args.bucket_lenghts;
        y_args->matches=mc_args.matches;
        y_args->all_buckets_array=mc_args.all_buckets_array;
        y_args->beta_value_vector=beta_value_vector; 
        y_args->cprg=mc_args.cprg;
        y_args->bucket_no=i;

            
        if(mc_args.print_stdout) log_info("Executing Yao Protocol\n");   
        execYaoProtocol(&pd_vec.at(i), mpfss_single_bucket, y_args);
        cleanupProtocol(&pd_vec.at(i));

    }  


    auto end_proto = std::chrono::system_clock::now();
    runtime = end_proto-start_proto;
    if(mc_args.print_stdout)log_info("Time to execute Yao Protocol : %lf seconds\n", runtime.count());
}

void print_measurements(int max_loop){ 

        if (mc_args.do_benchmark){
            std::vector<string> list_of_names={
                "runtime",
                "t",
                "size",
                "no_buckets", 
                "no_hashfunctions", 
                "max_loop", 
                "max_loop_reached", 
                "evictions",
                "runtime_buckets", 
                "runtime_assignment", 
            };
            std::vector<string> list_of_values={
                to_string(runtime.count()),
                to_string(mc_args.t),
                to_string(mc_args.size),
                to_string(mc_args.b),
                to_string(mc_args.w),
                to_string(max_loop), 
                "no", 
                to_string(evictions_logging),
                to_string(runtime_buckets.count()), 
                to_string(runtime_assignment.count()) 
            };
            benchmark_list("cuckoo_new", list_of_names.size(), list_of_names, list_of_values);
        }
            
}

absl::Span<T> prepare_results_new(){

    std::vector<T> v_value(mc_args.size);
    std::vector<bool> v_bit(mc_args.size);


    //#pragma omp parallel for <- causes errors in results
    for(int i=0;i<mc_args.b; ++i){
        int *bucket=mc_args.all_buckets_array[i];
        int len=mc_args.bucket_lenghts[i];
        bool *dpf_bit_vector=mc_args.y_args_all.at(i)->dpf_bit_output;
        uint8_t *dpf_value_vector=mc_args.y_args_all.at(i)->dpf_output;

        for (int j=0; j<len;j++){
            bool is_set=dpf_bit_vector[j];
            int position=bucket[j];
            v_bit.at(position)=v_bit.at(position)^is_set;
            T value=0;

            for (int ii = memblocksize-lim; ii < memblocksize;++ii){
                uint8_t value_part=dpf_value_vector[j*memblocksize+ii];
                value = ((T)value << 8) | value_part;
            }
            v_value.at(position)=v_value.at(position)^value;
        }
    }
    
    absl::Span<T> span_output=absl::Span<T>(v_value);
    mc_args.mpfss_output=v_value;
    mc_args.mpfss_bit_output=v_bit;
    return span_output;
}


//TODO:rename?
//cp should be 2
mpc_utils::Status RunValueProvider(absl::Span<const T> y, absl::Span<T> span_output) {


    if(mc_args.print_stdout)log_info("Party %d \n", mc_args.cp);
   

    //--------------------Setting Parameters ------------------------------------------------------

        //following Angle et al. : PIR..
        mc_args.w= 3;   //Number of hash functions
        mc_args.b= 1.5*mc_args.t; //Number of buckets --> TODO: AB oder AUF runden
       
       establish_connections();

        //TODO: find good value for max_loop
        int max_loop=0.5*y.size();
        mpfss_cuckoo *m=new_mpfss_cuckoo(mc_args.t, mc_args.size, mc_args.w, mc_args.b, max_loop,2, (int)mc_args.do_benchmark);
        mc_args.m=m;
        mc_args.func=hashfunc_absl;

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
        
        create_buckets();

    //--------------------Create Assignment----------------------------------------------------------------------
        bool succ=run_create_assignement( indices_no);
        if(!succ){
            return mpc_utils::Status{mpc_utils::StatusCode::kFailedPrecondition, "Assignment could not be created."};
        }
    
    //--------------------Execute Yao's protocol and cleanup----------------------------------------------------------------------
        
        run_exec_yao(beta_value_vector);
    
    //--------------------Print results----------------------------------------------------------------------

        print_measurements( max_loop);    
    
    //--------------------Prepare results for c++----------------------------------------------------------------------     
        
        span_output=prepare_results_new();



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
            orecv(&pd_dbg,1 ,&b,sizeof(bool));
            v_bit_op.at(i)=b;
        }

        for (int i = 0; i < mc_args.size; ++i){
            bool b=mc_args.mpfss_bit_output.at(i);
            osend(&pd_dbg,2,&b,sizeof(bool));
        }

        printf("MPFSS bit results\n");
        for (int i = 0; i < mc_args.size; ++i){
            bool v=mc_args.mpfss_bit_output.at(i)^v_bit_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

        std::vector<T> v_value_op(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            orecv(&pd_dbg,1 ,&v_value_op.at(i),sizeof(T));
        }

        for (int i = 0; i < mc_args.size; ++i){
            osend(&pd_dbg,2,&mc_args.mpfss_output.at(i),sizeof(T));
        }

        printf("MPFSS results \n");
        for (int i = 0; i < mc_args.size; ++i){
            T v=mc_args.mpfss_output.at(i)^v_value_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

    #endif


    free(mc_args.m);
    for (int i = 0; i < mc_args.b; ++i){
        free(mc_args.matches[i]);   
    }
    free(mc_args.matches);
    for (int i = 0; i < mc_args.t; ++i){
        free(beta_value_vector[i]);
    }
    free(beta_value_vector);
    for (int i = 0; i < mc_args.b; ++i){
        yao_arguments_new *y_args=mc_args.y_args_all.at(i);
        free(y_args->dpf_bit_output);
        free(y_args->dpf_output);
        free(y_args);
    }

    return mpc_utils::OkStatus();

}


mpc_utils::Status RunIndexProvider(absl::Span<const T> y, absl::Span<const int64_t> indices, absl::Span<T> span_output){


    if(mc_args.print_stdout)log_info("Party %d \n", mc_args.cp);

   

    //--------------------Setting Parameters ------------------------------------------------------

        //following Angle et al. : PIR..
        mc_args.w= 3;   //Number of hash functions
        mc_args.b= 1.5*mc_args.t; //Number of buckets --> TODO: AB oder AUF runden
        
        establish_connections();

        //Making max_loop dependent on the size of the input field
        //TODO: find good value for max_loop
        int max_loop=0.5*mc_args.size;
        mpfss_cuckoo *m=new_mpfss_cuckoo(mc_args.t, mc_args.size, mc_args.w, mc_args.b, max_loop,1, (int)mc_args.do_benchmark);
        mc_args.m=m;
        mc_args.func=hashfunc_absl;
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
        
        create_buckets();

    //--------------------Create Assignment----------------------------------------------------------------------
        
        bool succ=run_create_assignement( indices_no);
        if(!succ){
            return mpc_utils::Status{mpc_utils::StatusCode::kFailedPrecondition, "Assignment could not be created."};
        }
    
    //--------------------Execute Yao's protocol and cleanup----------------------------------------------------------------------

        run_exec_yao(beta_value_vector);

    //--------------------Print results----------------------------------------------------------------------

        print_measurements( max_loop);    

    //--------------------Prepare results for c++----------------------------------------------------------------------    

        span_output=prepare_results_new();

    #ifdef DEBUG

        int other_p;
        if (mc_args.cp==1){
            other_p=2;
        }else{
            other_p=1;
        }

        for (int i = 0; i < mc_args.size; ++i){
            bool b= mc_args.mpfss_bit_output.at(i);
            osend(&pd_dbg,1,&b,sizeof(bool));
        }

        std::vector<bool> v_bit_op(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            bool b;
            orecv(&pd_dbg,2,&b,sizeof(bool));
            v_bit_op.at(i)=b;
        }

        printf("MPFSS bit results\n");
        for (int i = 0; i < mc_args.size; ++i){
            bool v=mc_args.mpfss_bit_output.at(i)^v_bit_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

        for (int i = 0; i < mc_args.size; ++i){
            osend(&pd_dbg,1,&mc_args.mpfss_output.at(i),sizeof(T));
        }

        std::vector<T> v_value_op(mc_args.size);
        for (int i = 0; i < mc_args.size; ++i){
            orecv(&pd_dbg,2,&v_value_op.at(i),sizeof(T));
        }

        printf("MPFSS results\n");
        for (int i = 0; i < mc_args.size; ++i){
            T v=mc_args.mpfss_output.at(i)^v_value_op.at(i);
            cout<<v<<" ";
        }
        printf("\n");

    #endif

    
    free(mc_args.m);
    for (int i = 0; i < mc_args.b; ++i){
        free(mc_args.matches[i]);   
    }
    free(mc_args.matches);
    for (int i = 0; i < mc_args.t; ++i){
        free(beta_value_vector[i]);
    }
    free(beta_value_vector);

    for (int i = 0; i < mc_args.b; ++i){
        yao_arguments_new *y_args=mc_args.y_args_all.at(i);
        free(y_args->dpf_bit_output);
        free(y_args->dpf_output);
        free(y_args);
    }
    return mpc_utils::OkStatus();
}
};



