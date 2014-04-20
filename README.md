# BigInt

Arbitary-precision positive decimal integer support in C++.

## Functions

* Addition
* Subtraction
* Multiplication
* Division
* Modular
* Power
* Power and Mod

All operations support normal integer or BigInt as operands.

## Examples

Run `make` to build all the examples.

### bigint_demo

It is the simplest example to illustrate the usage of BigInt.

### diffie_hellman

This is the DH Key Exchange method, implemented roughly on RFC 2631.

However, the BigInt library is in decimal and thus bitwise operations are hard to implement.
Therefore some operations in the parameter generation (P and G) are ommited, which might do
harm to the overall security (especially the randomness).

The prime test is done in the following two steps:
1. Use a short prime list (smaller than about 2000) to do the quick sieve.
2. Use {2, 3, 5, 7, 11, 17} as the testing vector to do Fermat Prime Test.

Lengths:
* parameter Q: 160 (prime)
* parameter P: 1024 (prime)
* parameter G: 1024
* private keys: 160
* final shared key: 1024

There will be LOTS OF debug message. Redirect file descriptor 2 (stderr) to NULL if you do not want to see all these stuff.
