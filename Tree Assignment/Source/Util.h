#ifndef UTIL_H
#define UTIL_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*General node structure to use in tree. Children will be stored dynamically*/
typedef struct node Node;
struct node{
    char data;
    char* password;
    int child_count;
    Node* child;
};

/*Assigns password to given node, which is last letter of a username*/
void assign_password(Node* , char*);

/*Adds a new children to given parent*/
void add_node(Node*, Node);

/*Obtains search for a child with given data in parent's children
	and returns an index value if child is present, -1 otherwise*/
int search(Node*, char);

/*Adds a new username with a password to root*/
void add_word(Node* , char*, char*);

/*Appends given char to given string. Actually this function is redundant :) but it was designed
to use with dynamic strings at first, but then it turned out to constant size array
but function stayed since I use it many times in source code*/
void append_char(char*, char);

/*Gives branchings by reading until first multi-children node from root*/
Node* extract_branch(Node*, char*);

/*Prints preorder traversal of tree*/
void print_preorder(Node*,char*, FILE*);

/*Searchs for a given username on tree, returns "!" or "!!" or username */
char* search_tree(Node*, char*);

/*Removes the node with given data from a parent's children*/
void remove_node(Node*, char);

/*Deletes a word from tree*/
void delete(Node*, char*);

/*Frees all nodes on tree*/
void free_all(Node*);
#endif
