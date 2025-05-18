#ifndef BIG_NUM_H
#define BIG_NUM_H

#include <cstdint>
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::pair;

class BigNum {
    vector<uint8_t> digits;
    bool isNegative;

public:
    // Constructors and Destructor
    BigNum();

    BigNum(const BigNum &bigNum);

    BigNum(const string &bigStr);

    BigNum(const int num);

    ~BigNum();

    // Input/Output Operations
    void input();

    void print();

    void inputFromFile(const string &fileName);

    void printToFile(const string &fileName);

    void copy(const BigNum &bigNum);

    void operator=(const BigNum &bigNum);

    void zerofy();

    // Arithmetic Operations: Addition
    void increment();

    BigNum add(const BigNum &bigNum);

    BigNum add(const int num);

    void compoundAdd(const BigNum &bigNum);

    void compoundAdd(const int num);

    // Arithmetic Operations: Subtraction
    void decrement();

    BigNum subtract(const BigNum &bigNum);

    BigNum subtract(const int num);

    void compoundSubtract(const BigNum &bigNum);

    void compoundSubtract(const int num);

    // Arithmetic Operations: Multiplication and Division
    BigNum multiply(const BigNum &bigNum); // like c = a*b; e.g. c = a.multiply(b);
    BigNum div(const BigNum &bigNum); // integer division (/) operator
    BigNum mod(const BigNum &bigNum); // remainder (%) operator

    // Comparison Operations:
    bool equals(const BigNum &bigNum);

    bool notEquals(const BigNum &bigNum);

    bool lessThan(const BigNum &bigNum);

    bool greaterThan(const BigNum &bigNum);

private:
    void clear();

    void parseString(const string &bigStr);

    void addMagnitude(const BigNum &bigNum);

    void subMagnitude(const BigNum &bigNum);

    bool isGreaterInMagnitude(const BigNum &bigNum) const;

    bool isLesserInMagnitude(const BigNum &bigNum) const;

    bool isEqualInMagnitude(const BigNum &bigNum) const;

    static bool isValidNumStr(const string &bigStr);

    void removeLeadingZeros();

    bool isZero() const;

    BigNum absolute() const;

    BigNum leftShift() const;

    pair<BigNum, BigNum> divAndMod(const BigNum &divisor) const;
};

#endif //BIG_NUM_H
