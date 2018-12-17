#include <stdio.h>
#include <stdlib.h>
#include <obliv.h>
#include <obliv.oh>
#include "dbg.h"
#include <string.h>
#include <math.h>
#include <assert.h>
#include "util.h"
#include "mpfss_naive.h"
#include "mpfss_naive.oc"

#include <fss_cprg.oh>


bool test_get_mpfss_vectors(int t ){
	int *indices_notobliv = calloc(t, sizeof(int));
	obliv size_t *indices = calloc(t, BLOCKSIZE*sizeof(obliv size_t)*blockmultiple);
	feedOblivIntArray(indices, indices_notobliv, t, 1);
	 for(int i=0; i<t; i++){
    	indices_notobliv[i]=i;
  	}

  	return false;
}


bool dpf_test(size_t size, int index, int party ){
	log_info("Running DPF_Test");
  	obliv bool *vector= calloc(1, sizeof(int));
  	obliv uint8_t *values = calloc(1, BLOCKSIZE*sizeof(obliv uint8_t)*blockmultiple);
  	obliv size_t index_obliv;
  	log_info("Running DPF_Test 2");

  	index_obliv=feedOblivInt(index, 1);
  	log_info("Running DPF_Test 3");

	dpf(size,index_obliv,party, values, 0,&vector);
	int activ_value;
    int *activ_value_ptr=&activ_value;
  	revealOblivInt(activ_value_ptr, values[0], 0);
  	bool *dpf= calloc(size, sizeof(bool));
	revealOblivBoolArray(dpf, vector, size, 0);
	
	bool succ=true;
	bool print=false;
	if(activ_value!=10){
		printf("dpf_test: activ_value has wrong value after reveal.\n");
		printf("dpf_test: activ_value %d\n", activ_value );
		succ=false;
	}

	for(int i=0; i<size; i++){
		if(i==index && dpf[i]!=1 ){
			printf("dpf_test: Resulting Dpf is not 1 at index %d.\n", index);
				print=true;
				succ=false;

		}else if(dpf[i]!=0 && i!=index){
			printf("dpf_test: Resulting Dpf is not 0 at %d.\n", i);
			print=true;
			succ=false;
			break;
		}
	}

	if(print){
      	printf("dpf_test: dpf\n");  
        for(int i = 0; i <size ; i++) {
          printf("%d ", dpf[i]);  
        }
        printf(" \n");
	}

	free(vector);
	free(values);
	free(dpf);
	return succ;

}


void TESTING_mpfss_naive(void* args){
	bool *err=(bool *) args;
  	if(!dpf_test(10, 3, cp )){
        printf("%s\n", "dpf_test(10,1, 0, %d ) failed", cp );
        *err=1;
    }

    if(!dpf_test(10, 11, cp )){
        printf("%s\n", "dpf_test(10,1, 0, %d ) failed", cp );
        *err=1;
    }

    *err=0;

  }


int main(int argc, char *argv[]) {
    if (argc == 3) {
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
        bool err=1;  
     	execYaoProtocol(&pd, TESTING_mpfss_naive, &err);
        cleanupProtocol(&pd);
        return err;

      
	} else {
        log_info("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
                 "\tHostname usage:\n" 
                 "\tlocal -> 'localhost' remote -> IP address or DNS name\n", argv[0]);
    	exit(1);
    }
}