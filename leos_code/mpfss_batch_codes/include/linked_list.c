#include "linked_list.h"

dllnode_t *insert_dllnode( struct dllnode * prev, struct arbitray_struct *s, int val){
	dllnode_t *current=calloc(1,sizeof(dllnode_t));
	current->val=val;
	current->s=s;
	current->prev=prev;
	prev->next=current;
	current->next=NULL;
	return current;

}
void set_head( struct arbitray_struct *s, int val, struct dllnode * list_head){
	list_head->is_head=1;
	list_head->prev=NULL;
	list_head->next=NULL;
	list_head->s=s;
	list_head->val=val;
}

dllnode_t *create_head( struct arbitray_struct *s, int val){
	dllnode_t *list_head=calloc(1,sizeof(dllnode_t));
	list_head->is_head=1;
	list_head->prev=NULL;
	list_head->next=NULL;
	list_head->s=s;
	list_head->val=val;
	return list_head;
}


void set_values(struct arbitray_struct *s, int val, struct dllnode * element,  struct dllnode * prev){
	element->prev=prev;
	element->next=NULL;
	element->s=s;
	element->val=val;
	
}


/*	Deletes given dllnode_t form list and returns next element in list. 
	If deleted element was last element in list, previous will be returned. 
	If there is no previous element, null will be returned.
*/
dllnode_t *del_dllnode(dllnode_t *to_delete){


	dllnode_t *for_free=to_delete;
	dllnode_t *new_dllcurrent;
	if(to_delete->next!=NULL && to_delete->prev!=NULL){
		#ifdef DEBUG
			//printf("Deleting middle node\n"); 
		#endif
		//normal middle node
		dllnode_t *prev= to_delete->prev;
		dllnode_t *next=to_delete->next;
		prev->next=next;
		next->prev=prev;
		new_dllcurrent=next;

	}else if(to_delete->prev==NULL && to_delete->next!=NULL){
		#ifdef DEBUG
			//printf("Deleting first/head node\n"); 
		#endif
		//head node
		dllnode_t *next=to_delete->next;
		next->prev=NULL;
		next->is_head=1;
		new_dllcurrent=next;

	}else if(to_delete->prev!=NULL &&to_delete->next==NULL) {
		#ifdef DEBUG
			//printf("Deleting last node\n"); 
		#endif
		//last node
		dllnode_t *prev=to_delete->prev;
		prev->next=NULL;
		new_dllcurrent=to_delete->prev;

	}else{
		#ifdef DEBUG
			//printf("Deleting node that is first and last node\n"); 
		#endif
		new_dllcurrent=NULL;

	}

	free(for_free);
	return new_dllcurrent;
	
}

/*		
	Like del_dllnode but does not free the memory.
*/
dllnode_t *delink_dllnode(dllnode_t *to_delete){


	dllnode_t *new_dllcurrent;
	if(to_delete->next!=NULL && to_delete->prev!=NULL){
		#ifdef DEBUG
			//printf("Deleting middle node\n"); 
		#endif
		//normal middle node
		dllnode_t *prev= to_delete->prev;
		dllnode_t *next=to_delete->next;
		prev->next=next;
		next->prev=prev;
		new_dllcurrent=next;

	}else if(to_delete->prev==NULL && to_delete->next!=NULL){
		#ifdef DEBUG
			//printf("Deleting first/head node\n"); 
		#endif
		//head node
		dllnode_t *next=to_delete->next;
		next->prev=NULL;
		next->is_head=1;
		new_dllcurrent=next;

	}else if(to_delete->prev!=NULL &&to_delete->next==NULL) {
		#ifdef DEBUG
			//printf("Deleting last node\n"); 
		#endif
		//last node
		dllnode_t *prev=to_delete->prev;
		prev->next=NULL;
		new_dllcurrent=to_delete->prev;

	}else{
		#ifdef DEBUG
			//printf("Deleting node that is first and last node\n"); 
		#endif
		new_dllcurrent=NULL;

	}

	return new_dllcurrent;
	
}


dllnode_t *get_head(dllnode_t *some_node){
	dllnode_t *current=some_node;
	while(!current->is_head){
		current=current->prev;
	}
	return current;
}

void print_dll(dllnode_t *head){

	dllnode_t *current=head->next;
	bool tailfound=false;
	while(!tailfound){
		printf("%d ", current->val );
		if(current->next!=NULL){
			current=current->next;

		}else{
			tailfound=true;
		}

	}
	printf("\n");
}