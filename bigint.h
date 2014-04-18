#ifndef _BIGINT_H_
#define _BIGINT_H_

// int32_t is in cinttypes, which requires C++11
#include <cinttypes>
#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

// Put four decimal digits in one int
#define COMPRESSED_NUM  ( 10000 )
#define COMPRESSED_BITS ( 4 )

// Use int32_t instead of int to avoid subtle problems
typedef int32_t single_int;

class BigInt {
    private:
        vector< single_int > data;

        void mul10 ( );
        void div10 ( );

    public:
        BigInt ( );
        BigInt ( unsigned long x );
        BigInt ( const BigInt &x );

        friend ostream &operator << ( ostream &os, const BigInt &num );
        friend istream &operator >> ( istream &is, BigInt &num );
        friend BigInt operator + ( const BigInt &a, const BigInt &b );
        friend BigInt operator - ( const BigInt &a, const BigInt &b );
        friend BigInt operator * ( const BigInt &a, const BigInt &b );
        friend BigInt operator / ( BigInt a, BigInt b );
        friend BigInt operator % ( BigInt a, BigInt b );

        friend BigInt power ( const BigInt &a, const BigInt &b );

        friend int cmp ( const BigInt &a, const BigInt &b );
        friend bool operator < ( const BigInt &a, const BigInt &b );
        friend bool operator > ( const BigInt &a, const BigInt &b );
        friend bool operator <= ( const BigInt &a, const BigInt &b );
        friend bool operator >= ( const BigInt &a, const BigInt &b );
        friend bool operator == ( const BigInt &a, const BigInt &b );
        friend bool operator != ( const BigInt &a, const BigInt &b );
};

#endif // _BIGINT_H_
