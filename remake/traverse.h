#ifndef indexer_h
#define indexer_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

extern int numFiles;
extern int haveNum;
extern int globalIndex;

typedef struct file_node{
	//	These nodes just hold names of files and their sequence number to associate word counts with
	char * filename;
	//int seqnum;
	struct file_node * next;
} file_t;

extern file_t * filelist;

char* makePath(char* s1, char* s2);
//void insert_file(file_t ** head, char * name);
void exportFile(FILE *file);
void traverseDir(char *name);

#endif