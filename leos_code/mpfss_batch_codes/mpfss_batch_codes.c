#include <stdio.h>
#include <stdlib.h>
#include <obliv.h>
#include <obliv.oh>
#include "dbg.h"
#include <string.h>
#include <math.h>
#include <assert.h>
#include "util.h"
#include "mpfss_batch_codes.h"
#include "mpfss_batch_codes.oc"
#include <fss_cprg.oh>



int main(int argc, char *argv[]) {
  // call a function in another file

    printf("MPFSS NAIVE\n");
    printf("=================\n\n");
    // Check args
    if (argc == 5) {
        // Initialize protocols and obtain connection information
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
        int t = atoi(argv[3]);
        int size = atoi(argv[4]);

        mpfss *m=new_mpfss_naive(t, size);
        lap = wallClock();        

         // Execute Yao's protocol and cleanup
        execYaoProtocol(&pd, mpfss_batch_codes, m);
        cleanupProtocol(&pd);
        double runtime = wallClock() - lap; // stop clock here 

        // Print results and gate count
        log_info("Total time: %lf seconds\n", runtime);
        printf("\n");
        benchmark(runtime, size, t, cp, "Batch_Codes");


        
   } else {
        log_info("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
                 "\tHostname usage:\n" 
                 "\tlocal -> 'localhost' remote -> IP address or DNS name\n", argv[0]);
    }
    //return 0;
    exit(0);
}
