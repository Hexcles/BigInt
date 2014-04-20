CC=g++
CFLAGS=-std=c++11 -O2 -Wall -lm

all: bigint.o bigint_demo diffie_hellman

bigint.o: bigint.cpp bigint.h
	$(CC) bigint.cpp -o bigint.o -c $(CFLAGS)

# Do not reuse bigint.o so that __DEBUG__ macro will work
bigint_demo: bigint_demo.cpp bigint.h bigint.cpp
	$(CC) bigint.cpp bigint_demo.cpp -o bigint_demo $(CFLAGS) -D__DEBUG__

diffie_hellman: DiffieHellman/diffie_hellman.cpp
	$(CC) DiffieHellman/diffie_hellman.cpp DiffieHellman/sha1.cpp bigint.o -o diffie_hellman $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o bigint_demo diffie_hellman
