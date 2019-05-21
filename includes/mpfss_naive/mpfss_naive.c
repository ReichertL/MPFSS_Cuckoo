#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
#include <time.h>

#include <obliv.h>
#include "bcrandom.h"

#include "includes/util.h"
#include "includes/dbg.h"
#include "mpfss_naive.h"



void run_mpfss_naive(const char *remote_host, const char *port, int cp, int t, int size) {
  // call a function in another file

    // Check args

        ProtocolDesc pd;
        
        // Make connection between two shells
        // Modified ocTestUtilTcpOrDie() function from obliv-c/test/oblivc/common/util.c
        log_info("Connecting to %s on port %s ...\n", remote_host, port);
        if(cp==1) {
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


        log_info("-----Party %d-------\n", cp);
        setCurrentParty(&pd, cp); // only checks for a '1'        

        mpfss *m=new_mpfss_naive(t, size);
        clock_t clock_time = clock();       

         // Execute Yao's protocol and cleanup
        execYaoProtocol(&pd, mpfss_naive, m);
        cleanupProtocol(&pd);
        clock_time = clock() - clock_time;
        double runtime = ((double)clock_time)/CLOCKS_PER_SEC; // in seconds 

        // Print results and gate count
        printf("t: %d\n", t);
        printf("size: %d\n", size);
        printf("runtime: %lf\n", runtime);
      
           
}
