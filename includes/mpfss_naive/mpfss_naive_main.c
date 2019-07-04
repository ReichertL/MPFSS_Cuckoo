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



int main(int argc, char *argv[]) {
  // call a function in another file

    printf("MPFSS NAIVE\n");
    printf("=================\n\n");
    // Check args
    if (argc == 5) {
        // Initialize protocols and obtain connection information
        const char *remote_host = strtok(argv[1], ":");
        const char *port = strtok(NULL, ":");
        cp = (argv[2][0]=='1'? 1 : 2);
        int t = atoi(argv[3]);
        int size = atoi(argv[4]);
        bool cprg=true;
        run_mpfss_naive(remote_host, port, cp, t, size, cprg);
           
   } else {
        printf("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
                 "\tHostname usage:\n" 
                 "\tlocal -> 'localhost' remote -> IP address or DNS name\n", argv[0]);
    }
    exit(0);
}
