//
//  indexer.c
//
//
//  Created by Jaroor Modi and Frederick Lau on 1/24/18.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "traverse.c"



int main(int argc, char *argv[]){
	if(argc<2){
		printf("Too few arguments, exiting...");
		return 0;
	}
	else if(argc>2){
		printf("Too many arguments, exiting...");
		return 0;
	}
	else{
		traverseDir(argv[1]);
		haveNum=1;
		traverseDir(argv[1]);
	}
	free(filelist);
	free(inverted_index);
	return 0;
}


