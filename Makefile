CC=g++
CFLAGS=-std=c++11 -O2 -Wall

all: bigint.o bigint_demo

bigint.o: bigint.cpp bigint.h
	$(CC) bigint.cpp -o bigint.o -c $(CFLAGS)

# Do not reuse bigint.o so that __DEBUG__ macro will work
bigint_demo: bigint_demo.cpp bigint.h bigint.cpp
	$(CC) bigint.cpp bigint_demo.cpp -o bigint_demo $(CFLAGS) -D__DEBUG__

.PHONY: clean
clean:
	rm -f *.o bigint_demo
