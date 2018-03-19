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
	//new_word->count[update_index]++;
	//printf("First check %d\n", new_word->count[1]);
	if(list->head==NULL){
		new_word->count[update_index]++;
		//printf("%d\n", new_word->count[1]);
		list->head=new_word;
		return;
	}
	node_t * curr=list->head;
	node_t * prev=NULL;
	int didFind=0;
	while(curr!=NULL){
		//printf("%s %s\n", curr->word, wordd);
		if(strcmp(curr->word, wordd)>=0){
			if(strcmp(curr->word, wordd)==0){
				curr->count[update_index]++;
				//printf("%d\n", curr->count[1]);
				//printf("Global Index is %d, Index ix %d and count is %d\n", globalIndex, update_index, curr->count[update_index]);
				didFind=1;
				return;
			}
		}
		else{
			prev=curr;
			curr=curr->next;
		}

	}
	if(didFind==0){
		new_word->count[update_index]++;
		//printf("%d\n", new_word->count[1]);
		if(curr==list->head){
		new_word->next=curr;
		list->head=new_word;
		}
		else{
		new_word->next=curr;
		prev->next=new_word;
		}
	}

	//if we get here we know we did not find a match or inserted anywhere in the list
	/*if(list->head == NULL){
		//printf("Inserted new head: %s\n", new_word->word);
		list->head = new_word;
		return;
	}
	node_t * current = list->head;
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
	return;*/
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
		//printf("list %s\n", word_array[position]);
		insert_node(inverted_index, word_array[position], update_index);
		position++;
	}
	return;
}

void print_list(list_t * sorted, file_t * file_list){
	//	Prints input linked list. The list is sorted by the time this function is called,
	//	this completes our program.
	node_t * temp;
	file_t * file;
	int i;
	for(temp=sorted->head; temp!=NULL; temp=temp->next){
		printf("Token is %s\n", temp->word);
		for(i=0; i<numFiles; i++){
			printf("Count is %d\n", temp->count[i]);
		}
	}

	for(file=file_list; file!=NULL; file=file->next){
		printf("File is %s\n", file->filename);
	}

	
}