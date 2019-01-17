#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include <string.h>
#include <math.h>
#include "binary_search_tree.h"


int counter;

// A utility function to create a new BST bt_node 
struct bt_node *newBTNode(struct arbitray_struct *s, int key) 
{ 
    struct bt_node *temp =  (struct bt_node *)malloc(sizeof(struct bt_node)); 
    temp->s = s; 
    temp->key=key;
    temp->left = temp->right = NULL; 
    return temp; 
} 
  
// A utility function to do inorder traversal of BST 
void inorder(struct bt_node *root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->left); 
        printf("%d ", root->key); 
        inorder(root->right); 
    } 
} 
 
// writes all key into an array, start index has to be 1
void write_in_array(struct bt_node *root, struct arbitray_struct **arr, int index){
    counter=0;
    inorder_array(root, arr);
}

void inorder_array(struct bt_node *root, struct arbitray_struct **arr) 
{ 
    if(root != NULL){

        arr[counter]=root->s;
        counter++;
    

        //printf("index: %d,  key: %d \n", counter, root->key);

                
        inorder_array(root->left, arr); 
        inorder_array(root->right, arr);
    }
} 

/* A utility function to insert a new bt_node with given key in BST */
struct bt_node* insert(struct bt_node* bt_node, struct arbitray_struct *s, int key) 
{ 
    /* If the tree is empty, return a new bt_node */
    if (bt_node == NULL) return newBTNode(s, key); 
  
    /* Otherwise, recur down the tree */
    if (key < bt_node->key) 
        bt_node->left  = insert(bt_node->left, s, key); 
    else
        bt_node->right = insert(bt_node->right,s,  key); 
  
    /* return the (unchanged) bt_node pointer */
    return bt_node; 
} 
  
/* Given a non-empty binary search tree, return the bt_node with minimum 
   key value found in that tree. Note that the entire tree does not 
   need to be searched. */
struct bt_node * minValueBTNode(struct bt_node* bt_node) 
{ 
    struct bt_node* current = bt_node; 
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL) 
        current = current->left; 
  
    return current; 
} 
  
/* Given a binary search tree and a key, this function deletes the key 
   and returns the new root */
struct bt_node* deleteBTNode(struct bt_node* root, int key) 
{ 
    // base case 
    if (root == NULL) return root; 
  
    // If the key to be deleted is smaller than the root's key, 
    // then it lies in left subtree 
    if (key < root->key) 
        root->left = deleteBTNode(root->left, key); 
  
    // If the key to be deleted is greater than the root's key, 
    // then it lies in right subtree 
    else if (key > root->key) 
        root->right = deleteBTNode(root->right, key); 
  
    // if key is same as root's key, then This is the bt_node 
    // to be deleted 
    else
    { 
        // bt_node with only one child or no child 
        if (root->left == NULL) 
        { 
            struct bt_node *temp = root->right; 
            free(root); 
            return temp; 
        } 
        else if (root->right == NULL) 
        { 
            struct bt_node *temp = root->left; 
            free(root); 
            return temp; 
        } 
  
        // bt_node with two children: Get the inorder successor (smallest 
        // in the right subtree) 
        struct bt_node* temp = minValueBTNode(root->right); 
  
        // Copy the inorder successor's content to this bt_node 
        root->key = temp->key; 
  
        // Delete the inorder successor 
        root->right = deleteBTNode(root->right, temp->key); 
    } 
    return root; 
}

// C function to search a given key in a given BST 
struct bt_node* search(struct bt_node* root, int key) 
{ 
    if (root == NULL) 
       return NULL;

    // Base Cases: root is null or key is present at root 
    if (root->key == key) 
       return root; 
   

    // Key is greater than root's key 
    if (root->key < key) 
       return search(root->right, key); 
  
    // Key is smaller than root's key 
    return search(root->left, key); 
}  


// returns no of elements in binary search tree
int get_size(struct bt_node *root){
    counter=0;
    bool works=increase_counter(root);
    return counter;
}

bool increase_counter(struct bt_node *root) 
{ 
    if(root != NULL){

        counter++;
                
        increase_counter(root->left); 
        increase_counter(root->right);
    }
    return true;
} 