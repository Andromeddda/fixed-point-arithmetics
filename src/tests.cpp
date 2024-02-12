#include "test_system.hpp"
#include "long-number.hpp"

using namespace TestSystem;

bool test_empty_constructor() {
	try {
		LongNumber x = LongNumber();
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

bool test_ToString() {
	LongNumber x = -1234.56701_ln;
	string str = "-1234.56701";

	return (x.ToString() == str);
}

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

bool test_division() {
	LongNumber x = -1234.5_ln;
	LongNumber y = 0.0678_ln;
	LongNumber z = -83.6991_ln;

	return (x == z / y);
}
