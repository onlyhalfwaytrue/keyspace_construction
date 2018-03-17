//
//  indexer.c
//
//
//  Created by Jaroor Modi and Frederick Lau on 1/24/18.
//

#include "indexer.h"
#include "traverse.c"

int is_uppercase (char letter){
	//	Tests whether a character is a uppercase letter.
	if('A' <= letter && letter <= 'Z'){
		return 1;
	}
	return 0;
}

int is_lowercase (char letter){
	//	Tests whether a character is an lowercase letter.
	if('a' <= letter && letter <= 'z'){
		return 1;
	}
	return 0;
}

int is_numeric (char numeral){
	//	Tests whether a character is a numerical character.
	if('0' <= numeral && numeral <= '9'){
		return 1;
	}
	return 0;
}

int is_accepted (char letter){
	//	Simply returns whether or not a character is either a letter or a number
	if(is_lowercase(letter) || is_uppercase(letter) || is_numeric(letter)){
		return 1;
	}
	return 0;
}

int num_words (char * in_string){
	//	Counts the number of words (sets of consecutive alphabetical characters),
	//	used for memory allocation and giving array sizes.
	int word_count = 0;
	int position = 0;
	char prev_char = '{';
	while (in_string[position] != 0){
		if((is_accepted(in_string[position])) && (!(is_accepted(prev_char)))){
			word_count++;
		}
		prev_char = in_string[position];
		//printf("\n%c  WORD: %d  ACCEPT? %d", prev_char, word_count, is_accepted(prev_char));
		position++;
	}
	return word_count;
}

char process_letter(char letter, char validation){
    //  Returns lowercase version of the letter and leaves unchanged if not a letter
    if (is_lowercase(letter) || (is_numeric(letter)&&(is_lowercase(validation)||is_uppercase(validation)))){
        return letter;
    }
    else if (is_uppercase(letter)){
        return letter+32;
    }
    else{
        return '-';
    }
}

char * normalize_string(char * in_string){
    //  Normalizes input string for separation: turns all non-alphabetical characters
    //  into the given delimiter for use in strtok operations later in the process.
    int length,position;
    char validation = '-';
    length = strlen(in_string);
    position = 0;
   
    char * out_string = malloc(sizeof(char) * length);
    strcpy(out_string, in_string);
   
    while(out_string[position]!=0){
        if(!(is_numeric(out_string[position]))){
            validation = out_string[position];
        }
        out_string[position] = process_letter(out_string[position], validation);
        position++;
    }
   
    printf("\n%s\n", out_string);
   
    return out_string;
}

char ** separate_string(char * in_string){
	//	Returns an array of strings (char * variables), each of which is a single word found in the
	//	input string. Here, "word" refers to a set of consecutive characters which are found in the
	//	English alphabet.
	int word_count, position;
	word_count = num_words(in_string);
	printf("\nWORD COUNT:   %d\n", word_count);
	position = 0;
	char * interim = normalize_string(in_string);
	char ** word_array = malloc(sizeof(char*) * (word_count + 1));
	
	word_array[position] = strtok(interim,"-");
	
	for(position=1; position <= word_count; position++){
		word_array[position] = strtok(NULL,"-");
	}
	
	*(word_array + word_count) = 0;
	
	return word_array;
}

node_t *file_node(char* wordd){
	node_t *fileNode = malloc(sizeof(node_t));
	fileNode->word=wordd;
	fileNode->count=malloc(sizeof(int));
	fileNode->prev=NULL;
	fileNode->next=NULL;
	return fileNode;
}
node_t * new_node(char * wordd){
	node_t * ret_node = malloc(sizeof(node_t));
	ret_node->word = wordd;
	ret_node->count = (int *) calloc(numFiles, sizeof(int));
	ret_node->prev = NULL;
	ret_node->next = NULL;
	return ret_node;
}

relation comes_first(char * new_word, char * old_word){
	//	Returns 1 if new_word sorts before old_word, 0 otherwise. Used in the insert_node function
	//	where the new_word field is the new node to insert and the old_word field will be used for
	//	the preexisting nodes in the linked list as we iterate through.
	int position;
	if(strcmp(new_word,old_word) == 0){
		//		If the new word is the same as the old, we say it is a "clone" for counting
		return CLONE;
	}
	position = 0;
	//compare character by character
	while(new_word[position] != 0 && old_word[position] != 0){
		//both chars numeric
		if ((is_numeric(new_word[position])) && (is_numeric(old_word[position]))){
			if(new_word[position]<old_word[position]){
				//printf("case 1 %s BEFORE %s\n", new_word, old_word);
				return BEFORE;
			}
			if(new_word[position]>old_word[position]){
				return AFTER;
			}
		}
		//only new word char is numeric
		else if ((is_numeric(new_word[position])) && !(is_numeric(old_word[position]))){
			return AFTER;
		}
		
		//only old word char is numeric
		else if (!(is_numeric(new_word[position])) && (is_numeric(old_word[position]))){
			//printf("case 2 %s BEFORE %s\n", new_word, old_word);
			return BEFORE;
		}
		
		//neither char is numeric
		else{
			if(new_word[position]<old_word[position]){
				//printf("case 3 %s BEFORE %s\n", new_word, old_word);
				//printf("because %c is LESS THAN %c\n", new_word[position], old_word[position]);
				return BEFORE;
			}
			if(new_word[position]>old_word[position]){
				return AFTER;
			}
		}
		position ++;
	}
	//everything compared so far is equivalent, so we just put the shorter word first
	if(strlen(old_word) > strlen(new_word)){
		return BEFORE;
	}
	return AFTER;
}

void insertFile(list_t *list, char *file){
	node_t *new_file=file_node(file);
	new_file->count[0]=numFiles;
	if(list->head==NULL){
		list->head=new_file;
		return;
	}
	else{
		node_t *temp=list->head;
		while(temp!=NULL){
			temp=temp->next;
		}
		temp=new_file;
	}
}

void insert_node(list_t * list, char * wordd, int update_index){
	//	Inserts nodes into our linked list structure but in the proper, sorted order using the
	//	comes_first comparison function to find the word's relative position in the list.
	node_t * new_word = new_node(wordd);
	if(list->head == NULL){
		//printf("Inserted new head: %s\n", new_word->word);
		list->head = new_word;
		return;
	}
	node_t * current = list->head;
	while (current!=NULL){
		if(comes_first(new_word->word, current->word) == CLONE){
			current->count[update_index] += 1;
			free(new_word);
			return;
		}
		current = current->next;
	}
	current = list->head;
	while (current->next!=NULL){
		switch(comes_first(new_word->word, current->word)){
			case BEFORE:
				//printf("\n placed %s BEFORE %s\n", new_word->word, current->word);
				if (current->prev == NULL){
					//printf("inserted new head: %s\n", new_word->word);
					new_word->next = current;
					current->prev = new_word;
					new_word->prev = NULL;
					list->head = new_word;
					return;
				}
				new_word->prev = current->prev;
				current->prev->next = new_word;
				new_word->next = current;
				current->prev = new_word;
				return;
			case CLONE:
			case AFTER:
				break;
		}
		current = current->next;
	}
	//add to end if it doesn't come before any of the words in the list
	current->next = new_word;
	new_word->prev = current;
	return;
}

list_t * sort_list (char ** word_array, list_t *use_list, int update_index){
	//	Creates an empty list and inserts nodes using the insert_node function defined above. This
	//	results in a linked list with the nodes such that the words they contain are sorted from head to tail.
	list_t * ret_list;
	if (use_list == NULL){
		ret_list = malloc(sizeof(list_t));
		ret_list->head = NULL;
	}
	else{
		ret_list = use_list;
	}
	int position = 0;
	while(word_array[position]!=0){
		insert_node(ret_list, word_array[position], update_index);
		position++;
	}
	return ret_list;
}

void print_list(list_t * sorted){
	//	Prints input linked list. The list is sorted by the time this function is called,
	//	this completes our program.
	node_t * current = sorted->head;
	while(current != NULL){
		printf("%s, %d\n",current->word, current->count[0]);
		current = current->next;
	}
	return;
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


