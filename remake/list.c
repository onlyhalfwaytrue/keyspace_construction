#include "list.h"

list_t * inverted_index=NULL;

node_t * new_node(char * wordd){
	node_t * ret_node = (node_t *) malloc(sizeof(node_t));
	ret_node->word = wordd;
	ret_node->count = (int *) calloc(numFiles, sizeof(int));
	int i;
	for(i=0; i<globalIndex; i++){
		ret_node->count[i]=0;
	}
	ret_node->ptr=NULL;
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

void insert_node(list_t * list, char * wordd, int update_index){
	//	Inserts nodes into our linked list structure but in the proper, sorted order using the
	//	comes_first comparison function to find the word's relative position in the list.
	node_t * new_word = new_node(wordd);
	new_word->count[update_index]++;
	//new_word->count[update_index]++;
//	printf("First check %d\n", new_word->count[1]);
	if(list->head==NULL){
		//printf("%d\n", new_word->count[1]);
		list->head=new_word;
		return;
	}
	node_t * current=list->head;
	
	while (current!=NULL){
		if(comes_first(new_word->word, current->word) == CLONE){
			current->count[update_index]++;
			free(new_word);
			return;
		}
		current = current->next;
	}
	
	current = list->head;
	while (current->next!=NULL){
		
		if(comes_first(new_word->word, current->word) == BEFORE){
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
		}
		current = current->next;
	}
	if(comes_first(new_word->word, current->word) == BEFORE){//before tail
		new_word->prev = current->prev;
		current->prev->next = new_word;
		new_word->next = current;
		current->prev = new_word;
		return;
	}
	//printf("\n placed %s AFTER %s\n", new_word->word, current->word);//after tail
	current->next = new_word;
	new_word->prev = current;
	new_word->next = NULL;
}

void sort_list (char ** word_array, int update_index){
	//	Creates an empty list and inserts nodes using the insert_node function defined above. This
	//	results in a linked list with the nodes such that the words they contain are sorted from head to tail.
	if (inverted_index == NULL){
		inverted_index = malloc(sizeof(list_t));
		inverted_index->head = NULL;
	}
	int position = 0;
	while(word_array[position]!=0){
//		printf("list %s index %d\n", word_array[position], update_index);
		insert_node(inverted_index, word_array[position], update_index);
		position++;
	}
	return;
}
void print_list(list_t * sorted, file_t * theList){
	//	Prints input linked list. The list is sorted by the time this function is called,
	//	this completes our program.
	printf("PRINTLIST CALL");
	node_t * temp;
	file_t * file = theList;
	int i;
	for(temp=sorted->head; temp!=NULL; temp=temp->next){
		file=theList;
		i=0;
		while(file!=NULL){
			if(temp->count[i]==0){
				i++;
				file=file->next;
				//continue;
			}
			else{
				printf("Token %s occurs %d times in file %s\n", temp->word, temp->count[i], file->filename);
				i++;
				file=file->next;
				//continue;
			}
		}
	}
}
