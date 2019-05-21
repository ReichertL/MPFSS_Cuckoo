#include <vector> 
#include <string>
#include <fstream>
#include<iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

extern "C" {
    #include "includes/util.h"
    #include "includes/dbg.h"
}
#include "code_master/mpfss_cuckoo/mpfss_cuckoo.h"
using namespace std;

int main(int argc, char *argv[]) {

  // call a function in another file

    printf("MPFSS CUCKOO\n");
    printf("=================\n\n");
    // Check args
    if (argc != 6) {

        log_info("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
                 "\tHostname usage:\n" 
                 "\tlocal . 'localhost' remote . IP address or DNS name\n", argv[0]);
        exit(1);
    }


    const char *remote_host = strtok(argv[1], ":");
    const char *port = strtok(NULL, ":");
    int cp = (argv[2][0]=='1'? 1 : 2);
    int t = atoi(argv[3]);
    int size = atoi(argv[4]);
    int interface = (argv[5][0]=='1'? 1 : 0);

    if(interface==1){
        mpfss_cuckoo_args<int> mc_args;
        mc_args.cp=cp;
        mc_args.do_benchmark=true;
        mc_args.size=size;
        mc_args.t=t;
        mc_args.pd=prepare_connection(mc_args.cp, remote_host, port);
        mc_args.port=port;
        mc_args.host=remote_host;
        mc_args.connection_already_exists=true;
        mc_args.print_stdout=true;
        mc_args.buckets_set=false;
        mc_args.cprg=true;
        MPFSS_Cuckoo<int> mc;
        mc.mc_args=mc_args;
        absl::Span<int> span_output;
        srand (time(NULL));

        if(cp==1){
            std::vector<int64_t> v;
            for (int i = 0; i < t; ++i){
                v.push_back((int64_t)(rand()%size));     
            }
            absl::Span<const int64_t> indices(v);

            std::vector< int>v_y;
            for (int i = 0; i < size; ++i){
                v_y.push_back(( int) (rand()%size)+1);
            }
            absl::Span<const int> y (v_y);
            mpc_utils::Status stat=mc.RunClientVectorOLE(y, indices, span_output);
        }else{
            int x=rand()%size;
            mpc_utils::Status stat=mc.RunServerVectorOLE(x, size, span_output);
        }
        free_mc_args_content(mc_args);
    }else{

    }  
    
    exit(0);
 }