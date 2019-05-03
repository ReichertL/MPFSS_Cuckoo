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
    mc_args->port=port;
    mc_args->host=remote_host;
    mc_args->connection_already_exists=true;
    mc_args->print_stdout=true;
    //std::vector<int> v(1.5*t, 50);
    std::vector<int> v;
    mc_args->set_beta=-1;
    //mc_args->beta_vector=NULL;
    
    mc_args->buckets_set=false;
    run_mpfss_cuckoo(t, size, mc_args);
    free_mc_args(mc_args);
    exit(0);
 }