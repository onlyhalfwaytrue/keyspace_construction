#include "indexer.h"

int numFiles = 0;
int haveNum = 0;
file_t * filelist = NULL;

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
	file_t * newfile = (file_t *) malloc(sizeof(file_t));
	newfile->filename = name;
	newfile->seqnum = numFiles;
	newfile->next = NULL;
	
	if (filelist==NULL){
		filelist= newfile;
	}
	else{
		file_t * temp = filelist;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = newfile;
	}
}

//function to take entire content of file, whitespaces and all, and put it into an array
//to be parsed
void export(FILE *file){
	printf("In export..\n");
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
				fputs("FILE IS EMPTY...", stderr);
			}
			else{
				source[++newLen]= '\0';
			}
		}
	}
	printf("%s\n", source);
	char ** split_words = separate_string(source);
	sort_list(split_words, 0);
	print_list(inverted_index);
}

/*int main (int argc, char *argv[]){
 
 
 if (argc<2){
 printf("You must input at least one string to run this program.\nRun using the format ./stringsorter \"<your input string here>\"\n");
 return 0;
 }
 if(argc>2){
 printf("No more than one input string allowed at a time.\nRun using the format ./stringsorter \"<your input string here>\"\n");
 return 0;
 }
 
 int length = strlen(argv[1]);
 char * input_string = malloc(sizeof(char)*length);
 strcpy(input_string,argv[1]);
 char ** split_words = separate_string(input_string);
 list_t * sorted = sort_list(split_words);
 print_list(sorted);
 
 return 0;
 }*/

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
				
				
				numFiles++;
				
				printf("%d\n", numFiles);
				continue;
			}
			else if(haveNum==1){
				FILE *fp=NULL;
				char* filePath=makePath(name, entry->d_name);
				printf("The file path is %s\n", filePath);
				fp=fopen(filePath, "r");
				printf("Exporting %s...\n", entry->d_name);
				export(fp);
			}
		}
	}
	closedir(dir);
}

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
	return 0;
}

