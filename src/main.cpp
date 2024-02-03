using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "utils.hpp"
#include "long-number.hpp"

int main(void) {

	LongNumber x = LongNumber("-12345.6789");
	vector_print(x.get_digits());
	return 0;
}