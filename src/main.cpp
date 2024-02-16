using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "utils.hpp"
#include "long-number.hpp"
#include "tests.hpp"
#include "test_system.hpp"

using namespace TestSystem;

int main(void) {
	run_test("empty constructor", 	test_empty_constructor);
	run_test("copy constructor", 	test_copy_constructor);
	run_test("string constructor",	test_string_constructor);
	run_test("double constructor",	test_double_constructor);
	run_test("ud-suffix", 			test_ud_suffix_constructor);

	run_test("ToString() method", 	test_ToString);

	run_test("equality", 			test_equality);
	run_test("comparison", 			test_comparison);

	run_test("unary minus",			test_unary_minus);
	run_test("addition", 			test_addition);

	run_test("subtraction", 		test_subtraction);
	
	run_test("multiplication", 		test_multiplication);
	run_test("rounding the product", test_multiplication_round);
	run_test("division", 			test_division);
	run_test("square root", 		test_sqrt);

	return 0;
}