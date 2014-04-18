#include "bigint.h"
#include <iostream>

using namespace std;

int main() {
    BigInt a, b;
    cout << "Please input operand a:";
    cin >> a;
    cout << "Please input operand b:";
    cin >> b;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl;
    cout << "a \% b = " << a % b << endl;
    cout << "a ^ b = " << power(a, b) << endl;
    return 0;
}
