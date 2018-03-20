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

/*typedef struct output_list{
	char * file;
	int count;
	struct outputList * next;
}out_t;*/

//out_t *outList=NULL;

void finalList(){
	node_t * temp;
	file_t * file;
	int i;
	for(temp=inverted_index->head; temp!=NULL; temp=temp->next){
		out_t * outList=NULL;
		file=filelist;
		i=0;
		while(file!=NULL){
			if(temp->count[i]==0){
				i++;
				file=file->next;
				continue;
			}
			else{
				out_t * newOut=malloc(sizeof(out_t));
				newOut->file=file->filename;
				newOut->count=temp->count[i];
				newOut->next=NULL;
				if(outList==NULL){
					outList=newOut;
				}
				else{
					out_t * current=outList;
					out_t * prev=NULL;
					while(current!=NULL){
						if(newOut->count>=current->count){
							break;
						}
						else{
							prev=current;
							current=current->next;
							continue;
						}
					}
					if(current==outList){
						newOut->next=outList;
						outList=newOut;
					}
					else{
						newOut->next=current;
						prev->next=newOut;
					}
				}
				i++;
				file=file->next;
			}
		}
		temp->ptr=outList;
	}
}
int is_regular_file(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/*void makeOutput(char * outputfile){
	FILE *output=fopen(outputfile, "w");
	fprintf(output, "%s\n", "<? xml version=\"1.0\" encoding=\"UTF-8\"?>");
	fprintf(output, "%s\n", "<fileIndex>");
}*/

int main(int argc, char *argv[]){
	//printf("%p\n", &outList);
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
	finalList();
	node_t * temp=inverted_index->head;
	out_t * tOut;
	while(temp!=NULL){
		tOut=temp->ptr;
		while(tOut!=NULL){
			printf("Token %s occurs %d times in %s\n", temp->word, tOut->count, tOut->file);
			tOut=tOut->next;
		}
		temp=temp->next;
	}
	//makeOutput(argv[1]);
	free(filelist);
	free(inverted_index);
	return 0;
}




