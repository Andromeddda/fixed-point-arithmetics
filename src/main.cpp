using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "utils.hpp"
#include "long-number.hpp"

int main(void) {

	LongNumber x = 10.0_ln;
	LongNumber y = 20.0_ln;

	clock_t start = clock();
	LongNumber z = x / y;
	clock_t end = clock();
	std::cout << "PRECISION: " << PRECISION << "\nMIN KRATSUBA: " << KARATSUBA_MIN_LEN << "\nTIME: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

	cout << z.ToString() << endl;
	a++;


	return 0;
}