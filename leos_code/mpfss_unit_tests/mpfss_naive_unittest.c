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
#include "mpfss_naive.oh"
//#include "mpfss_naive.oc"
#include <fss_cprg.oh>

bool TEST_get_mpfss_vector(int t, size_t size){
	mpfss *m1=new_mpfss_naive(t, size);
	int *indices_notobliv1 = calloc(t, sizeof(int));
	for(int i=0; i<t; i++){
		indices_notobliv1[i]=i;
	}
		//feed indices one by one because target type is not int but size_t
	obliv size_t *indices1 = calloc(t, BLOCKSIZE*sizeof(obliv size_t)*block_no);
	for(int i=0; i<t; i++){
		obliv size_t k_obliv= feedOblivInt(indices_notobliv1[i], 1);
		indices1[i]=k_obliv;
	}

	obliv bool **vector= calloc(size, sizeof(obliv bool));
	obliv uint8_t *values1 = calloc(t, BLOCKSIZE*sizeof(obliv uint8_t)*block_no);
	get_mpfss_vector(m1, indices1, vector, values1);

	bool succ=true;
	bool print=false;
	
	//Testing the vector
	bool *vdpf= calloc(size, sizeof(bool));
	revealOblivBoolArray(vdpf, vector, size, 0);
	for(int i=0; i<size; i++){
		if(vdpf[i]==1){
			bool found=false;
			for(int j=0; j<t;j++){
				if(indices_notobliv1[j]==i){
					found=true;
				}
			}
			if(found==false){
				succ=false;
				print=true;
				printf("TEST_get_mpfss_vector: Resulting MPF is not 1 at index %d, but should be.\n", i);
			}
		}else{
			bool found=false;
			for(int j=0; j<t;j++){
				if(indices_notobliv1[j]==i){
					found=true;
				}
			}
			if(found==true){
				succ=false;
				print=true;
				printf("TEST_get_mpfss_vector: Resulting MPF is 1 at index %d, but should not be.\n", i);
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
			print=true;
			printf("TEST_get_mpfss_vector: Warning! Value at one point of MPF was zero, but should be != zero.\n");
		}
	}

	//Print all values and vector if there has been an error
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

		bool *vdpf= calloc(size, sizeof(bool));
		revealOblivBoolArray(vdpf, vector, size, 0);
		printf("MPF: ");  
		for(int j = 0; j <size ; j++) {
		  printf("%d ", vdpf[j]);  
		}
		free(vdpf); 

		for(int i=0; i< t; i++){     

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
	free(vector);
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
	obliv uint8_t *values = calloc(1, BLOCKSIZE*sizeof(obliv uint8_t)*block_no);
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


void TEST_ALL_mpfss_naive(bool *err){

	printf("\n\nTesting MPFSS Naive-----------------------------------------------.----\n");


	printf("TEST_dpf---------------------------------------------------------\n" );
	if(!TEST_dpf(10, 3 )){
		printf("%s\n", "TEST_dpf(10,3) failed" );
		*err=1;
	}
	if(!TEST_dpf(10, 11 )){
		printf("%s\n", "TEST_dpf(10,11) failed" );
		*err=1;
	}

	printf("TEST_new_mpfss_naive---------------------------------------------------------\n" );
	if(!TEST_new_mpfss_naive( 6, (size_t) 10)){
		printf("%s\n", "TEST_new_mpfss_naive( 6, (size_t) 10 ) failed" );
		*err=1;
	}

	printf("TEST_get_mpfss_vector---------------------------------------------------------\n" );
	if(!TEST_get_mpfss_vector( 4, (size_t) 10)){
		printf("%s\n", "TEST_get_mpfss_vector( 4, (size_t) 10) failed" );
		*err=1;
	}
}


