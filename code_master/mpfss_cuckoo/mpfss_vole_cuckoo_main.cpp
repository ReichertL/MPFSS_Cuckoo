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
    mc_args.cprg=true;
    MPFSS_Cuckoo<int> mc;
    mc.mc_args=mc_args;
    absl::Span<int> span_output;
    srand (time(NULL));

    if(cp==1){
        printf("indices: ");
        std::vector<int64_t> v;
        for (int i = 0; i < t; ++i){
            int r=rand()%size;
            while ((std::find(v.begin(), v.end(), r) != v.end())or r==0){
                r=rand()%size;
            }
            v.push_back((int64_t)(r));     
            printf("%d ", r);
        }
        printf("\n");

        absl::Span<const int64_t> indices(v);

        printf("beta: ");
        std::vector< int>v_y;
        for (int i = 0; i < t; ++i){
            int r=( int) (rand()%255);
            while(r==0){
                r=( int) (rand()%255);
            }
            v_y.push_back(r);
            printf("%d ",r );
        }
        printf("\n");
        absl::Span<const int> y (v_y);
        mpc_utils::Status stat=mc.RunIndexProviderVectorOLE(y, indices, span_output);
    }else{
        int x=10;//rand()%size;
        mpc_utils::Status stat=mc.RunValueProviderVectorOLE(x, size, span_output);
    }

    free_mc_args_content(mc.mc_args);
    exit(0);
 }