CC=g++
CFLAGS=-std=c++11 -O2 -Wall

all: bigint.o bigint_demo

bigint.o:
	$(CC) bigint.cpp -o bigint.o -c $(CFLAGS)

bigint_demo:
	$(CC) bigint.o bigint_demo.cpp -o bigint_demo $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o bigint_demo
