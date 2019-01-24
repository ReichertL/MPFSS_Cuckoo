#include <stdio.h>
#include <stdlib.h>
#include <obliv.h>
#include <obliv.oh>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <bcrandom.h>

#include "dbg.h"
#include "util.h"
#include "binary_search_tree.h"
#include "list_utils.h"
#include "linked_list.h"


#include "mpfss_naive.oh"
#include "mpfss_batch_codes.h"
#include "mpfss_batch_codes.oh"
#include <fss_cprg.oh>

bool TEST_get_mpfss_vector_bc(int t, size_t size, double epsilon, double s){
	bool succ=false;
	bool print=true;
	int count=0;

  	mpfss_batch *mpfss=new_mpfss_batch( t, size,  epsilon,  s );
  	int t=mpfss->t;
  	int size=mpfss->size;
  	int m= mpfss->m_rounded;
	int d= mpfss->d_rounded;
	int *indices_notobliv = calloc(t, sizeof(int));
  	int *batch_len=calloc(m, sizeof(int));
	pointinfo_t **matches=calloc(t, sizeof(pointinfo_t *));
	int success =false;
	batchelement_t ***inputfield=calloc(size, sizeof(batchelement_t **));
  	BCipherRandomGen *random_gen= newBCipherRandomGen();
	
	while(count<(m*10)){
		count++;
		if(ocCurrentParty()==1){
		 	create_indices(random_gen, indices_notobliv, t , size);
			create_batches(mpfss, random_gen, batch_len,inputfield);
			success=try_combine_batches_indices(random_gen, inputfield, mpfss , indices_notobliv, matches);

			ocBroadcastInt(success, 1);
			if(success==1){

				succ=true;
				print=false;
				break;
		  	}
		}else{
			create_batches(mpfss, random_gen, batch_len,inputfield);
			int ok=3;
			while(!( ok==1 || ok==0)){
				ok= ocBroadcastInt(1, 1);
			}
			succ=ok;
			success=ok;
			print=(!ok);
	
			if(ok==1){
				break;
			}
		}
	}	
	
	obliv uint8_t *values = calloc(m, BLOCKSIZE*sizeof(obliv uint8_t)*block_no);
	obliv bool *mpfss_vector= calloc(size, sizeof( obliv bool));
	if(success==1){
		get_mpfss_vector_bc(mpfss, values, mpfss_vector,  matches, inputfield, batch_len );
	}
		

	if(count==(m*10)){
		succ=false;
		print=true;
		printf("TEST_get_mpfss_vectors_bc: Creating new indices and batches, then combining them failed %d time(s). \n", m*10);
	}

	for(int i=0; i<t; i++){
		indices_notobliv[i]= ocBroadcastInt(indices_notobliv[i], 1);
	}


	bool *vdpf= calloc(size, sizeof(bool));
	int *values_nonobliv = calloc(m, sizeof(int));
	if(success==1){

		revealOblivBoolArray(vdpf, mpfss_vector, size, 0);
		//Testing MPF 
		for(int j=0; j<size; j++){
			int val=vdpf[j];
				
			if(val==1){
				bool found=false;

				for (int i = 0; i < t; ++i){
					if(indices_notobliv[i]==j ){
						found=true;
					}
				}

				if(!found){
					succ=false;
					print=true;
					printf("TEST_get_mpfss_vectors_bc: Resulting MPF is 1 at index %d, but should not be.\n", j);
				}
			
			}else{
				bool not_set=false;
				for (int i = 0; i < t; ++i){
					if(indices_notobliv[i]==j ){
						not_set=true;
					}
				}

				if(not_set){
					succ=false;
					print=true;
					printf("TEST_get_mpfss_vectors_bc: Resulting MPF is not 1 at index %d.\n", j);
				}
			}
		}

		// Testing the values
		for(int i=0; i<t; i++){
			int v_value;
			int *v_value_ptr=&v_value;
			revealOblivInt(v_value_ptr, values[i], 0);
			
			if(v_value==0){
				printf("TEST_get_mpfss_vectors_bc: Warning! Y value for MPF was zero, but should be != zero.\n");
			}
			values_nonobliv[i]=v_value;
		}
	}

	//Print all values and vectors if there has been an error
	if(print){
		printf("indices_notobliv: ");
		for(int i=0; i<t; i++){
			int k=indices_notobliv[i];
			printf(" %d ", k);
		}
		printf("\n");

		printf("Batches\n");
	  	dllnode_t **heads=calloc(m, sizeof(dllnode_t));
	  	dllnode_t **tails=calloc(m, sizeof(dllnode_t));
	  
		for(int i=0; i<size; i++){

			batchelement_t **member_of_batches=inputfield[i];
	  		for(int j=0; j<d;j++){
	  	
				batchelement_t *batchelem=member_of_batches[j];
				int batch_no=batchelem->batch_no;
				if(heads[batch_no]==NULL){
					dllnode_t *first=create_head(batchelem, i);
					heads[batch_no]=first;
					tails[batch_no]=first;
				}else{
					dllnode_t *element=insert_dllnode( tails[batch_no], batchelem, i);		
					tails[batch_no]=element;
				}
	  		}
	  	}

  
  		for (int i = 0; i < m; ++i){
  			printf("Batch no %d : ", i);
  			dllnode_t *current= heads[i];
  			while(current!=NULL){
  				printf("%d ", current->val);
  				current=current->next;
  			}
  			printf("\n");
  		}
  		free(heads);
  		free(tails);

		printf("success: %d\n",success );


		if(success){
			if(ocCurrentParty()==1){
				printf("Matches:\n");
				for(int i=0;i<t;i++){
					pointinfo_t *p=matches[i];
					printf("batch %d, index_in_batch %d (val %d)\n",p->batch, p->index_in_batch, p->val );
				}
			}

			printf("MPFSS Vector:");
			for(int j = 0; j <size ; j++) {
			  printf("%d ", vdpf[j]);
			}
			printf(" \n");
			
			printf("Y / Active Values:\n");
			for(int i=0; i< t; i++){
				printf(" %d",  values_nonobliv[i] );
			}
			printf(" \n"); 
		}
	}

	free(vdpf);
 	free(random_gen);
  	free(indices_notobliv);
  	free(batch_len);
  	free(mpfss_vector);
  	free(matches);
  	free(values_nonobliv);

  	for (int i = 0; i < size; ++i)
  	{		
  		batchelement_t **member_of_batches= inputfield[i];
  		free(member_of_batches);
  	}
 	free(inputfield);
	return succ;
}

bool TEST_new_mpfss_batch( int t, size_t size, double epsilon, double s ){
	mpfss_batch *m=new_mpfss_batch( t, size,  epsilon,  s );
	bool succ=true;

	if(m->t !=t){
		printf("TEST_new_mpfss_batch: t value was %d, but %d expected.\n", m->t, t );
		succ=false;
	}

	if(m->size !=size){
		printf("TEST_new_mpfss_batch: size value was %d, but %d expected.\n", m->size, size );
		succ=false;
	}

	if(m->epsilon !=epsilon){
		printf("TEST_new_mpfss_batch: epsilon value was %d, but %d expected.\n", m->epsilon, epsilon );
		succ=false;
	}

	if(m->s !=s){
		printf("TEST_new_mpfss_batch: s value was %d, but %d expected.\n", m->s, s );
		succ=false;
	}

	free(m);
	return succ;
}

bool TEST_create_indices(int t, size_t size){
	bool succ=true;
	bool print=false;

	int *indices_notobliv = calloc(t, sizeof(int));
	BCipherRandomGen *random_gen= newBCipherRandomGen();
	create_indices(random_gen, indices_notobliv, t , size);

	for (int i = 0; i < t; ++i)
	{
		if(indices_notobliv[i]>size-1){
			printf("TEST_create_indices: Indices should be smaller than size value %d, but was %d.\n", size, indices_notobliv[i]);
		
			 succ=false;
			 print=true;
		}

		if(indices_notobliv[i]<0){
			printf("TEST_create_indices: Indices should not be smaller than 0, but was %d.\n", indices_notobliv[i]);
		
			 succ=false;
			 print=true;
		}
	}

	if(print){

		for (int i = 0; i < t; ++i)
		{
			printf("%d ", indices_notobliv[i] );
		}
		printf("\n");
	}

	free(indices_notobliv);
	free(random_gen);
	return succ;
}

/*bool TEST_create_batches_bst(int t, size_t size, double epsilon, double s){
	bool succ=true;
	bool print=false;

	mpfss_batch *mpfss=new_mpfss_batch( t, size,  epsilon,  s );
	int m= mpfss->m_rounded;
	int d=mpfss->d_rounded;

	dllnode_t **bt_roots = calloc(m, sizeof(dllnode_t *));
	int *indices_notobliv = calloc(t, sizeof(int));
	int *batch_len=calloc(m, sizeof(int));
	BCipherRandomGen *random_gen= newBCipherRandomGen();

	create_indices(random_gen, indices_notobliv, t , size);
	create_batches_bst(mpfss,random_gen,bt_roots, batch_len );

	//dllnode_t *batch_root=bt_roots[0];

	int N=0;
	int seen_zero=0;

	for (int i = 0; i < m; ++i)
	{

		dllnode_t *batch_root=bt_roots[i];
		if(!batch_root->is_head && i==0){
			printf("TEST_create_batches: Head for dllnode_t linked_list the boolean value is_head is not set.\n" );
			 succ=false;
			 print=true;
		}
		if(batch_root->is_head && i!=0){
			printf("TEST_create_batches: Boolean value is_head is not set, but dllnode_t  %d is not Head.\n",i );
			 succ=false;
			 print=true;
		}

		if(batch_root->next==NULL && i!=m-1){
			printf("TEST_create_batches: Next value is NULL, but dllnode_t  %d is not last in list.\n",i );
			 succ=false;
			 print=true;
		}
		if(batch_root->prev==NULL && i!=0){
			printf("TEST_create_batches: Prev value is NULL, but dllnode_t  %d is not first in list.\n",i );
			 succ=false;
			 print=true;
		}

		if(batch_root->next!=NULL && i==m-1){
			printf("TEST_create_batches: Next value is not NULL, but dllnode_t  %d is last in list.\n",i );
			 succ=false;
			 print=true;
		}
		if(batch_root->prev!=NULL && i==0){
			printf("TEST_create_batches: Prev value is not NULL, but dllnode_t  %d is first in list.\n",i );
			 succ=false;
			 print=true;
		}
		if(batch_root->val!=i){
			printf("TEST_create_batches: Val value is not equal to batch number. Was %d, but should be %d.\n",batch_root->val, i );
			 succ=false;
			 print=true;
		}


		struct bt_node *this_batch=batch_root->s;
		bool cont=true;

		if(this_batch==NULL){
			printf("TEST_create_batches: bt_node *this_batch for batch %d was NULL", i );
			 succ=false;
			 print=true;
			cont=false;
		}
		if(cont){
			if(minValueBTNode(this_batch)->key<0){
				printf("TEST_create_batches: Batch %d contained values smaller than zero.", i );
				 succ=false;
				 print=true;
			}

			batchelement_t **array=calloc(batch_len[i], sizeof(batchelement_t *));
			write_in_array(this_batch, array, 1);
			for (int j = 0; j < batch_len[i]; ++j)
			{
				if(array[j]->val==0){
					seen_zero++;
				}
			}

		}

		int acual_size=get_size(this_batch);
		N=N+acual_size;
		if (acual_size!=batch_len[i])
		{		
				printf("TEST_create_batches: Batch %d has lentgh %d but batch_len says %d.", i,acual_size,batch_len[i]   );
				 succ=false;
				 print=true;
		}

		

	}

	if(seen_zero!=d){
		printf("TEST_create_batches: The value 0 was seen %d time(s) over all batches, but should occur %d time(s).\n",seen_zero,d );
		 succ=false;
		 print=true;
	}

	if(N!=(size*d)){
		printf("TEST_create_batches: N was supposed to be %d, but %d elements were counted. \n",(size*d), N);
		 succ=false;
		 print=true;
	}

	if(print){

		for (int i = 0; i < m; ++i)
		{
			dllnode_t *batch_root=bt_roots[i];
			printf("batch no %d :",i );
			printf("batch val %d :", batch_root->val );
			dllnode_t *batch_root=bt_roots[i];
			struct bt_node *this_batch=batch_root->s;
	  		inorder(this_batch);
	  		printf("\n");

		}
	}

	free(mpfss);
	free(random_gen);
	free(indices_notobliv);
	for(int i=0; i<m;i++){
  		free(bt_roots[i]);
  	}
	free(batch_len);

	return succ;
}*/

bool TEST_create_batches(int t, size_t size, double epsilon, double s){
	bool succ=true;
	bool print=false;

	mpfss_batch *mpfss=new_mpfss_batch( t, size,  epsilon,  s );
	int m= mpfss->m_rounded;
	int d=mpfss->d_rounded;

	batchelement_t ***inputfield=calloc(size, sizeof(batchelement_t **));
	int *indices_notobliv = calloc(t, sizeof(int));
	int *batch_len=calloc(m, sizeof(int));
	BCipherRandomGen *random_gen= newBCipherRandomGen();

	create_indices(random_gen, indices_notobliv, t , size);
	create_batches(mpfss, random_gen, batch_len,inputfield);

	int N=0;

	for (int i = 0; i<size; ++i)
	{
		batchelement_t **member_of_batches= inputfield[i];
		if(member_of_batches==NULL){
			printf("TEST_create_batches:Member_of_batches was Null at position %d .\n",i );
			succ=false;
			print=true;
		}else{
			for(int j=0; j<d;j++){
				batchelement_t *batchelem=member_of_batches[j];
				int batch_no=batchelem->batch_no;
				if(batch_no>(m-1) || batch_no<0){
					printf("TEST_create_batches:Batch_no for position %d, entry %d  was %d, but this is an illegal value.\n",i,j,batch_no );
					succ=false;
					print=true;
				}
				int index_in_batch=batchelem->index_in_batch;
				if(batch_len[batch_no]<index_in_batch){
					printf("TEST_create_batches:Index_in_batch for position %d, entry %d  was %d, but the batch is only %d long.\n",i,j,batch_no, batch_len[batch_no] );
					succ=false;
					print=true;
				}
			}
		}
	}

	if(print){

		
	  	dllnode_t **heads=calloc(m, sizeof(dllnode_t));
	  	dllnode_t **tails=calloc(m, sizeof(dllnode_t));
	  
		for(int i=0; i<size; i++){

			batchelement_t **member_of_batches=inputfield[i];
	  		for(int j=0; j<d;j++){
	  	
				batchelement_t *batchelem=member_of_batches[j];
				int batch_no=batchelem->batch_no;
				if(heads[batch_no]==NULL){
					dllnode_t *first=create_head(batchelem, i);
					heads[batch_no]=first;
					tails[batch_no]=first;
				}else{
					dllnode_t *element=insert_dllnode( tails[batch_no], batchelem, i);		
					tails[batch_no]=element;
				}
	  		}
	  	}

  
  		for (int i = 0; i < m; ++i){
  			printf("Batch no %d : ", i);
  			dllnode_t *current= heads[i];
  			while(current!=NULL){
  				printf("%d ", current->val);
  				current=current->next;
  			}
  			printf("\n");
  		}
  		free(heads);
  		free(tails);
	}

	free(random_gen);
  	free(indices_notobliv);
  	free(batch_len);
  	for (int i = 0; i < size; ++i)
  	{		
  		batchelement_t **member_of_batches= inputfield[i];
  		free(member_of_batches);
  	}
 	free(inputfield);

	return succ;
}

/*bool TEST_combine_batches_indices_bst(int t, size_t size, double epsilon, double s){

	int succ=true;
	int print=false;
	mpfss_batch *mpfss=new_mpfss_batch( t, size,  epsilon,  s );
	int m= mpfss->m_rounded;
	int i=0;

	dllnode_t **bt_roots = calloc(m, sizeof(dllnode_t *));
	int *indices_notobliv = calloc(t, sizeof(int));
	BCipherRandomGen *random_gen= newBCipherRandomGen();
	int *batch_len=calloc(m, sizeof(int));
	pointinfo_t **matches=calloc(t, sizeof(pointinfo_t *));

	int matched=false;
	int *batch_no_test=calloc(t, sizeof(int));

	while(i<(m*10)){
		i++;

		
		create_indices(random_gen, indices_notobliv, t , size);

		create_batches_bst(mpfss,random_gen,bt_roots, batch_len );
		matched=try_combine_batches_indices_bst(random_gen,bt_roots,  mpfss , indices_notobliv, matches);


		if(matched){
			
			for (int i = 0; i < t; ++i)
			{
				pointinfo_t *p=matches[i];

					int val=p->val;
					int batch=p->batch;
					int index_in_batch=p->index_in_batch;
					bool found;
					for (int j = 0; j < t; ++j)
					{
						if(val==indices_notobliv[j]){
							found=true;
						}
					}
					if(!found){
						printf("TEST_combine_batches_indices_bst: Value of match was %d, but this indice was not selected.\n",val);
						succ=false;
						print=true;	
					}
					
					if(batch>=m){
						printf("TEST_combine_batches_indices_bst: Batch no was %d, but max number is %d. \n",batch, m-1);
						succ=false;
						print=true;	
					}
					for (int j = 0; j < i; ++j)
					{
						if(batch==batch_no_test[j]){
							printf("TEST_combine_batches_indices_bst: Batch no was %d, but this batch is already taken by another indice. \n",batch);
							succ=false;
							print=true;	
						}
					}
					
					dllnode_t *batch_root=bt_roots[batch];
					struct bt_node *this_batch=batch_root->s;
					batchelement_t **array=calloc(batch_len[i], sizeof(batchelement_t *));
	  				write_in_array(this_batch, array, 1);
					if(array[index_in_batch]->val!=val){
						printf("TEST_combine_batches_indices_bst: Match for val %d (batch no %d, index_in_batch %d) did not correspond with real value %d at this position. \n",val, batch, index_in_batch, array[index_in_batch]->val );
						succ=false;
						print=true;	
					}
			}

			break;
		}
		
	}

	if(i==(m*10)){
		printf("TEST_combine_batches_indices_bst: Matching indices and batches failed %d times. Possibly broken.\n",m*10);
		succ=false;
		print=true;
	}	

	if(print){
		printf("Indices of last run \n");
		for (int i = 0; i < t; ++i)
		{
			printf("%d ", indices_notobliv[i] );
		}
		printf("\n");


		printf("Batches of last run\n");
		for (int i = 0; i < m; ++i)
		{
			dllnode_t *batch_root=bt_roots[i];
			printf("batch no %d ",i );
			printf("batch val %d \n", batch_root->val );
			dllnode_t *batch_root=bt_roots[i];
			struct bt_node *this_batch=batch_root->s;
		  	inorder(this_batch);
		  	printf("\n");
		}


		if(matched){

			printf("Matches\n");
			for (int i = 0; i < t; ++i)
			{
				pointinfo_t *p=matches[i];

				int val=p->val;
				int batch=p->batch;
				int index_in_batch=p->index_in_batch;

				printf("Match %d (batch %d, index_in_batch %d, val %d)\n", i, batch, index_in_batch, val );
			}
		}
	}

	free(mpfss);
	free(random_gen);
	for(int i=0; i<m;i++){
  		free(bt_roots[i]);
  	}
	free(indices_notobliv);
	free(batch_len);
	free(matches);

	return succ;
}*/

bool TEST_combine_batches_indices(int t, size_t size, double epsilon, double s){

	int succ=true;
	int print=false;
	mpfss_batch *mpfss=new_mpfss_batch( t, size,  epsilon,  s );
	int m= mpfss->m_rounded;
	int d= mpfss->d_rounded;
	int i=0;

	batchelement_t ***inputfield=calloc(size, sizeof(batchelement_t **));
	int *indices_notobliv = calloc(t, sizeof(int));
	BCipherRandomGen *random_gen= newBCipherRandomGen();
	int *batch_len=calloc(m, sizeof(int));
	pointinfo_t **matches=calloc(t, sizeof(pointinfo_t *));

	int matched=false;
	int *batch_no_test=calloc(t, sizeof(int));
	int success;

	while(i<(m*10)){
		i++;

	   	create_indices(random_gen, indices_notobliv, t , size);
		create_batches(mpfss, random_gen, batch_len,inputfield);
		success=try_combine_batches_indices(random_gen, inputfield, mpfss , indices_notobliv, matches);;


		if(matched){
			
			for (int i = 0; i < t; ++i)
			{
				pointinfo_t *p=matches[i];

					int val=p->val;
					int batch=p->batch;
					int index_in_batch=p->index_in_batch;
					bool found;
					for (int j = 0; j < t; ++j)
					{
						if(val==indices_notobliv[j]){
							found=true;
						}
					}
					if(!found){
						printf("TEST_combine_batches_indices: Value of match was %d, but this indice was not selected.\n",val);
						succ=false;
						print=true;	
					}
					
					if(batch>=m){
						printf("TEST_combine_batches_indices: Batch no was %d, but max number is %d. \n",batch, m-1);
						succ=false;
						print=true;	
					}
					for (int j = 0; j < i; ++j)
					{
						if(batch==batch_no_test[j]){
							printf("TEST_combine_batches_indices: Batch no was %d, but this batch is already taken by another indice. \n",batch);
							succ=false;
							print=true;	
						}
					}
					
		
					batchelement_t *found_elem=NULL;
					batchelement_t **member_of_batches= inputfield[val];
					for(int k=0;k<d;k++){
						batchelement_t *batchelem=member_of_batches[k];
						if(batchelem->batch_no==batch){
							found_elem=batchelem;
						}

					}
					if(found_elem==NULL){

						printf("TEST_combine_batches_indices: Match for val %d (batch no %d, index_in_batch %d) is illegal because the value is not in this batch. \n",val, batch, index_in_batch);
						succ=false;
						print=true;	
					}
					if(found_elem!=NULL && found_elem->index_in_batch!=index_in_batch){
						printf("TEST_combine_batches_indices: Match for val %d (batch no %d, index_in_batch %d) is illegal because actual index_in_batch is %d. \n",val, batch, index_in_batch, found_elem->index_in_batch);
						succ=false;
						print=true;	
					}
			
			}

			break;
		}
		
	}

	if(i==(m*10)){
		printf("TEST_combine_batches_indices: Matching indices and batches failed %d times. Possibly broken.\n",m*10);
		succ=false;
		print=true;
	}	

	if(print){
		printf("Indices of last run \n");
		for (int i = 0; i < t; ++i)
		{
			printf("%d ", indices_notobliv[i] );
		}
		printf("\n");


		printf("Batches of last run\n");
	  	dllnode_t **heads=calloc(m, sizeof(dllnode_t));
	  	dllnode_t **tails=calloc(m, sizeof(dllnode_t));
	  
		for(int i=0; i<size; i++){

			batchelement_t **member_of_batches=inputfield[i];
	  		for(int j=0; j<d;j++){
	  	
				batchelement_t *batchelem=member_of_batches[j];
				int batch_no=batchelem->batch_no;
				if(heads[batch_no]==NULL){
					dllnode_t *first=create_head(batchelem, i);
					heads[batch_no]=first;
					tails[batch_no]=first;
				}else{
					dllnode_t *element=insert_dllnode( tails[batch_no], batchelem, i);		
					tails[batch_no]=element;
				}
	  		}
	  	}

  
  		for (int i = 0; i < m; ++i){
  			printf("Batch no %d : ", i);
  			dllnode_t *current= heads[i];
  			while(current!=NULL){
  				printf("%d ", current->val);
  				current=current->next;
  			}
  			printf("\n");
  		}
  		free(heads);
  		free(tails);

		if(matched){

			printf("Matches\n");
			for (int i = 0; i < t; ++i)
			{
				pointinfo_t *p=matches[i];

				int val=p->val;
				int batch=p->batch;
				int index_in_batch=p->index_in_batch;

				printf("Match %d (batch %d, index_in_batch %d, val %d)\n", i, batch, index_in_batch, val );
			}
		}
	}

	free(random_gen);
  	free(indices_notobliv);
  	free(batch_len);
  	for (int i = 0; i < size; ++i)
  	{		
  		batchelement_t **member_of_batches= inputfield[i];
  		free(member_of_batches);
  	}
 	free(inputfield);

	return succ;
}

void TEST_ALL_mpfss_batch_codes(bool *err){
	printf("\n\nTesting MPFSS BATCH CODES---------------------------------------------------\n");

	printf("TEST_new_mpfss_batch---------------------------------------------------------\n" );
	if(!TEST_new_mpfss_batch( 6, (size_t) 10, 0.1, 4)){
		printf("%s\n", "TEST_new_mpfss_batch( 6, (size_t) 10, 0.1, 4) failed" );
		*err=1;
	}

	printf("TEST_create_indices---------------------------------------------------------\n" );
	if(!TEST_create_indices(4, 10)){
		printf("%s\n", "TEST_create_indices(4, 10)" );
		*err=1;
	}

	if(!TEST_create_indices(10, 5)){
		printf("%s\n", "TEST_create_indices(10, 5)" );
		*err=1;
	}


	printf("TEST_create_batches---------------------------------------------------------\n" );
	if(!TEST_create_batches(2, (size_t) 10, 0.1 , 4 )){
		printf("%s\n", "TEST_create_batches(4, (size_t) 10, 0.1 , 4 )failed" );
		*err=1;
	}

	printf("TEST_combine_batches_indices---------------------------------------------------------\n" );
	if(!TEST_combine_batches_indices(2, (size_t) 10, 0.1 , 4 )){
		printf("%s\n", "TEST_combine_batches_indices(4, (size_t) 10, 0.1 , 4 )failed" );
		*err=1;
	}



	printf("TEST_get_mpfss_vector_bc---------------------------------------------------------\n" );
	if(!TEST_get_mpfss_vector_bc(2, (size_t) 10, 0.1 , 4 )){
		printf("%s\n", "TEST_get_mpfss_vector_bc(4, (size_t) 10, 0.1 , 4 )failed" );
		*err=1;
	}
}



