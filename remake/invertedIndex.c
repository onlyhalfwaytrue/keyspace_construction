//
//  invertedIndex
//
//
//  Created by Jaroor Modi and Frederick Lau
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "traverse.c"

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

void makeOutput(char * outputfile){
	FILE *output=freopen(outputfile, "w", stdout);
	fprintf(output, "%s\n", "<? xml version=\"1.0\" encoding=\"UTF-8\"?>");
	fprintf(output, "%s\n", "<fileIndex>");
	node_t * temp=inverted_index->head;
	out_t * tOut;
	while(temp!=NULL){
		printf("\t<word text=\"%s\">\n", temp->word);
		tOut=temp->ptr;
		while(tOut!=NULL){
			printf("\t\t<file name=\"%s\">%d</file>\n", tOut->file, tOut->count);
			tOut=tOut->next;
		}
		printf("\t</word>\n");
		temp=temp->next;
	}
	fprintf(output, "%s\n", "</fileIndex>");
}

int main(int argc, char *argv[]){
	//printf("%p\n", &outList);
	if(argc<3){
		printf("Too few arguments, exiting...\n");
		return 0;
	}
	else if(argc>3){
		printf("Too many arguments, exiting...\n");
		return 0;
	}
	else{
		if(is_regular_file(argv[2])){
			FILE *p=fopen(argv[2], "r");
			if(p!=NULL){
				file_t * newfile = malloc(sizeof(file_t));
				newfile->filename=argv[2];
				newfile->next=NULL;
				filelist=newfile;
				exportFile(p);
			}
		}
		else{
			traverseDir(argv[2]);
			haveNum=1;
			traverseDir(argv[2]);
		}
	}
	finalList();
	makeOutput(argv[1]);
	free(filelist);
	free(inverted_index);
	return 0;
}




