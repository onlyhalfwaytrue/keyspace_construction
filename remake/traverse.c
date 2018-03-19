#include "traverse.h"
#include "tokenizer.c"
#include "list.c"

file_t * filelist = NULL;
int numFiles=0;
int haveNum=0;
int globalIndex=0;

//function that does exactly what snprintf does because why not
char* makePath(char* s1, char* s2){
	int len = strlen(s1) + strlen(s2);
	char* temp=(char*)malloc(len*sizeof(char)+2);
	int i;
	int j;
	for(i=0; i<strlen(s1); i++){
		temp[i]=s1[i];
	}
	temp[i]='/';
	for(j=0; j<strlen(s2); j++){
		i++;
		temp[i]=s2[j];
	}
	i++;
	temp[i]='\0';
	//printf("%s\n", temp);
	return temp;
}

//inserts new file nodes for the global file list variable;
void insert_file(char * name){
	//printf("Inserting file %s\n", name);
	file_t * newfile = (file_t *) malloc(sizeof(file_t));
	newfile->filename = name;
	newfile->seqnum = numFiles;
	newfile->next = NULL;
	
	if (filelist==NULL){
		filelist= newfile;
		//printf("File %s inserted!\n", newfile->filename);
		return;
	}
	else{
		file_t * temp = filelist;
		while(temp->next!= NULL){
			temp=temp->next;
			//printf("Current file is %s\n", temp->filename);
			//printf("%d\n", i);
		}
		temp->next=newfile;
		//printf("File %s inserted!\n", newfile->filename);
	}
}

//function to take entire content of file, whitespaces and all, and put it into an array
//to be parsed
void exportFile(FILE *file){
	//printf("In export..\n");
	char *source = NULL;
	if (file != NULL){
		if(fseek(file, 0L, SEEK_END)==0){
			long bufSize=ftell(file);
			if(bufSize==-1){
				printf("Error reading file size, exiting...");
				return;
			}
			source=malloc(sizeof(char)*(bufSize+1));
			if(fseek(file, 0L, SEEK_SET)!=0){
				printf("Some error...exiting...");
				return;
			}
			size_t newLen = fread(source, sizeof(char), bufSize, file);
			if(newLen==0){
				fputs("FILE IS EMPTY...\n", stderr);
			}
			else{
				source[++newLen]= '\0';
			}
		}
	}
	//printf("%s\n", source);
	char ** split_words = separate_string(source);
	sort_list(split_words, globalIndex);
	print_list(inverted_index, filelist);
}

//function to traverse directory to access all subdirectories
void traverseDir(char *name){
	DIR *dir;
	struct dirent *entry;
	if (!(dir=opendir(name))){
		printf("Failed to open directory... exiting\n");
		return;
	}
	while ((entry=readdir(dir))!= NULL){
		if(entry->d_type==DT_DIR){
			char path[1024];
			if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0){
				continue;
			}
			//subdirectory to recursively traverse
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);;
			traverseDir(path);
		}
		else{
			//file to be opened or counted
			if(haveNum==0){
				insert_file(entry->d_name);
				//printf("%d\n", numFiles);
				numFiles++;
				continue;
			}
			else if(haveNum==1){
				FILE *fp=NULL;
				char* filePath=makePath(name, entry->d_name);
				//printf("The file path is %s\n", filePath);
				fp=fopen(filePath, "r");
				printf("Exporting %s...\n", entry->d_name);
				//printf("%d\n", globalIndex);
				exportFile(fp);
				globalIndex++;
			}
		}
	}
	closedir(dir);
}