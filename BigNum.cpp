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

BigNum::~BigNum() = default;


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
            if (i % 3 == 0 && i != 0) cout << ',';
        }
    cout << endl;
}

void BigNum::inputFromFile(const string &fileName) {
    ifstream inputFile(fileName);
    if (!inputFile)
        cerr << "Error opening file: " + fileName + " for read" << endl;


    string line;
    getline(inputFile, line);
    if (!line.empty() && line.back() == '\n') {
        line.erase(line.end() - 1);
    }
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
        outputFile << static_cast<char>(digits[len - i - 1] + '0');

    outputFile.close();
}

void BigNum::copy(const BigNum &bigNum) {
    if (this == &bigNum) return;
    digits = bigNum.digits;
    isNegative = bigNum.isNegative;
}

void BigNum::operator=(const BigNum &bigNum) {
    if (this == &bigNum) return;
    digits = bigNum.digits;
    isNegative = bigNum.isNegative;
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
    if (isNegative != bigNum.isNegative) {
        subMagnitude(bigNum);
    } else
        addMagnitude(bigNum);
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
        addMagnitude(bigNum);
    else
        subMagnitude(bigNum);
}

void BigNum::compoundSubtract(const int num) {
    compoundSubtract(BigNum(num));
}

BigNum BigNum::multiply(const BigNum& bigNum) {
    if (isZero() || bigNum.isZero())
        return {};

    BigNum result;
    result.digits.resize(digits.size() + bigNum.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < bigNum.digits.size() || carry; ++j) {
            const int current = result.digits[i + j]
            + digits[i] * (j < bigNum.digits.size() ? bigNum.digits[j] : 0)
            + carry;
            result.digits[i + j] = current % 10;
            carry = current / 10;
        }
    }

    result.isNegative = isNegative ^ bigNum.isNegative;
    result.removeLeadingZeros();
    return result;
}

BigNum BigNum::div(const BigNum &bigNum) {
    return divAndMod(bigNum).first;
}

BigNum BigNum::mod(const BigNum &bigNum) {
    return divAndMod(bigNum).second;
}
// COMPARISON OPERATIONS
bool BigNum::equals(const BigNum &bigNum) {
    if (this == &bigNum) return true;
    if (isNegative != bigNum.isNegative) return false;
    if (digits.size() != bigNum.digits.size()) return false;

    for (size_t i = 0; i < digits.size(); ++i)
        if (digits[i] != bigNum.digits[i])
            return false;

    return true;
}

bool BigNum::notEquals(const BigNum &bigNum) {
    return !equals(bigNum);
}

bool BigNum::lessThan(const BigNum &bigNum) {
    if (this == &bigNum) return false;
    if (!isNegative && bigNum.isNegative) return false;
    if (isNegative && !bigNum.isNegative) return true;
    return isLesserInMagnitude(bigNum) ^ isNegative;
}

bool BigNum::greaterThan(const BigNum &bigNum) {
    if (this == &bigNum) return false;
    if (isNegative && !bigNum.isNegative) return false;
    if (!isNegative && bigNum.isNegative) return true;
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
        minIndex = 1;
    } else {
        isNegative = false;
        minIndex = 0;
    }

    clear();
    const size_t len = bigStr.length();
    digits.resize(len - minIndex, 0);

    for (size_t i = 0; i < len - minIndex; ++i)
        digits[i] = bigStr[len - i - 1] - '0';

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

    // add and propagae carry
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

    const BigNum &larger = isLesserInMagnitude(bigNum) ? bigNum : *this;
    const BigNum &smaller = isLesserInMagnitude(bigNum) ? *this : bigNum;

    isNegative = lessThan(bigNum);
    digits.resize(larger.digits.size(), 0);
    int borrow = 0;
    size_t i;

    for (i = 0; i < smaller.digits.size(); ++i) {
        int diff = larger.digits[i] - smaller.digits[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else 
            borrow = 0;

        digits[i] = static_cast<uint8_t>(diff);
    }

    // Process remaining digits of the larger number
    for (; i < larger.digits.size(); ++i) {
        int diff = larger.digits[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else
            borrow = 0;
        digits[i] = static_cast<uint8_t>(diff);
    }

    // Remove leading zeros
    removeLeadingZeros();
}

bool BigNum::isGreaterInMagnitude(const BigNum &bigNum) const {
    if (digits.size() < bigNum.digits.size()) return false;
    if (digits.size() > bigNum.digits.size()) return true;

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

    for (size_t i = digits.size(); i-- > 0;) {
        if (digits[i] < bigNum.digits[i])
            return true;
        if (digits[i] > bigNum.digits[i])
            return false;
    }
    return false;
}

bool BigNum::isValidNumStr(const string &bigStr) {
    const size_t len = bigStr.length();
    if (len == 0) return false;
    const short minIndex = bigStr[0] == '-';

    if (minIndex && len < 2) return false;

    for (size_t i=minIndex; i < len; ++i) {
        if (const char num = bigStr[i]; num < '0' || num > '9')
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
    BigNum result = *this;
    result.isNegative = false;
    return result;
}

BigNum BigNum::leftShift() const {
    BigNum result;
    if (isZero()) return result;
    result.digits.resize(digits.size() + 1, 0);

    for (size_t i = 0; i < digits.size(); ++i)
        result.digits[i+1] = digits[i];

    return result;
}

pair<BigNum, BigNum> BigNum::divAndMod(const BigNum &divisor) const {
    if (divisor.isZero()) {
        cerr << "Error: Division by zero." << endl;
        return {{}, {}};
    }

    BigNum quotient;
    quotient.isNegative = (isNegative != divisor.isNegative);

    BigNum dividend = absolute();
    BigNum absDivisor = divisor.absolute();
    BigNum current;

    for (int i = digits.size() - 1; i >= 0; --i) {
        current = current.leftShift();
        current.digits[0] = digits[i];
        current.removeLeadingZeros();

        int count = 0;
        while (current.isGreaterInMagnitude(absDivisor) || current.equals(absDivisor)) {
            current = current.subtract(absDivisor);
            count++;
        }

        quotient.digits.insert(quotient.digits.begin(), count);
    }

    quotient.removeLeadingZeros();
    if (quotient.isZero()) {
        quotient.isNegative = false;
    }

    current.isNegative = this->isNegative;
    return {quotient, current};
}


