#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <obliv.h>
#include "bcrandom.h"

#include "includes/dbg.h"
#include "includes/util.h"

#include "mpfss_naive_unittest.h"



void TEST(void* args){
	bool *err=(bool *) args;
	TEST_ALL_mpfss_naive(err);
}





int main(int argc, char *argv[]) {

	pid_t pid;
	const char *remote_host ="localhost";
	const char *port = "99990";
    pid = fork();

	ProtocolDesc pd;
	log_info("Connecting to %s on port %s ...\n", remote_host, port);
		
	if(pid==0) {
		if(protocolAcceptTcp2P(&pd,port)!=0) {
			log_err("TCP accept from %s failed\n", remote_host);
			exit(1);
		}
	} else {
		if(protocolConnectTcp2P(&pd,remote_host,port)!=0) {
			log_err("TCP connect to %s failed\n", remote_host);
			exit(1);
		}
		//Uncomment to debug and see output from Party 2
		fclose(stdout);
	}

		// Final initializations before entering protocol
		int cp = (pid==0? 1 : 2);
		log_info("-----Party %d-------\n", cp);
		setCurrentParty(&pd, cp); // only checks for a '1' 
		bool err=0;  
		execYaoProtocol(&pd, TEST, &err);
		cleanupProtocol(&pd);
		if(!err){
		  log_info("%s\n","Success! ");
		}

		return err;
}
