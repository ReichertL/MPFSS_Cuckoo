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
    #include "includes/create_structs/create_structs.h"
}
#include "includes/benchmark.h"
#include "code_master/mpfss_cuckoo/buckets.h"
#include "includes/cuckoo/cuckoo.h"
#include "mpfss_new_cuckoo.h"
#include "create_structs_new.h"

using namespace std;



ProtocolDesc prepare_connection(int cp,const char *remote_host, const char *port ){

        ProtocolDesc pd;
        
        // Make connection between two shells
        // Modified ocTestUtilTcpOrDie() function from obliv-c/test/oblivc/common/util.c
        if(cp == 1) {
            log_info("Waiting for connection from %s on port %s ...\n", remote_host, port);
            if(protocolAcceptTcp2P(&pd,port)!=0) {
                log_err("TCP accept from %s:%s failed\n", remote_host,port);
                exit(1);
            }
        } else {
            log_info("Connecting to %s on port %s ...\n", remote_host, port);
            if(protocolConnectTcp2P(&pd,remote_host,port)!=0) {
                log_err("TCP connect to %s:%s failed\n", remote_host, port);
                exit(1);
            }
        }

        // Final initializations before entering protocol

        setCurrentParty(&pd, cp); // only checks for a '1'   

        return pd;

}


void free_mc_args_content_new( mpfss_cuckoo_args_new<int> mc_args){

    for (int i = 0; i < (int)mc_args.rands.size(); ++i){
        free(mc_args.all_buckets_array[i]);
    }
    free(mc_args.all_buckets_array);
    free(mc_args.bucket_lenghts);

}


