using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "utils.hpp"
#include "long-number.hpp"

int main(void) {

	LongNumber x = -123.4_ln;
	LongNumber y = -234.51_ln;

	clock_t start = clock();
	LongNumber z = x * y;
	clock_t end = clock();
	std::cout << "PRECISION: " << PRECISION << "\n" << \
	"MIN KRATSUBA: " << KARATSUBA_MIN_LEN << "\n" << \
	"TIME: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

	cout << z.ToString() << endl;

	return 0;
}