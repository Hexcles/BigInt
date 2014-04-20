/*
 * Diffie-Hellman Key Agreement Method
 * A simple example of the usage of BigInt
 * Implemented roughly on RFC 2631
 *
 * For simplicity, assume:
 * 1. the length of Q (m) is 160 (at least 160)
 *    (defines the length of private key, x)
 * 2. the length of P (L) is 1024 (at least 512)
 *    (defines the length of final shared secret, ZZ)
 * 3. KM = ZZ and RC2-128 encryption is used
 *    (so that ZZ is directly the symmetric key for encryption)
 *
 * DEMO only!
 */
#include "../bigint.h"
#include "sha1.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define m 160
#define L 1024

using namespace std;

// C++11
// No need to test 2 because P, Q must be odd.
const vector<int> prime_sieve = {
    3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109,
    113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
    307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367,
    373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
    439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499,
    503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577,
    587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643,
    647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719,
    727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
    809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
    877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947,
    953, 967, 971, 977, 983, 991, 997, 1009, 1013,
    1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
    1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
    1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
    1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291,
    1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373,
    1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451,
    1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
    1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583,
    1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
    1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733,
    1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811,
    1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
    1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
    1993, 1997, 1999, 2003, 2011, 2017
};
// Fermat tester (should be enough)
const vector<int> fermat_tester = {2, 3, 5, 7, 11, 17};

// A 160-bit char string with the ending 0
// 160 = 20 * 8 (char)  = 5 * 4 * 8 (int)
unsigned char SEED[20];

BigInt P, Q, G, Xa, Xb, Ya, Yb, ZZ;

void rand160(unsigned char bits[20]) {
    for (int i = 0; i < 5; i++) {
        // Extract the small 16 bits from rand() and put it into temp
        unsigned int t = rand();
        if ( rand() & 1 ) t <<= 1;
        memcpy(bits + i * 4, &t, 4);
    }
}

string hex160(const unsigned char bits[20]) {
    stringstream ss;
    for (int i = 0; i < 5; i++) {
        unsigned int temp = 0;
        memcpy(&temp, bits + i * 4, 4);
        ss << hex << setw(8) << setfill('0') << temp;
    }
    return ss.str(); 
}

BigInt bits160_to_bigint(const unsigned char bits[20]) {
    BigInt res(0);
    BigInt shift(256);
    for (int i = 19; i >= 0; i--) {
        res = res * shift + bits[i];
    }
    return res;
}

void bigint_to_bits160(BigInt num, unsigned char bits[20]) {
    BigInt shift(256);
    for (int i = 0; i < 20; i++) {
        if ( num == 0 ) {
            bits[i] = 0;
        } else {
            bits[i] = (num % shift).to_uchar();
            num = num / shift;
        }
    }
}

bool fermat_test(BigInt p, int a) {
    BigInt ans = power(a, p - 1, p);
    return ans == 1;
}

bool is_prime(BigInt num) {
    for (auto i = prime_sieve.begin(); i!= prime_sieve.end(); i++){
        if (num % *i == 0) return false;
    }
    for (auto i = fermat_tester.begin(); i!= fermat_tester.end(); i++){
        if (!fermat_test(num, *i)) return false;
    }
    return true;
}

void generate_pq() {
    const int m2 = ceil(m / 160.0);
    const int L2 = ceil(L / 160.0);
    const int N = ceil(L / 1024.0);
    cerr << "Start generating p, q..." << endl;
    unsigned char u[20], buf[20];
    int counter = 0;
    do{
        cerr << "[Q] Round " << counter++ << endl;
        rand160(SEED);
        cerr << "[Q] SEED = " << hex160(SEED) << endl;
        // RFC: for m = 160, U = SHA1[SEED] XOR SHA1[(SEED+1) mod 2^160 ]
        // For simplicity, just use sha1(SEED)
        sha1::calc(SEED, 20, u);
        // According to RFC, make MSB & LSB 1 (thus it must be odd!)
        u[0] |= 1;
        u[19] |= 128;
        Q = bits160_to_bigint(u);
        cerr << "[Q] Q = " << Q << endl;
    } while( !is_prime(Q) );

    counter = 0;
    BigInt seed = bits160_to_bigint(SEED);
    BigInt R, V, shift;
    BigInt two160(2), two1023(2), two1024(2);
    two160 = power(two160, 160);
    two1023 = power(two1024, 1023);
    two1024 = power(two1024, 1024);
    do {
        if (counter >= 4096 * N) {
            cerr << "Cannot find a prime P." << endl;
            exit(1);
        }
        cerr << "[P] Round " << counter << endl;
        R = seed + m2 * 2 + L2 * counter++;
        V = 0;
        shift = 1;
        for (int i = 0; i < L2; i++) {
            bigint_to_bits160(R + i, buf);
            sha1::calc(buf, 20, u);
            V = V + bits160_to_bigint(u) * shift;
            shift = shift * two160;
        }
        V = V % two1024;
        if ( V < two1023 ) V = V + two1023;     // V |= 2^(L-1)
        cerr << "[P] V = " << V << endl;
        P = V - (V % (Q * 2)) + 1;
        cerr << "[P] P = " << P << endl;
        if ( P <= two1023 ) continue;
    } while ( !is_prime(P) );
}

void generate_g() {
    int counter = 0;
    BigInt j = (P - 1) / Q;
    do {
        cerr << "[G] Round " << counter++ << endl;
        BigInt h(rand());
        G = power(h, j, P);
        cerr << "[G] G = " << G << endl;
    } while (G == 1);
}

void alice_and_bob() {
    unsigned char xa[20], xb[20];
    rand160(xa);
    rand160(xb);
    Xa = bits160_to_bigint(xa);
    Xb = bits160_to_bigint(xb);
    Ya = power(G, Xa, P);
    Yb = power(G, Xb, P);
    ZZ = power(Ya, Xb, P);
}

int main() {
    // seed the random generator
    srand(time(0));

    // parameter generation
    generate_pq();      // RFC2631 2.2.1.1
    generate_g();       // RFC2631 2.2.1.2
    // shared key negotiation
    alice_and_bob();

    // output
    cout << endl;
    cout << "(public) P = " << P << endl;
    cout << "(public) G = " << G << endl;
    cout << "(A's private) Xa = " << Xa << endl;
    cout << "(A's public) Ya = " << Ya << endl;
    cout << "(B's private) Xb = " << Xb << endl;
    cout << "(B's public) Yb = " << Yb << endl;
    cout << "Finally they get the same private shared key: " << endl;
    cout << "ZZ = " << ZZ << endl;
    return 0;
}
