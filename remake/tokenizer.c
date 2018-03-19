#include "tokenizer.h"


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
   
    //printf("\n%s\n", out_string);
   
    return out_string;
}

char ** separate_string(char * in_string){
	//	Returns an array of strings (char * variables), each of which is a single word found in the
	//	input string. Here, "word" refers to a set of consecutive characters which are found in the
	//	English alphabet.
	int word_count, position;
	word_count = num_words(in_string);
	//printf("\nWORD COUNT:   %d\n", word_count);
	position = 0;
	char * interim = normalize_string(in_string);
	char ** word_array = malloc(sizeof(char*) * (word_count + 1));
	
	word_array[position] = strtok(interim,"-");
	
	for(position=1; position <= word_count; position++){
		word_array[position] = strtok(NULL,"-");
	}
	
	*(word_array + word_count) = 0;
	/*while(*word_array){
		printf("2 %s\n", *word_array++);
	}*/
	return word_array;
}