typedef struct node {

	//index of batch
	int index_in_batch;
    int right_node;
    int left_node;
    bool is_head;
    struct node * next;
} node_t;


typedef struct batchelement {

	//index of batch
	int index_in_batch;
    int batch_no;
    int val;

} batchelement_t;


typedef struct batch {

	int index;
	bool is_head;
	node_t *list;
    struct batch * prev;
    struct batch * next;
} batch_t;

typedef struct pointinfo {

	int val;
	int batch;
	int index_in_batch;
} pointinfo_t;



/*	Deletes the given batch_t node from a list of batch_t nodes.
	If deleted element was last element in list, previous will be returned. 
	If there is no previous element, null will be returned.
*/
void del_batch(batch_t *to_delete){


	if(to_delete->next!=NULL && to_delete->prev!=NULL){
		#ifdef DEBUG
			//printf("Deleting middle node\n"); 
		#endif
		//normal middle node
		batch_t *prev= to_delete->prev;
		batch_t *next=to_delete->next;
		prev->next=next;
		next->prev=prev;

	}else if(to_delete->prev==NULL && to_delete->next!=NULL){
		//unlikely to happen
		#ifdef DEBUG
			//printf("Deleting first/head node\n"); 
		#endif
		//head node
		batch_t *next=to_delete->next;
		next->prev=NULL;
		next->is_head=1;

	}else if(to_delete->prev!=NULL &&to_delete->next==NULL) {
		#ifdef DEBUG
			//printf("Deleting last node\n"); 
		#endif
		//last node
		batch_t *prev=to_delete->prev;
		prev->next=NULL;

	}

	free(to_delete);
	
}







void print_batch_list(batch_t *head){

	batch_t *current=head->next;

	bool tailfound=false;
	while(!tailfound){
		
		printf("%d ", current->index );
		if(current->next!=NULL){
			current=current->next;

		}else{
			tailfound=true;
		}

	}
	printf("\n");
}
