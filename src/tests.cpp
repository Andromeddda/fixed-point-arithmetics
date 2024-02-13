#include "test_system.hpp"
#include "long-number.hpp"
#include "utils.hpp"

using namespace TestSystem;

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
	try {			
		LongNumber x = -1234.567_ln;
		LongNumber y = LongNumber(x);

    	return (y.get_sign() == x.get_sign()) && (x.get_digits() == y.get_digits());
	}
	catch (const std::exception& exc) {
        return FAIL;
    }
    return OK;
}

bool test_string_constructor() {
	try {
		LongNumber x = LongNumber("-1234.567");
		vector<int> d = vector<int>(PRECISION + 4, 0);
    	for (int i = 0; i < 7; i++) d[i] = i+1;

    	return (x.get_sign() == -1) && (x.get_digits() == d);
	}
	catch (const std::exception& exc) {
        return FAIL;
    }
    return OK;
}

bool test_double_constructor() {
	try {			
		LongNumber x = LongNumber(-1234.567);
		vector<int> d = vector<int>(PRECISION + 4, 0);
    	for (int i = 0; i < 7; i++) d[i] = i+1;

    	return (x.get_sign() == -1) && (x.get_digits() == d);
	}
	catch (const std::exception& exc) {
        return FAIL;
    }
    return OK;
}

bool test_ud_suffix_constructor() {
	try {			
		LongNumber x = -1234.567_ln;
		vector<int> d = vector<int>(PRECISION + 4, 0);
    	for (int i = 0; i < 7; i++) d[i] = i+1;

    	return (x.get_sign() == -1) && (x.get_digits() == d);
	}
	catch (const std::exception& exc) {
        return FAIL;
    }
    return OK;
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

	return (y == z - x);
}

bool test_multiplication() {
	LongNumber x = -1234.5_ln;
	LongNumber y = 0.0678_ln;
	LongNumber z = -83.6991_ln;

	return (x*y == z);
}

bool test_multiplication_round() {
	LongNumber x = LongNumber(vector<int>{3, 3});
	LongNumber y = 0.2_ln;
	LongNumber z = LongNumber(vector<int>{7});

	LongNumber t = x*y;

	return t == z;
}

bool test_division() {
	LongNumber x = -1234.5_ln;
	LongNumber y = 0.0678_ln;
	LongNumber z = -83.6991_ln;

	LongNumber res = (z / y) + 1234.5_ln;

	return (res.abs() < LongNumber(vector<int>{1, 0}));
}
