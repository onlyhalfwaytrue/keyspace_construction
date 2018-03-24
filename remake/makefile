CC=gcc
CFLAGS=-g -Wall

make: invertedIndex.c traverse.c tokenizer.c list.c traverse.h tokenizer.h list.h
	$(CC) $(CFLAGS) invertedIndex.c -o invertedIndex

.PHONY: clean

clean:
	rm invertedIndex