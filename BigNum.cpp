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
        const uint8_t digit = (numCopy % 10);
        digits.push_back(digit);
        numCopy /= 10;
    } while (numCopy > 0);
}

BigNum::~BigNum() = default;


// INPUT & OUTPUT OPERATIONS
void BigNum::input() {
    string num;
    cin >> num;
    clear();
    // incomplete
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
    if (!inputFile) {
        std::cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    getline(inputFile, line);
    if (!line.empty() && line.back() == '\n') {
        line.erase(line.end() - 1);
    }
    parseString(line);
    inputFile.close();
}

void BigNum::printToFile(const string &fileName) {
    ofstream outputFile(fileName);

    // Check if the file opened successfully
    if (!outputFile) {
        std::cerr << "Error opening file for writing!" << endl;
        return;
    }

    if (isNegative) outputFile << '-';
    const size_t len = digits.size();
    for (size_t i=0; i < len-1; i++)
        outputFile << static_cast<char>(digits[len-i-1] + '0');


    outputFile.close();
}

void BigNum::copy(const BigNum& bigNum) {
    if (this ==  &bigNum) return;
    digits = bigNum.digits;
    isNegative = bigNum.isNegative;
}

void BigNum::operator=(const BigNum &bigNum) {
    if (this ==  &bigNum) return;
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
    int carry = 1;
    for (size_t i=0; i<digits.size(); i++) {
        const int sum = digits[i] + carry;
        digits[i] = sum % 10;
        carry = sum / 10;
        if (!carry) return;
    }
    digits.push_back(carry);
}

BigNum BigNum::add(const BigNum &bigNum) {
    BigNum result;
    if (isNegative != bigNum.isNegative) {
        return {};
    }

    result.isNegative = bigNum.isNegative;
    const size_t newSize = max(digits.size(), bigNum.digits.size());
    result.digits.assign(newSize, 0);

    int carry = 0;
    for (size_t i=0; i<result.digits.size(); i++) {
        const uint8_t d1 = digits.at(i) ? digits.at(i) : 0;
        const uint8_t d2 = bigNum.digits.at(i) ? bigNum.digits.at(i) : 0;
        const uint8_t sum = d1 + d2 + carry;
        result.digits.at(i) = sum % 10;
        carry = sum / 10;
    }
    if (carry) result.digits.push_back(carry);
    return result;
}

BigNum BigNum::add(int num) {
    BigNum result;
}

void BigNum::compoundAdd(const BigNum &bigNum) {}

void BigNum::compoundAdd(const int num) {}

void BigNum::decrement() {}

BigNum BigNum::subtract(const BigNum &bigNum) {}

BigNum BigNum::subtract(int num) {}

void BigNum::compoundSubtract(const BigNum &bigNum) {}

void BigNum::compoundSubtract(const int num) {}

BigNum BigNum::multiply(const BigNum &bigNum) {}

BigNum BigNum::div(const BigNum &bigNum) {}

BigNum BigNum::mod(const BigNum &bigNum) {}

// COMPARISON OPERATIONS
bool BigNum::equals(const BigNum &bigNum) {
    if (this == &bigNum) return true;
    if (digits.size() != bigNum.digits.size()) return false;

    for (size_t i=0; i<digits.size(); i++)
        if (digits[i] != bigNum.digits[i]) return false;

    return true;
}

bool BigNum::notEquals(const BigNum &bigNum) {
    return !equals(bigNum);
}

bool BigNum::lessThan(const BigNum &bigNum) {
    if (this == &bigNum) return false;
    if (!isNegative && bigNum.isNegative) return false;
    if (isNegative && !bigNum.isNegative) return true;
    if (digits.size() > bigNum.digits.size()) return isNegative;
    if (digits.size() < bigNum.digits.size()) return !isNegative;

    bool result = false;
    for (size_t i=0; i<digits.size(); i++)
        if (digits[i] < bigNum.digits[i]) {
            result = true;
            break;
        }

    return isNegative ? !result : result;
}

bool BigNum::greaterThan(const BigNum &bigNum) {
    if (this == &bigNum) return false;
    if (isNegative && !bigNum.isNegative) return false;
    if (!isNegative && bigNum.isNegative) return true;
    if (digits.size() < bigNum.digits.size()) return isNegative;
    if (digits.size() > bigNum.digits.size()) return !isNegative;

    bool result = false;
    for (size_t i=0; i<digits.size(); i++)
        if (digits[i] > bigNum.digits[i]) {
            result = true;
            break;
        }

    return isNegative ? !result : result;
}

// BigNum& BigNum::intToBigNum(int num) {
//     isNegative = num < 0;
//     do {
//         uint8_t digit = num % 10;
//         digits.push_back(digit);
//         num /= 10;
//     } while (num > 0);
// }

void BigNum::parseString(const string &bigStr) {
    int minIndex;
    if (bigStr[0] == '-') {
        isNegative = true;
        minIndex = 1;
    } else {
        isNegative = false;
        minIndex = 0;
    }

    const size_t len = bigStr.length();
    for (size_t i=0; i < len - minIndex; i++)
        digits.push_back(bigStr[len - i - 1] - '0');
}




