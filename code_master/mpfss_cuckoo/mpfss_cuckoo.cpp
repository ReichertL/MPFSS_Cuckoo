#include <vector> 
#include <string>
#include <fstream>
#include<iostream>

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

double lap;
int cp;



ProtocolDesc prepare_connection(int cp,const char *remote_host, const char *port ){

        ProtocolDesc pd;
        
        // Make connection between two shells
        // Modified ocTestUtilTcpOrDie() function from obliv-c/test/oblivc/common/util.c
        log_info("Connecting to %s on port %s ...\n", remote_host, port);
        if(cp == 1) {
            if(protocolAcceptTcp2P(&pd,port)!=0) {
                log_err("TCP accept from %s failed\n", remote_host);
                exit(1);
            }
        } else {
            if(protocolConnectTcp2P(&pd,remote_host,port)!=0) {
                log_err("TCP connect to %s failed\n", remote_host);
                exit(1);
            }
        }

        // Final initializations before entering protocol
        log_info("-----Party %d-------\n", cp);
        setCurrentParty(&pd, cp); // only checks for a '1'   

        return pd;

}



template <typename T>
void run_mpfss_cuckoo(int t, int size, mpfss_cuckoo_args<T> *mc_args){


        //Setting Parameters ------------------------------------------------------

            //following Angle et al. : PIR..
            int w= 3;   //Number of hash functions
            int b= 1.5*t; //Number of buckets --> TODO: AB oder AUF runden
            
            //Making max_loop dependent on the size of the input field
            //TODO: find good value for max_loop
            int max_loop=0.5*size;
            mpfss_cuckoo *m=new_mpfss_cuckoo(t, size, w, b, max_loop,mc_args->cp, (int)mc_args->do_benchmark);
            int (*func)( int, int)=hashfunc_absl;
            int memblocksize=16; //TODO

        //Preparations: Creating Indices, Buckets and finding assignment------------
            lap = wallClock();        

            log_info("Creating Indices\n");
            int *indices_notobliv = (int *)calloc(t, sizeof(int ));
            if(mc_args->cp==1){
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

                log_info("Write Indices to file\n");
                string filename="results_debug_mpfss_cuckoo_indices.txt";
                ofstream file;
                try{
                    file.open (filename);
                    for (int i = 0; i < t; ++i){
                        file << indices_notobliv[i]<<"\n";
                    }

                    file.close();
                }catch(ios_base::failure& e){
                    log_err("%s",e.what());
                }
            }

            int *bucket_lenghts=(int *) calloc(b, sizeof(int));
            match **matches = (match **) calloc(b, sizeof(match*));
            int evictions_logging=0;
            vector<vector<int>> all_buckets = preparations(m,indices_notobliv, matches, bucket_lenghts,func, &evictions_logging);


            int ** all_buckets_array=(int **) calloc(b, sizeof(int *));
            for (int i = 0; i < b; ++i){
                all_buckets_array[i]=all_buckets.at(i).data();
            }

            yao_arguments *y_args= (yao_arguments *) calloc(1, sizeof(yao_arguments));
            y_args->m=m;
            y_args->bucket_lenghts=bucket_lenghts;
            y_args->matches=matches;
            y_args->all_buckets_array=all_buckets_array;
            y_args->set_beta=mc_args->set_beta;

            int lim=memblocksize;
            if(sizeof(T)<(size_t)memblocksize){
                lim=sizeof(T);
            }
            if(mc_args->beta_vector.empty()){
                for (int i = 0; i < b; ++i){
                    uint8_t *this_beta= (uint8_t *)calloc(16, sizeof(uint8_t));
                    T beta_T=mc_args->beta_vector.at(i);
                    for (int j = 0; j < lim; ++j){
                        uint8_t x = ((uint8_t *)(&beta_T))[j];
                        this_beta[j]=x;
                    }              
                }
            }else{
                y_args->beta_vector=NULL;
            }

        // Execute Yao's protocol and cleanup
            if(mc_args->cp==1){ 
                log_info("Executing Yao Protocol\n");
            }
            execYaoProtocol(&mc_args->pd, mpfss_batch_cuckoo, y_args);
            cleanupProtocol(&mc_args->pd);
            

        // Print results
            double runtime = wallClock() - lap; // stop clock here 
            log_info("Total time: %lf seconds\n", runtime);
            if (mc_args->do_benchmark && mc_args->cp==1){
                std::vector<string> list_of_names={"runtime","t","size","no_buckets b", "no_hashfunctions w", "max_loop", "max_loop_reached", "evictions"};
                std::vector<string> list_of_values={to_string(runtime),to_string(t),to_string(size),to_string(b),to_string(w),to_string(max_loop), "no", to_string(evictions_logging) };
                benchmark_list("cuckoo", 8, list_of_names, list_of_values);
            }

        //prepare results for c++     
            std::vector<T> v;
            std::vector<bool> v_bit;
            for (int i = 0; i < size; ++i){

                uint8_t *this_value_vector=y_args->mpfss_output[i];
                T value;
                for (int i = 0; i < lim; ++i){
                    value = ((T)value << 8*i) | this_value_vector[i];
                }
                v.push_back(value);
                v_bit.push_back(y_args->mpfss_bit_output[i]);
            }
            mc_args->mpfss_output=v;
            mc_args->mpfss_bit_output=v_bit;
            mc_args->mpfss_output_raw=y_args->mpfss_output;
            mc_args->mpfss_bit_output_raw=y_args->mpfss_bit_output;

    free(m);
    free(indices_notobliv);
    free(bucket_lenghts);
    for (int i = 0; i < b; ++i)
    {
        free(matches[i]);   
    }
    free(matches);
    free(all_buckets_array);
    free(y_args);

}

int main(int argc, char *argv[]) {

  // call a function in another file

    printf("MPFSS CUCKOO\n");
    printf("=================\n\n");
    // Check args
    if (argc != 5) {

        log_info("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
                 "\tHostname usage:\n" 
                 "\tlocal -> 'localhost' remote -> IP address or DNS name\n", argv[0]);
        exit(1);
    }

    const char *remote_host = strtok(argv[1], ":");
    const char *port = strtok(NULL, ":");
    int cp = (argv[2][0]=='1'? 1 : 2);
    int t = atoi(argv[3]);
    int size = atoi(argv[4]);

    // Initialize protocols and obtain connection information
    mpfss_cuckoo_args<int> *mc_args=(mpfss_cuckoo_args<int> *)calloc(1, sizeof(mpfss_cuckoo_args<int>));
    mc_args->cp=cp;
    mc_args->do_benchmark=true;
    mc_args->pd=prepare_connection(mc_args->cp, remote_host, port);

    std::vector<int> v(1.5*t, 50);
    mc_args->set_beta=true;
    mc_args->beta_vector=v;
    run_mpfss_cuckoo(t, size, mc_args);

    exit(0);
 }