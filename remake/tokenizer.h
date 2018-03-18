#ifndef tokenizer_h
#define tokenizer_h
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int is_uppercase (char letter);
int is_lowercase (char letter);
int is_numeric (char numeral);
int is_accepted (char letter);
int num_words (char * in_string);
char process_letter (char letter, char validation);
char * normalize_string (char * in_string);
char ** separate_string (char * in_string);

#endif