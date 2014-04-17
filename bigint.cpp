#include "bigint.h"

using namespace std;

BigInt::BigInt ( ) {
}

BigInt::BigInt ( unsigned long x ) {
    while ( x ) {
        data.push_back( x % COMPRESSED_NUM );
        x /= COMPRESSED_NUM;
    }
}

BigInt::BigInt ( const BigInt &x ) : data( x.data ) {
}

void BigInt::mul10 ( ) {
    if ( !data.size() )
        return;
    vector< int >::reverse_iterator i;
    *( data.end() - 1 ) *= 10;
    for ( i = data.rbegin() + 1; i != data.rend(); i++ ) {
        *i *= 10;

        *( i - 1 ) += ( *i ) / COMPRESSED_NUM;
        *i %= COMPRESSED_NUM;
    }

    if ( *( data.end() - 1 ) > COMPRESSED_NUM ) {
        data.push_back( *( data.end() - 1 ) / COMPRESSED_NUM );
        *( data.end() - 2 ) %= COMPRESSED_NUM;
    }
}

void BigInt::div10 ( ) {
    if ( !data.size() )
        return;
    vector< int >::reverse_iterator i;
    for ( i = data.rbegin(); i + 1 != data.rend(); i++ ) {
        *( i + 1 ) += ( *i % 10 ) * COMPRESSED_NUM;
        *i /= 10;
    }
    *i /= 10;

    if ( ! *( data.end() - 1 ) )
        data.erase( data.end() - 1 );
}

ostream &operator << ( ostream &os, const BigInt &num ) {
    if ( num.data.size() == 0 ) {
        os << 0;
        return os;
    }
    vector< int >::const_reverse_iterator i = num.data.rbegin();
    if ( *i ) 
        os << *i;
    i++;
    for ( ; i != num.data.rend(); i++ ) {
        os.width( COMPRESSED_BITS );
        os.fill( '0' );
        os << *i;
    }
    return os;
}

BigInt operator + ( const BigInt &a, const BigInt &b ) {
    cout << "+ " << a << " " << b << endl;
    BigInt c;

    vector< int >::const_iterator 
        ai = a.data.begin(),
        bi = b.data.begin();

    while ( ai != a.data.end() || bi != b.data.end() ) {
        c.data.push_back( 
                ( ai != a.data.end() ? *( ai++ ) : 0 )
              + ( bi != b.data.end() ? *( bi++ ) : 0 ) 
            );
    }

    vector< int >::iterator i;
    for ( i = c.data.begin();
            i+1 != c.data.end(); i++ ) {
        ( *( i + 1 ) ) += ( *i ) / COMPRESSED_NUM;
        ( *i ) %= COMPRESSED_NUM;
    }

    if ( ( *i ) >= COMPRESSED_NUM ) {
        c.data.push_back( *i / COMPRESSED_NUM );
        *( c.data.end() - 2 ) %= COMPRESSED_NUM;
    }

    return c;
}

BigInt operator - ( const BigInt &a, const BigInt &b ) {
    cout << "- " << a << " " << b << endl;
    BigInt c;

    if ( a < b ) {
        throw 1;
    }

    for ( vector< int >::const_iterator 
        ai = a.data.begin(),
        bi = b.data.begin();

        ai != a.data.end();
        ) {
        c.data.push_back( *ai++ - ( bi != b.data.end() ? *bi++ : 0 ) );
    }

    for ( vector< int >::iterator i = c.data.begin();
            i + 1 != c.data.end(); i++ ) {
        ( *i ) += COMPRESSED_NUM;
        ( *( i + 1 ) ) --;
        ( *( i + 1 ) ) += ( *i ) / COMPRESSED_NUM;
        ( *i ) %= COMPRESSED_NUM;
    }

    vector< int >::iterator i = c.data.end() - 1;

    while ( c.data.size() && *i == 0 ) {
        i = c.data.erase( i ) - 1;
    }

    return c;
}

BigInt operator * ( const BigInt &a, const BigInt &b ) {
    cout << "* " << a << " " << b << endl;
    BigInt c;
    c.data = vector< int >( a.data.size() * b.data.size() + 1, 0 );

    for ( vector< int >::const_iterator ai = a.data.begin(); 
            ai != a.data.end(); ai++ ) {
        vector< int >::iterator 
            ci = c.data.begin() + ( ai - a.data.begin() );
        for ( vector< int >::const_iterator bi = b.data.begin();
                bi != b.data.end(); bi++, ci++ ) {
            *ci += ( ( *ai ) * ( *bi ) );
            *( ci + 1 ) += ( *ci ) / COMPRESSED_NUM;
            *ci %= COMPRESSED_NUM;
        }
    }

    vector< int >::iterator i = c.data.end() - 1;

    while ( c.data.size() && *i == 0 ) {
        i = c.data.erase( i ) - 1;
    }

    return c;
}

BigInt operator / ( BigInt a, BigInt b ) {
    cout << "/ " << a << " " << b << endl;
    if ( a < b )
        return 0;
    if ( ! b.data.size() )
        throw 1;

    BigInt c;

    int x = 0;

    while ( a >= b ) {
        b.mul10();
        x++;
    }

    b.div10();

    while ( x-- ) {
        while ( a >= b ) {
            a = a - b;
            c = c + 1;
        }
        c.mul10();
        b.div10();
    }

    c.div10();
    return c;
}

BigInt operator % ( BigInt a, BigInt b ) {
    cout << "% " << a << " " << b << endl;
    if ( a < b )
        return a;

    int x = 0;

    while ( a >= b ) {
        b.mul10();
        x++;
    }

    b.div10();

    while ( x-- ) {
        while ( a >= b ) {
            a = a - b;
        }
        b.div10();
    }
    return a;
}

BigInt power ( const BigInt &a, const BigInt &b ) {
    cout << "pow " << a << " " << b << endl;
    if ( b.data.size() == 0 ) {
        return 1;
    }

    if ( b == 1 ) {
        return a;
    }

    BigInt t = power( a, b / 2 );
    if ( b.data[0] % 2 )
        return t * t * a;
    else 
        return t * t;
}

int cmp ( const BigInt &a, const BigInt &b ) {
    if ( a.data.size() != b.data.size() )
        return a.data.size() - b.data.size();

    for ( vector< int >::const_reverse_iterator
        ai = a.data.rbegin(),
        bi = b.data.rbegin();

        ai != a.data.rend();

        ai++, bi++
        ) {

        if ( *ai != *bi ) 
            return *ai - *bi;
    }
    return 0;
}

bool operator < ( const BigInt &a, const BigInt &b ) {
    return cmp( a, b ) < 0;
}

bool operator > ( const BigInt &a, const BigInt &b ) {
    return cmp( a, b ) > 0;
}

bool operator <= ( const BigInt &a, const BigInt &b ) {
    return cmp( a, b ) <= 0;
}

bool operator >= ( const BigInt &a, const BigInt &b ) {
    return cmp( a, b ) >= 0;
}

bool operator == ( const BigInt &a, const BigInt &b ) {
    return cmp( a, b ) == 0;
}

bool operator != ( const BigInt &a, const BigInt &b ) {
    return cmp( a, b ) != 0;
}
