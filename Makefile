CC=gcc
CFLAGS=-I.

dpmake: chopsticks.o philosophers.o
	$(CC) -o dpmake chopsticks.o philosophers.o -I.
