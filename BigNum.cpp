// author: Abdullah Ahmed
// authorID: aa09303
// section: L1
#include "BigNum.h"
#include <fstream>
#include <iostream>
using std::cin;
using std::cout;
using std::max;
using std::endl;
using std::getline;
using std::ifstream;
using std::ofstream;
using std::cerr;
using std::pair;


// CONSTRUCTORS & DESTRUCTORS
BigNum::BigNum() : digits({0}), isNegative(false) {}

BigNum::BigNum(const BigNum &bigNum) = default;

BigNum::BigNum(const string &bigStr) {
    parseString(bigStr);
}

BigNum::BigNum(const int num) {
    isNegative = num < 0;
    int numCopy = abs(num);
    do {
        const uint8_t digit = numCopy % 10;
        digits.push_back(digit);
        numCopy /= 10;
    } while (numCopy > 0);
}

BigNum::~BigNum() {
    clear();
}

// INPUT & OUTPUT OPERATIONS
void BigNum::input() {
    string num;
    cin >> num;
    parseString(num);
}

void BigNum::print() {
    if (isNegative)
        cout << '-';

    if (digits.empty())
        cout << '0';
    else
        for (size_t i = digits.size(); i-- > 0;) {
            cout << static_cast<char>(digits[i] + '0');
            if (i % 3 == 0 && i != 0) cout << ','; // add commas for formatting
        }
    cout << endl;
}

void BigNum::inputFromFile(const string &fileName) {
    ifstream inputFile(fileName);
    if (!inputFile)
        cerr << "Error opening file: " + fileName + " for read" << endl;

    string line;
    getline(inputFile, line);

    if (!line.empty() && line.back() == '\n')  // remove '\n' if exists
        line.erase(line.end() - 1);

    inputFile.close();
    parseString(line);
}

void BigNum::printToFile(const string &fileName) {
    ofstream outputFile(fileName);

    if (!outputFile) {
        cerr << "Error opening file: " + fileName + " for write" << endl;
        return;
    }

    if (isNegative) outputFile << '-';
    const size_t len = digits.size();
    for (size_t i = 0; i < len; ++i)
        outputFile << static_cast<char>(digits[len - i - 1] + '0'); // add chars to file in reverse order

    outputFile.close();
}

void BigNum::copy(const BigNum &bigNum) {
    if (this == &bigNum) return;
    digits = bigNum.digits;
    isNegative = bigNum.isNegative;
}

void BigNum::operator=(const BigNum &bigNum) {
    copy(bigNum);
}

void BigNum::zerofy() {
    clear();
    digits.push_back(0);
}

void BigNum::clear() {
    digits.clear();
    digits.shrink_to_fit();
}

// ARITHMETIC OPERATIONS
void BigNum::increment() {
    compoundAdd(BigNum(1));
}

BigNum BigNum::add(const BigNum &bigNum) {
    BigNum result(*this);
    result.compoundAdd(bigNum);
    return result;
}

BigNum BigNum::add(const int num) {
    BigNum result(*this);
    result.compoundAdd(num);
    return result;
}

void BigNum::compoundAdd(const int num) {
    compoundAdd(BigNum(num));
}

void BigNum::compoundAdd(const BigNum &bigNum) {
    if (isNegative != bigNum.isNegative) { // add a positive num to a negative num
        subMagnitude(bigNum);
    } else
        addMagnitude(bigNum); // add 2 positive or 2 negative numbers
}

void BigNum::decrement() {
    compoundSubtract(BigNum(1));
}

BigNum BigNum::subtract(const BigNum &bigNum) {
    BigNum result(*this);
    result.compoundSubtract(bigNum);
    return result;
}

BigNum BigNum::subtract(const int num) {
    BigNum result(*this);
    result.compoundSubtract(num);
    return result;
}

void BigNum::compoundSubtract(const BigNum &bigNum) {
    if (isNegative != bigNum.isNegative)
        addMagnitude(bigNum); //
    else
        subMagnitude(bigNum);
}

void BigNum::compoundSubtract(const int num) {
    compoundSubtract(BigNum(num));
}

BigNum BigNum::multiply(const BigNum& bigNum) {
    if (isZero() || bigNum.isZero()) // if either is 0 return 0
        return {};

    BigNum result;
    // product of n and m can have max n+m digits
    result.digits.resize(digits.size() + bigNum.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < bigNum.digits.size() || carry; ++j) {
            const int curVal = result.digits[i + j] // prev value of result[i+j]
            + digits[i] * (j < bigNum.digits.size() ? bigNum.digits[j] : 0) // product of digits of both nums
            + carry;
            result.digits[i + j] = curVal % 10; // digit cannot be more than 10
            carry = curVal / 10;
        }
    }

    result.isNegative = isNegative ^ bigNum.isNegative; // same sign +ve, opp sign -ve
    result.removeLeadingZeros();
    return result;
}

BigNum BigNum::div(const BigNum &bigNum) {
    return divAndMod(bigNum).first;
}

BigNum BigNum::mod(const BigNum &bigNum) {
    BigNum remainder = divAndMod(bigNum).second;
    if (remainder.isNegative) {
        cerr << "Cannot calculate remainder from negative numbers" << endl;
        return {};
    }
    return remainder;
}
// COMPARISON OPERATIONS
bool BigNum::equals(const BigNum &bigNum) {
    if (isNegative != bigNum.isNegative) return false;
    return isEqualInMagnitude(bigNum);
}

bool BigNum::notEquals(const BigNum &bigNum) {
    return !equals(bigNum);
}

bool BigNum::lessThan(const BigNum &bigNum) {
    if (this == &bigNum) return false;
    if (!isNegative && bigNum.isNegative) return false;
    if (isNegative && !bigNum.isNegative) return true;
    // check magnitude because now signs are same
    return isLesserInMagnitude(bigNum) ^ isNegative;
}

bool BigNum::greaterThan(const BigNum &bigNum) {
    if (this == &bigNum) return false;
    if (isNegative && !bigNum.isNegative) return false;
    if (!isNegative && bigNum.isNegative) return true;
    // check magnitude because now signs are same
    return isGreaterInMagnitude(bigNum) ^ isNegative;
}

void BigNum::parseString(const string &bigStr) {
    if (!isValidNumStr(bigStr)) {
        cerr << bigStr + " is not a valid num string" << endl;
        return;
    }

    short minIndex;
    if (bigStr[0] == '-') {
        isNegative = true;
        minIndex = 1; // start from index 1 if the first char is '-'
    } else {
        isNegative = false;
        minIndex = 0;
    }

    clear(); // empty the vector
    const size_t len = bigStr.length();
    digits.resize(len - minIndex, 0); // make space accordingly

    for (size_t i = 0; i < len - minIndex; ++i)
        digits[i] = bigStr[len - i - 1] - '0'; // add digits in reverse order

    removeLeadingZeros();
    if (isZero()) isNegative = false;
}

void BigNum::addMagnitude(const BigNum &bigNum) {
    const size_t thisSize = digits.size();
    const size_t otherSize = bigNum.digits.size();

    // make space if the new num is greater is size
    if (otherSize > thisSize) {
        digits.resize(otherSize, 0);
    }

    // add and propagate carry
    int carry = 0;
    size_t i = 0;
    for (; i < otherSize; ++i) {
        const int sum = digits[i] + bigNum.digits[i] + carry;
        digits[i] = sum % 10;
        carry = sum / 10;
    }
    // propagate carry until carry = 0
    for (; i < digits.size() && carry; ++i) {
        const int sum = digits[i] + carry;
        digits[i] = sum % 10;
        carry = sum / 10;
    }

    // if still carry then add a new digit
    if (carry)
        digits.push_back(carry);
}

void BigNum::subMagnitude(const BigNum &bigNum) {
    if (equals(bigNum)) {
        isNegative = false;
        zerofy();
        return;
    }
    // assign larger and smaller
    const BigNum &larger = isLesserInMagnitude(bigNum) ? bigNum : *this;
    const BigNum &smaller = isLesserInMagnitude(bigNum) ? *this : bigNum;

    isNegative = lessThan(bigNum);
    digits.resize(larger.digits.size(), 0);
    int borrow = 0;
    size_t i;

    for (i = 0; i < smaller.digits.size(); ++i) {
        int diff = larger.digits[i] - smaller.digits[i] - borrow;
        if (diff < 0) { // determine the borrow
            diff += 10;
            borrow = 1;
        } else 
            borrow = 0;

        digits[i] = static_cast<uint8_t>(diff);
    }

    // remaining digits of the larger number
    for (; i < larger.digits.size(); ++i) {
        int diff = larger.digits[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else
            borrow = 0;
        digits[i] = static_cast<uint8_t>(diff);
    }

    removeLeadingZeros();
}

bool BigNum::isGreaterInMagnitude(const BigNum &bigNum) const {
    if (digits.size() < bigNum.digits.size()) return false;
    if (digits.size() > bigNum.digits.size()) return true;

    // loop from the highest place value
    for (size_t i = digits.size(); i-- > 0;) {
        if (digits[i] > bigNum.digits[i])
            return true;
        if (digits[i] < bigNum.digits[i])
            return false;
    }
    return false;
}

bool BigNum::isLesserInMagnitude(const BigNum &bigNum) const {
    if (digits.size() > bigNum.digits.size()) return false;
    if (digits.size() < bigNum.digits.size()) return true;

    // loop from the highest place value
    for (size_t i = digits.size(); i-- > 0;) {
        if (digits[i] < bigNum.digits[i])
            return true;
        if (digits[i] > bigNum.digits[i])
            return false;
    }
    return false;
}

bool BigNum::isEqualInMagnitude(const BigNum &bigNum) const {
    if (this == &bigNum) return true;
    if (digits.size() != bigNum.digits.size()) return false;

    // loop over the numbers
    for (size_t i = 0; i < digits.size(); ++i)
        if (digits[i] != bigNum.digits[i])
            return false;

    return true;
}


bool BigNum::isValidNumStr(const string &bigStr) {
    const size_t len = bigStr.length();
    if (len == 0) return false;
    const short minIndex = bigStr[0] == '-'; // minIndex = 1 if num is negative

    if (minIndex && len < 2) return false; // if first char is '-' but there are no chars after it

    for (size_t i=minIndex; i < len; ++i) {
        if (const char num = bigStr[i]; num < '0' || num > '9') // if not a valid char
            return false;
    }
    return true;
}

void BigNum::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
}

bool BigNum::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}

BigNum BigNum::absolute() const {
    // returns a positive copy of the object
    BigNum result = *this;
    result.isNegative = false;
    return result;
}

BigNum BigNum::leftShift() const {
    BigNum result;
    if (isZero()) return result; // no need to shift if its already 0

    result.isNegative = isNegative;
    result.digits.resize(digits.size() + 1, 0); // make space with size + 1

    for (size_t i = 0; i < digits.size(); ++i)
        result.digits[i+1] = digits[i]; // copy the digits one place ahead

    return result;
}

pair<BigNum, BigNum> BigNum::divAndMod(const BigNum &divisor) const {
    if (divisor.isZero()) {
        cerr << "Error: Division by zero." << endl;
        return {{}, {}};
    }

    BigNum quotient;
    BigNum dividend = this->absolute();
    const BigNum absDivisor = divisor.absolute();
    BigNum curDividend;

    for (int i = digits.size() - 1; i >= 0; --i) {
        // curDividend * 10 + digits[i]
        curDividend = curDividend.leftShift();
        curDividend.digits[0] = digits[i];
        curDividend.removeLeadingZeros();

        int count = 0;
        // curDividend >= absDivisor
        while (curDividend.isGreaterInMagnitude(absDivisor) || curDividend.equals(absDivisor)) {
            curDividend.compoundSubtract(absDivisor);
            count++;
        }
        // after the above loop
        // count = curDividend / absDivisor
        // curDividend equals curDividend % absDivisor

        quotient = quotient.leftShift(); // curDivisor * 10
        quotient.digits[0] = count; // curDivisor + count
    }

    quotient.removeLeadingZeros();
    quotient.isNegative = isNegative ^ divisor.isNegative;
    if (quotient.isZero()) {
        quotient.isNegative = false;
    }

    // if either number is -ve the mod function will handle this
    curDividend.isNegative = isNegative || divisor.isNegative;
    return {quotient, curDividend};
}

