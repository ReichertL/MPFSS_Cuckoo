#pragma once

//Source: https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
struct bt_node 
{ 
    int key;
    struct arbitray_struct *s;
    struct bt_node *left, *right; 
}; 
   
// A utility function to create a new BST bt_bt_node 
struct bt_node *newBTNode(struct arbitray_struct *s, int key);
   
// A utility function to do inorder traversal of BST 
void inorder(struct bt_node *root); 
void inorder_array(struct bt_node *root, struct arbitray_struct **arr); 
void write_in_array(struct bt_node *root, struct arbitray_struct **arr, int index);

/* A utility function to insert a new bt_bt_node with given key in BST */
struct bt_node* insert(struct bt_node* bt_node, struct arbitray_struct *s, int key); 
struct bt_node * minValueBTNode(struct bt_node* bt_node); 
struct bt_node* deleteBTNode(struct bt_node* root, int key);  
struct bt_node* search(struct bt_node* root, int key);
int get_size(struct bt_node *root);
bool increase_counter(struct bt_node *root);