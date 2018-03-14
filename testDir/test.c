#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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

//function to take entire content of file, whitespaces and all, and put it into an array
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
        fputs("Error reading file", stderr);
      }
      else{
        source[++newLen]= '\0';
      }
    }
  }
  printf("%s\n", source);
}

void tokenize(char *string){
  
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
    DIR *d;
    struct dirent *dir;
    d=opendir(argv[1]);
    //dir=readdir(d);
    if(d){
      while ((dir=readdir(d))!=NULL){
        //printf("%s\n", dir->d_name);
        if(!strcmp(dir->d_name, ".")||!strcmp(dir->d_name, "..")){
          continue;
        }
        FILE *fp=NULL;
        char* filePath=makePath(argv[1], dir->d_name);
        fp=fopen(filePath, "r");
        printf("Path is %s\n", filePath);
        printf("Exporting %s...\n", dir->d_name);
        export(fp);
        fclose(fp);
      }
      closedir(d);
    }
    return 0;
  }
}