#include "bigint.h"
#include <iostream>

using namespace std;

int main() {
    BigInt a, b, c;
    cout << "Please input operand a:";
    cin >> a;
    cout << "Please input operand b:";
    cin >> b;
    cout << "Please input operand c:";
    cin >> c;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl;
    cout << "a \% b = " << a % b << endl;
    if ( b < 100000 ) cout << "a ^ b = " << power(a, b) << endl;
    cout << "a ^ b (mod c) = " << power(a, b, c) << endl;
    return 0;
}
