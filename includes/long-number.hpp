using namespace std;

#include <iostream>
#include <vector>
#include <string>

#include "utils.hpp"

#define BASE (10)
#define PRECISION (10000)

class LongNumber {
	// The number is represented as some big integer number divided by BASE^PRECISION
	std::vector<int> digits;
	int sign;
}