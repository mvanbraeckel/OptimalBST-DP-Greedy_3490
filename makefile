# @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
# @course CIS*3490: Analysis and Design of Computer Algorithms - A4
# @version 23/03/2019
# @file makefile
# @brief compiles everything for A4
 
CC = gcc
CFLAGS = -g -Wall -std=c11 -pedantic

all: P1 P2 main

main: P1 P2 main.c a4header.h
	$(CC) $(CFLAGS) -c main.c -o bin/main.o
	$(CC) $(CFLAGS) bin/*.o -o bin/run

P1: P1.c
	$(CC) $(CFLAGS) -c P1.c -o bin/P1.o
P2: P2.c
	$(CC) $(CFLAGS) -c P2.c -o bin/P2.o

val:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./bin/run

clean:
	rm -f *.o bin/*