CC = gcc
CFLAGS = -g -Wall

all: traverse

indexer: indexer.c traverse.c
	$(CC) $(CFLAGS) indexer.c traverse.c -o indexer

traverse: indexer.c traverse.c
	$(CC) $(CFLAGS) -c indexer.c
	$(CC) $(CFLAGS) traverse.c indexer.c -o traverse
clean:
	rm -f traverse indexer