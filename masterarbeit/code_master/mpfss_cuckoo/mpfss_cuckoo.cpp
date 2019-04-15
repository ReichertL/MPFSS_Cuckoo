#include <vector> 

extern "C" {
    #include "obliv.h"
    #include "includes/util.h"
    #include "includes/dbg.h"
    #include "mpfss_cuckoo.h"
}
#include "buckets.h"
#include "includes/cuckoo/cuckoo.h"
using namespace std;

double lap;
int cp;


/*  Creates management structure for MPFSS_cuckoo. Tested.
    Input 
    t: Number of points in mpfss
    size: Size of the field the points can be taken from 

*/
mpfss_cuckoo * new_mpfss_cuckoo( int t, size_t size, int w, int b, int max_loop){
  mpfss_cuckoo * mpfss =(mpfss_cuckoo * ) malloc(sizeof(mpfss_cuckoo));
  mpfss->t = t;
  mpfss->size = size;
  mpfss->w=w;
  mpfss->b=b;
  mpfss->max_loop=max_loop;

  return mpfss;

}

ProtocolDesc prepare_connection(char *argv[]){
    const char *remote_host = strtok(argv[1], ":");
        const char *port = strtok(NULL, ":");
        ProtocolDesc pd;
        
        // Make connection between two shells
        // Modified ocTestUtilTcpOrDie() function from obliv-c/test/oblivc/common/util.c
        log_info("Connecting to %s on port %s ...\n", remote_host, port);
        if(argv[2][0] == '1') {
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
        cp = (argv[2][0]=='1'? 1 : 2);
        log_info("-----Party %d-------\n", cp);
        setCurrentParty(&pd, cp); // only checks for a '1'   

        return pd;

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

        // Initialize protocols and obtain connection information
            ProtocolDesc pd=prepare_connection(argv);

        //Setting Parameters ------------------------------------------------------
            int t = atoi(argv[3]);
            int size = atoi(argv[4]);

            //following Angle et al. : PIR..
            int w= 3;   //Number of hash functions
            int b= 1.5*t; //Number of buckets --> TODO: AB oder AUF runden
            
            //Making max_loop dependent on the size of the input field
            //TODO: find good value for max_loop
            int max_loop=size;
            mpfss_cuckoo *m=new_mpfss_cuckoo(t, size, w, b, max_loop);
            int (*func)( int, int, int)=hashfunc_absl;
        
        //Preparations: Creating Indices, Buckets and finding assignment------------
            lap = wallClock();        

            int *indices_notobliv = (int *)calloc(t, sizeof(int ));
            if(cp==1){
                bool created=false;
                int count=0;
                while(!created && count<t*size*100000){ 
                    created=create_indices(indices_notobliv, t , size);
                    count++;
                }
                if(created==false){
                    printf("Error: Could not create Indices.\n" );
                    exit(1);
                }
            }

            int *bucket_lenghts=(int *) calloc(b, sizeof(int));
            match **matches = (match **) calloc(b, sizeof(match*));
            vector<vector<int>> all_buckets = preparations(m,indices_notobliv, cp, matches, bucket_lenghts,func);


            int ** all_buckets_array=(int **) calloc(b, sizeof(int *));
            for (int i = 0; i < b; ++i){
                all_buckets_array[i]=all_buckets.at(i).data();
            }

            yao_arguments *y_args= (yao_arguments *) calloc(1, sizeof(yao_arguments));
            y_args->m=m;
            y_args->bucket_lenghts=bucket_lenghts;
            y_args->matches=matches;
            y_args->all_buckets_array=all_buckets_array;

        // Execute Yao's protocol and cleanup
            execYaoProtocol(&pd, mpfss_batch_cuckoo, y_args);
            cleanupProtocol(&pd);
            
        

        // Print results and gate count
            double runtime = wallClock() - lap; // stop clock here 
            log_info("Total time: %lf seconds\n", runtime);
            //benchmark(runtime, size, t, cp, "Naive");
    
    exit(0);
}
