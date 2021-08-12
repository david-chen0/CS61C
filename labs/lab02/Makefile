CC=gcc
CFLAGS=-Wall -std=c99

all: vector

vector: vector.o test_vector.o
	$(CC) -o vector vector.o test_vector.o

vector.o: bit_ops.c bit_ops.h
	$(CC) $(CFLAGS) -g -c vector.c

test_vector.o: test_bit_ops.c bit_ops.h
	$(CC) $(CFLAGS) -g -c test_vector.c

clean:
	rm vector vector.o test_vector.o
