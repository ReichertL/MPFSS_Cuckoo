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

bool TEST_get_mpfss_vectors(int t, size_t size){
	mpfss *m1=new_mpfss_naive(t, size);
	int *indices_notobliv1 = calloc(t, sizeof(int));
	for(int i=0; i<t; i++){
		indices_notobliv1[i]=i;
	}
		//feed indices one by one because target type is not int but size_t
	obliv size_t *indices1 = calloc(t, BLOCKSIZE*sizeof(obliv size_t)*blockmultiple);
	for(int i=0; i<t; i++){
		obliv size_t k_obliv= feedOblivInt(indices_notobliv1[i], 1);
		indices1[i]=k_obliv;
	}

	obliv bool **vectors1= calloc(t, sizeof(int *));
	obliv uint8_t *values1 = calloc(t, BLOCKSIZE*sizeof(obliv uint8_t)*blockmultiple);
	get_mpfss_vectors(m1, indices1, vectors1, values1);

	bool succ=true;
	bool print=false;
	//Testing the vectors
	for(int i=0; i<t; i++){
		bool *vdpf= calloc(size, sizeof(bool));
		revealOblivBoolArray(vdpf, vectors1[i], size, 0);

		for(int j=0; j<size; j++){
			int val=vdpf[j];
			if(j==indices_notobliv1[i] && val!=1){
				succ=false;
				print=true;
				printf("TEST_get_mpfss_vectors: Resulting Dpf %d is not 1 at index %d.\n", i, j);

			}
			if(j!=indices_notobliv1[i] && val==1){
				succ=false;
				print=true;
				printf("TEST_get_mpfss_vectors: Resulting Dpf %d is 1 at index %d.\n", i, j);
			}
		}
		free(vdpf);
	}

	// Testing the values
	for(int i=0; i<t; i++){
		int v_value;
		int *v_value_ptr=&v_value;
		revealOblivInt(v_value_ptr, values1[i], 0);
		if(v_value==0){
			succ=false;
			print=true;
			printf("TEST_get_mpfss_vectors: Value for DPF %d was zero, but should be != zero.", i);
		}
	}

	//Print all values and vectors if there has been an error
	if(print){
		printf("indices (expected)	: ");
		for(int i=0; i<t; i++){
			int k=indices_notobliv1[i];
			printf(" %d ", k);
		}
		printf("\n");

	//Do not use revealOblivIntArray() when obliv type is size_t
		int *indices_revealed = calloc(t, sizeof(int));
		printf("indices (revealed)	: ");
		for(int i=0; i<t; i++){
			int k;
			revealOblivInt(&k, indices1[i],0);
			printf(" %d ", k);
		}
		printf("\n");
		free(indices_revealed);

		for(int i=0; i< t; i++){
			bool *vdpf= calloc(size, sizeof(bool));
			revealOblivBoolArray(vdpf, vectors1[i], size, 0);
			printf("dpf #%d: ", i);  
			for(int j = 0; j <size ; j++) {
			  printf("%d ", vdpf[j]);  
			}
			free(vdpf);      

			int v_value;
			int *v_value_ptr=&v_value;
			revealOblivInt(v_value_ptr, values1[i], 0);
			printf("	Value: %d",  v_value );
			printf(" \n"); 
		}
		
	}

	free(m1);
	free(indices_notobliv1);
	free(indices1);
	free(vectors1);
	free(values1);

	return succ;
}

bool TEST_new_mpfss_naive( int t, size_t size){
	mpfss *m=new_mpfss_naive(t, size);
	bool succ=true;

	if(m->t !=t){
		printf("TEST_new_mpfss_naive: t value was %d, but %d expected.\n", m->t, t );
		succ=false;
	}

	if(m->size !=size){
		printf("TEST_new_mpfss_naive: size value was %d, but %d expected.\n", m->size, size );
		succ=false;
	}

	free(m);
	return succ;
}

bool TEST_dpf(size_t size, int index){
	obliv bool *vector= calloc(1, sizeof(int));
	obliv uint8_t *values = calloc(1, BLOCKSIZE*sizeof(obliv uint8_t)*blockmultiple);
	obliv size_t index_obliv;
	index_obliv=feedOblivInt(index, 1);

	dpf(size,index_obliv, values, 0,&vector);
	int activ_value;
	int *activ_value_ptr=&activ_value;
	revealOblivInt(activ_value_ptr, values[0], 0);
	bool *dpf= calloc(size, sizeof(bool));
	revealOblivBoolArray(dpf, vector, size, 0);
	
	bool succ=true;
	bool print=false;

	if(activ_value==0 && index<size){
		printf("TEST_dpf: activ_value has wrong value after reveal. Was 0, but should be greater than 0. \n");
		succ=false;
	}
	if(activ_value!=0 && index>size){
		printf("TEST_dpf: activ_value has wrong value after reveal. Was %d, but should be 0. \n", activ_value);
		succ=false;
	}



	for(int i=0; i<size; i++){
		if(i==index && dpf[i]!=1 ){
			printf("TEST_dpf: Resulting Dpf is not 1 at index %d.\n", index);
				print=true;
				succ=false;

		}else if(dpf[i]!=0 && i!=index){
			printf("TEST_dpf: Resulting Dpf is not 0 at %d.\n", i);
			print=true;
			succ=false;
			break;
		}
	}

	if(print){
	printf("TEST_dpf: dpf\n");  
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


void TEST_ALL(void* args){
	bool *err=(bool *) args;

	printf("TEST_dpf---------------------------------------------------------\n" );
	if(!TEST_dpf(10, 3 )){
		printf("%s\n", "TEST_dpf(10,1, 0) failed" );
		*err=1;
	}
	if(!TEST_dpf(10, 11 )){
		printf("%s\n", "TEST_dpf(10,1, 0) failed" );
		*err=1;
	}

	printf("TEST_new_mpfss_naive---------------------------------------------------------\n" );
	if(!TEST_new_mpfss_naive( 6, (size_t) 10)){
		printf("%s\n", "TEST_new_mpfss_naive( 6, (size_t) 10 ) failed" );
		*err=1;
	}

	printf("TEST_get_mpfss_vectors---------------------------------------------------------\n" );
	if(!TEST_get_mpfss_vectors( 4, (size_t) 10)){
		printf("%s\n", "TEST_get_mpfss_vectors( 4, (size_t) 10) failed" );
		*err=1;
	}
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