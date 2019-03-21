typedef struct node {

	int index_in_batch;
    int batch_no;
    int val;
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
void del_batch(batch_t *to_delete);

void print_batch_list(batch_t *head);
