//
//  indexer.c
//
//
//  Created by Jaroor Modi and Frederick Lau on 1/24/18.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "traverse.c"

int is_regular_file(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


int main(int argc, char *argv[]){
	if(argc<2){
		printf("Too few arguments, exiting...\n");
		return 0;
	}
	else if(argc>2){
		printf("Too many arguments, exiting...\n");
		return 0;
	}
	else{
		if(is_regular_file(argv[1])){
			FILE *p=fopen(argv[1], "r");
			if(p!=NULL){
				file_t * newfile = malloc(sizeof(file_t));
				newfile->filename=argv[1];
				newfile->next=NULL;
				filelist=newfile;
				exportFile(p);
			}
		}
		else{
			traverseDir(argv[1]);
			haveNum=1;
			traverseDir(argv[1]);
		}
	}
	free(filelist);
	free(inverted_index);
	return 0;
}


