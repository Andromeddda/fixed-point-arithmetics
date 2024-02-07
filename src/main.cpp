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

	LongNumber z = x + y;

	cout << z.ToString() << endl;
	return 0;
}