# ache; Cheng, Allan

CC = g++

all: huffman

huffman: huffman.o
	$(CC) -o huffman huffman.o

huffman.o: huffman.cpp classes.h
	$(CC) -o huffman.o -c -O3 -g huffman.cpp

clean:
	rm huffman.o huffman
