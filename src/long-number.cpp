using namespace std;

#include <iostream>
#include <string>
#include <vector>

#include "utils.hpp"
#include "long-number.hpp"

/////////////
// GETTERS //
/////////////

int LongNumber::get_sign() {
	return sign;
}

// Return copy of digits
vector<int> LongNumber::get_digits() {
	return vector<int>(digits);
}

//////////////////
// CONSTRUCTORS //
//////////////////

// Construct 0.0
LongNumber::LongNumber() {
	sign = 1;
	// Create an empty vector
	digits = std::vector<int>(1, 0);
}

// Copy constructor
LongNumber::LongNumber(LongNumber& other) {
	// Copy sign
	sign = other.sign; 
	// Copy digits
	digits = vector<int>(other.digits);
}

// Construct from string
LongNumber::LongNumber(const string& str) {
	int index = 0;
	if (str[index] == '-') {
		sign = -1;
		++index;
	}
	else sign = 1;

	// Count points to detect incorrect form of number
	// and to tell if current digit is before or after the point
	int count_points = 0;

	// significant == false if current digit is a leading zero
	bool significant = false;

	int cur_precision = 0;
	int length = str.length();

	while (index < length) {

		// Handle the case of point
		if (str[index] == '.') {
			VERIFY_CONTRACT(count_points == 0, 
				"LongNumber(const string): incorrect form of number, more than one point");
			count_points++;
			index++;
			continue;
		}

		// Handle the case of non-digit (throw exception)
		VERIFY_CONTRACT(isdigit(str[index]), 
			"LongNumber(const string): incorrect form of number, unexpected character");

		// Handle the case of digit

		// Check if current digit is significant
		// (for 0.000088164 five zeros in the front are insignificant)
		significant |= (str[index] != '0');

		// increase current precision if we already met the point
		if (count_points > 0) ++cur_precision;

		// if current precision is greater than fixed precision, break
		if (cur_precision >= PRECISION) break;

		// push every significant digit
		if (significant) digits.push_back((int)(str[index] - '0'));

		index++;
	}

	digits.insert(digits.end(), (size_t)(PRECISION - cur_precision), 0);

}

// Construct from double
// Delegate to constructor from string litearal (cince C++11)
LongNumber::LongNumber(long double number) : LongNumber(std::to_string(number)) {}

/////////////////////
// LOGIC OPERATORS //
/////////////////////

// Return false if number == 0 or empty, return true otherwise
LongNumber::operator bool() const {
	// Return false if empty
	if (digits.size() == 0) return false;

	// Else return (number != 0)
	return !((digits.size() == 1) && (digits[0] == 0));
}

bool LongNumber::operator== (const LongNumber& other) {
	return (sign == other.sign) && (digits == other.digits);
}

bool LongNumber::operator> (const LongNumber& other) {
	// If signs are different, compare signs and return
	if (sign != other.sign) {
		return (sign > other.sign);
	}

	// If signs are equal, compare lengths of numbers
	if (digits.size() != other.digits.size()) {
		return (digits.size() > other.digits.size()) ^ (sign == -1);
	}

	// If lengths and signs are equal, run through digits and compare
	for (size_t i = 0; i < digits.size(); ++i) {
		if (digits[i] != other.digits[i]) {
			return (digits[i] > other.digits[i]) ^ (sign == -1);
		}
	}
	return false;
}

bool LongNumber::operator>= (const LongNumber& other) {
	return (*this > (LongNumber)other) || (*this == (LongNumber)other);
}

bool LongNumber::operator< (const LongNumber& other) {
	return !(*this >= (LongNumber)other);
}

bool LongNumber::operator<= (const LongNumber& other) {
	return !(*this > (LongNumber)other);
}

bool LongNumber::operator!= (const LongNumber& other) {
	return !(*this == (LongNumber)other);
}

///////////////////////
// NUMERIC OPERATORS //
///////////////////////

LongNumber LongNumber::operator- () {
	if (!(*this)) {
		return LongNumber();
	}

	LongNumber result;
	result.sign = -sign;
	result.digits = vector<int>(digits);
	return result;
}

// LongNumber operator+ (const LongNumber& other);
// LongNumber operator- (const LongNumber& other);
// LongNumber operator* (const LongNumber& other);
// LongNumber operator/ (const LongNumber& other);

////////////////////////////////
// UTILITIES FOR LONG NUMBERS //
////////////////////////////////

// Print all elements of vector
void vector_print(const vector<int>& vec) {
	std::cout << "{";
	for (size_t i = 0; i < vec.size(); i++) {
		cout << vec[i] << ((i + 1 == vec.size()) ? "" : ", ");
	}
	std::cout << "} ";
}

// Return a string form of long number
// string ToString(LongNumber& number) {
// 	if (!number) return "0";

// 	string result = "";
// 	vector<int> d = number.get_digits();
// 	int size = d.size();

// 	int back_zeros = 0;
// 	bool significant = false;

// 	for (int i = size - 1; i >= 0; i++) {
// 		significant |= (d[i] != 0);

// 		if (!significant) ++back_zeros;

// 		if (back_zeros > PRECISION) {
// 			result = (char)(d[i] + '0') + result;
// 		}
// 		else {
			
// 		}
// 	}
// }