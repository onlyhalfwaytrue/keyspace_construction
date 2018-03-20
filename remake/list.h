#ifndef list_h
#define list_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum relative_position {BEFORE, AFTER, CLONE} relation;

typedef struct output_list{
	char * file;
	int count;
	struct output_list * next;
}out_t;

typedef struct word_node{
	//	Nodes know the word which they represent and the node which follows in the linked list.
	char * word;
	int * count;
	struct output_list * ptr;
	struct word_node * prev;
	struct word_node * next;
}node_t;

typedef struct node_list{
	//	This is the "list" structure which is the primary structure which the functions below work with.
	//	This was created to make tracking the head of our linked list manageable.
	node_t * head;
}list_t;

extern list_t * inverted_index;

node_t * new_node(char * wordd);
relation comes_first(char * new_word, char * old_word);
void insert_node(list_t * list, char * wordd, int update_index);
void sort_list (char ** word_array, int update_index);
void print_list(list_t * sorted, file_t * theList);

#endif