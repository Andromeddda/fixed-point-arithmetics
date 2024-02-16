#include "test_system.hpp"
#include "long-number.hpp"
#include "utils.hpp"
#include <vector>

using namespace TestSystem;
using namespace std;

// TEST CONSTRUCTORS
bool test_empty_constructor() {
	try {
		LongNumber x = LongNumber();
	}
	catch (const std::exception& exc) {
        return FAIL;
    }
    return OK;
}

bool test_copy_constructor() {			
	LongNumber x = -1234.567_ln;
	LongNumber y = LongNumber(x);

	return (y == x);
}

bool test_string_constructor() {
	LongNumber x = LongNumber("-1234.567");
	vector<digit_t> d = vector<digit_t>(PRECISION + 4, 0);
	for (int i = 0; i < 7; i++) d[i] = i+1;
	LongNumber y = LongNumber(d, false);

	return (x == y);
}

bool test_double_constructor() {		
	LongNumber x = LongNumber(-1234.567);
	vector<digit_t> d = vector<digit_t>(PRECISION + 4, 0);
	for (int i = 0; i < 7; i++) d[i] = i+1;

	LongNumber y = LongNumber(d, false);

	return (x == y);

}

bool test_ud_suffix_constructor() {			
	LongNumber x = -1234.567_ln;
	vector<digit_t> d = vector<digit_t>(PRECISION + 4, 0);
	for (int i = 0; i < 7; i++) d[i] = i+1;

	LongNumber y = LongNumber(d, false);

	return (x == y);
}

// TEST ToString
bool test_ToString() {
	LongNumber x = -1234.56701_ln;
	string str = "-1234.56701";

	return (x.ToString() == str);
}

// TEST LOGIC OPERATORS
bool test_equality() {
	LongNumber x = -1234.567_ln;
	LongNumber y = LongNumber("-1234.567");

	return x == y;
}

bool test_comparison() {
	LongNumber x = -1234.567_ln;
	LongNumber y = -1234.568_ln;

	return x > y;
}

// TEST NUMERIC OPERATORS
bool test_unary_minus() {
	LongNumber x = -1234.567_ln;

	return x == -(-x);
}

bool test_addition() {
	LongNumber x = 999.999999_ln;
	LongNumber y = 0.00002_ln;
	LongNumber z = 1000.000019_ln;

	return (x + y == z);
}

bool test_subtraction() {
	LongNumber x = 999.999999_ln;
	LongNumber y = 0.00002_ln;
	LongNumber z = 1000.000019_ln;

	LongNumber t = z - x;

	return (y == t);
}

bool test_multiplication() {
	LongNumber x = -1234.5_ln;
	LongNumber y = 0.0678_ln;
	LongNumber z = -83.6991_ln;

	return (x*y == z);
}

bool test_multiplication_round() {
	LongNumber x = LongNumber(vector<digit_t>{3, 3});
	LongNumber y = 0.2_ln;
	LongNumber z = LongNumber(vector<digit_t>{7});

	LongNumber t = x*y;

	return t == z;
}

bool test_division() {
	LongNumber x = -1234.5_ln;
	LongNumber y = 0.0678_ln;
	LongNumber z = -83.6991_ln;

	LongNumber res = (z / y) + 1234.5_ln;

	return (res.abs() < LongNumber(vector<digit_t>{1, 0}));
}

bool test_sqrt() {
	LongNumber x = 9.8596_ln;

	return (x.sqrt() - 3.14_ln).abs() < LongNumber(vector<digit_t>{1, 0});
}
