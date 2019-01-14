#pragma once

typedef struct dllnode {
	struct arbitray_struct *s;
	int val;
	bool is_head;
    struct dllnode * prev;
    struct dllnode * next;
} dllnode_t;

dllnode_t *insert_dllnode( struct dllnode * prev, struct arbitray_struct *s, int val);
void set_head( struct arbitray_struct *s, int val, struct dllnode * list_head);
dllnode_t *create_head( struct arbitray_struct *s, int val);
void set_values(struct arbitray_struct *s, int val, struct dllnode * element,  struct dllnode * prev);
dllnode_t *del_dllnode(dllnode_t *to_delete);
dllnode_t *delink_dllnode(dllnode_t *to_delete);
dllnode_t *get_head(dllnode_t *some_node);
void print_dll(dllnode_t *head);