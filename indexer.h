//
//  indexer.h
//  
//
//  Created by Jaroor Modi and Frederick Lau on 3/16/18.
//

#ifndef indexer_h
#define indexer_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

int numFiles = 0;
int haveNum = 0;

typedef enum relative_position {BEFORE, AFTER, CLONE} relation;

typedef struct word_node{
	//	Nodes know the word which they represent and the node which follows in the linked list.
	char * word;
	int * count;
	struct word_node * prev;
	struct word_node * next;
}node_t;

typedef struct node_list{
	//	This is the "list" structure which is the primary structure which the functions below work with.
	//	This was created to make tracking the head of our linked list manageable.
	node_t * head;
}list_t;
list_t *fileHead;


int is_uppercase(char letter);
int is_lowercase(char letter);
int is_numeric(char numeral);
int is_accepted(char letter);
int num_words(char * in_string);
char process_letter(char letter, char validation);
char * normalize_string(char * in_string);
char ** separate_string(char * in_string);
node_t *file_node(char* wordd);
node_t * new_node(char * wordd);
relation comes_first(char * new_word, char * old_word);
void insertFile(list_t *list, char *file);
void insert_node(list_t * list, char * wordd, int update_index);
list_t * sort_list (char ** word_array, list_t *use_list, int update_index);
void print_list(list_t * sorted);

char* makePath(char* s1, char* s2);
void export(FILE *file);
void traverseDir(char *name);


#endif /* indexer_h */
