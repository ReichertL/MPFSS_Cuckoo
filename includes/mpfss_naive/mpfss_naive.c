#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include <obliv.h>
#include "bcrandom.h"

#include "includes/util.h"
#include "includes/dbg.h"
#include "mpfss_naive.h"

int memblocksize=16;

void run_mpfss_naive(const char *remote_host, const char *port, int cp, int t, int size, bool cprg, int threads) {

        ProtocolDesc pd;
        
        if(cp==1) {
            log_info("Waiting for connection from %s on port %s ...\n", remote_host, port);
            if(protocolAcceptTcp2P(&pd,port)!=0) {
                log_err("TCP accept from %s failed\n", remote_host);
                exit(1);
            }
        } else {
            log_info("Connecting to %s on port %s ...\n", remote_host, port);
            if(protocolConnectTcp2P(&pd,remote_host,port)!=0) {
                log_err("TCP connect to %s failed\n", remote_host);
                exit(1);
            }
        }

        log_info("-----Party %d-------\n", cp);
        setCurrentParty(&pd, cp);      
        
        mpfss *m=new_mpfss_naive(t, size);
        m->cprg_set=cprg;
        m->threads=threads;

        uint8_t **beta_share=calloc(t, sizeof(uint8_t *));
        BCipherRandomGen *random_gen= newBCipherRandomGen();    
       
        for (int i = 0; i < t; ++i){
            uint8_t *beta_value=calloc(memblocksize, sizeof(uint8_t));
            int rand= bcRandomInt(random_gen, (unsigned long long) size);
            beta_value[memblocksize-1]=(uint8_t) rand;
            beta_share[i]=beta_value;
        }

        #ifdef DEBUG
        debug("beta share\n");
        for (int i = 0; i < t; ++i){
            for (int j = 0; j<memblocksize; ++j){
                printf("%d ", beta_share[i][j]);
            }
            printf("\n");
        }
        #endif

        releaseBCipherRandomGen(random_gen);
        m->beta_share=beta_share;
        
        int *indices_notobliv = calloc(t, sizeof(int ));

        if(cp==1){
            int ret= create_indices( indices_notobliv, t , size);

        }
        m->indices_notobliv=indices_notobliv;

        clock_t clock_time = clock();       
    

         // Execute Yao's protocol and cleanup
        execYaoProtocol(&pd, mpfss_naive, m);
        clock_t clock_time1 = clock() - clock_time;
        cleanupProtocol(&pd);
        clock_t  clock_time2 = clock() - clock_time;
        double runtime = ((double)clock_time1)/CLOCKS_PER_SEC; // in seconds 
        double runtime2 = ((double)clock_time2)/CLOCKS_PER_SEC; // in seconds 

        // Print results and gate count
        printf("t: %d\n", t);
        printf("size: %d\n", size);
        printf("runtime: %lf\n", runtime);
        printf("runtime2: %lf\n", runtime2);
        
        free(indices_notobliv);
        free(m->mpfss_bit_vector);
        for (int i = 0; i < size; ++i){
            free(m->mpfss_value_vector[i]);
        }
        free(m->mpfss_value_vector);
        free(m);
        for (int i = 0; i < t; ++i){
            free(beta_share[i]);
        }
        free(beta_share);
      
           
}
