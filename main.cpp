#include <iostream>
#include <string>
#include "BigNum.h"

using std::cout;
using std::cin;
using std::endl;

void testConstruction() {
    cout << "Testing Constructors:" << endl;
    BigNum a;
    cout << "Default constructor: "; a.print();

    BigNum b("-1234567890");
    cout << "String constructor: "; b.print();

    BigNum c(98765);
    cout << "Int constructor: "; c.print();

    BigNum d(b);
    cout << "Copy constructor: "; d.print();

    cout << endl;
}

void testArithmetic() {
    cout << "Testing Arithmetic Operations:" << endl;
    BigNum a("-1111111111111111111111111");
    BigNum b("-1111111111111111111111111");

    cout << "a: "; a.print();
    cout << "b: "; b.print();

    BigNum sum = a.add(b);
    cout << "a + b = "; sum.print();

    // BigNum diff = a.subtract(b);
    // cout << "a - b = "; diff.print();

    // BigNum product = a.multiply(b);
    // cout << "a * b = "; product.print();
    //
    // BigNum quotient = a.div(b);
    // cout << "a / b = "; quotient.print();
    //
    // BigNum remainder = a.mod(b);
    // cout << "a % b = "; remainder.print();

    cout << endl;
}

void testComparison() {
    cout << "Testing Comparison Operations:" << endl;
    BigNum a("1000000000000000000000000");
    BigNum b("-99999999999999999999999999");

    cout << "a: "; a.print();
    cout << "b: "; b.print();

    cout << "a equals b: " << (a.equals(b) ? "true" : "false") << endl;
    cout << "a not equals b: " << (a.notEquals(b) ? "true" : "false") << endl;
    cout << "a less than b: " << (a.lessThan(b) ? "true" : "false") << endl;
    cout << "a greater than b: " << (a.greaterThan(b) ? "true" : "false") << endl;

    cout << endl;
}

void testFileIO() {
    cout << "Testing File I/O:" << endl;
    BigNum a("-9876543210987654321098765432109876543210");
    cout << "Original number: "; a.print();

    a.printToFile("bignum_output.txt");
    cout << "Number written to file." << endl;

    BigNum b;
    b.inputFromFile("bignum_output.txt");
    cout << "Number read from file: "; b.print();

    cout << endl;
}

int main() {
    // testConstruction();
    testArithmetic();
    // testComparison();
    // testFileIO();

    return 0;
}