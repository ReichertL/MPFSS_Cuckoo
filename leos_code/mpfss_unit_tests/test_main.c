#include <stdio.h>
#include <stdlib.h>
#include <obliv.h>
#include <obliv.oh>
#include <string.h>
#include <assert.h>

#include "dbg.h"
#include "util.h"


#include "mpfss_batch_codes_unittest.h"
#include "mpfss_naive_unittest.h"

void TEST_ALL(void* args){
	bool *err=(bool *) args;
	
	TEST_ALL_mpfss_naive(err);
	TEST_ALL_mpfss_batch_codes(err);
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
		int cp = (argv[2][0]=='1'? 1 : 2);
		log_info("-----Party %d-------\n", cp);
		setCurrentParty(&pd, cp); // only checks for a '1' 
		bool err=0;  
		execYaoProtocol(&pd, TEST_ALL, &err);
		cleanupProtocol(&pd);
		if(!err){
		  printf("%s\n","Success! ");
		}

		return err;
	} else {
	log_info("Usage: %s <hostname:port> <1|2> <t> <size> \n" 
			"\tHostname usage:\n" 
			"\tlocal -> 'localhost' remote -> IP address or DNS name\n", argv[0]);
	exit(1);
  }
}
