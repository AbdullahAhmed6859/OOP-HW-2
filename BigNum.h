#ifndef BIG_NUM_H
#define BIG_NUM_H

#include <cstdint>
#include <string>
#include <vector>
using std::string;
using std::vector;

class BigNum {
    vector<uint8_t> digits;
    bool isNegative;

public:

    // Constructors and Destructor
    BigNum();                      // default initialization value should be '0'
    BigNum(const BigNum& bigNum);  // copy constructor
    BigNum(const string& bigStr);  // bigStr is a string e.g. �348723482�
    BigNum(const int num);         // num is an integer e.g. 487234985
    ~BigNum();                     // delete dynamic array, if you using dynamic array

    // Input/Output Operations
    void input();                                // clears current number and sets the number input from user.
                                                 // see clear() helper method
    void print();                                // prints the number to screen. (comma separate after 3-digit
                                                 // groups e.g. 12,345,678)
    void inputFromFile(const string& fileName);  // clears current number and sets the number
                                                 // given in file. see clear() helper method
    void printToFile(const string& fileName);    // prints the number to file

    // Initialization/Assignment Operations
    void copy(const BigNum& bigNum);
    void operator=(const BigNum& bigNum);
    void zerofy();  // initializes the number to 0

    // Arithmetic Operations: Addition
    void   increment();                        // increments the whole number by 1 (not every digit)
    BigNum add(const BigNum& bigNum);          // like c = a+b; e.g. c = a.add(b);
    BigNum add(const int num);                 // like c = a+2; e.g. c = a.add(2);
    void   compoundAdd(const BigNum& bigNum);  // like a+=b; e.g. a.compoundAdd(b);
    void   compoundAdd(const int num);         // like a+=2; e.g. a.compoundAdd(2);

    // Arithmetic Operations: Subtraction
    void   decrement();                             // decrements the whole number by 1 (not every digit)
    BigNum subtract(const BigNum& bigNum);          // like c = a-b; e.g. c = a.subtract(b);
    BigNum subtract(const int num);                 // like c = a-2; e.g. c = a.subtract(2);
    void   compoundSubtract(const BigNum& bigNum);  // like a-=b; e.g. a.compoundSubtract(b);
    void   compoundSubtract(const int num);         // like a-=2; e.g. a.compoundSubtract(2);

    // Arithmetic Operations: Multiplication and Division
    BigNum multiply(const BigNum& bigNum);  // like c = a*b; e.g. c = a.multiply(b);
    BigNum div(const BigNum& bigNum);       // integer division (/) operator
    BigNum mod(const BigNum& bigNum);       // remainder (%) operator

    // Comparison Operations:
    bool equals(const BigNum& bigNum);
    bool notEquals(const BigNum& bigNum);
    bool lessThan(const BigNum& bigNum);
    bool greaterThan(const BigNum& bigNum);

private:

    void clear();  // equivalent to deleting current dynamic array.
    void parseString(const string& bigStr);
};

#endif //BIG_NUM_H
