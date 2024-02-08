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
	size_t x_size = digits.size();
	size_t y_size = other.digits.size();
	if (x_size != y_size) {
		return (x_size > y_size) ^ (sign == -1);
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
	return (*this > other) || (*this == other);
}

bool LongNumber::operator< (const LongNumber& other) {
	return !(*this >= other);
}

bool LongNumber::operator<= (const LongNumber& other) {
	return !(*this > other);
}

bool LongNumber::operator!= (const LongNumber& other) {
	return !(*this == other);
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

LongNumber LongNumber::operator+ (const LongNumber& other) {
	// If signs are different call operator-
	if (sign != other.sign) {
		if (sign == -1) {
			return -((-(*this)) - other);
		}
		else {
			return (*this - (LongNumber)(-other));
		}
	}

	// Signs are equal
	// Copy digits
	vector<int> d1 = vector<int>(digits);
	vector<int> d2 = vector<int>(other.digits);

	size_t size = MAX(d1.size(), d2.size());

	// Insert zeros in the front to balance sizes
	if (d1.size() < size) {
		d1.insert(d1.begin(), size - d1.size(), 0);
	}
	if (d2.size() < size) {
		d2.insert(d2.begin(), size - d2.size(), 0);
	}

	// Create new LongNumber
	LongNumber result;
	result.digits = vector<int>(size, 0);
	result.sign = sign;

	// Go from the back and add digits and overflows
	int overflow = 0;
	for (int i = (int)size; i >= 0; --i) {
		int sum = d1[i] + d2[i] + overflow;
		result.digits[i] = sum % 10;
		overflow = sum / 10;
	}

	// If first digit is also overflowed, insert overflow in the front
	if (overflow != 0) {
		result.digits.insert(result.digits.begin(), overflow);
	}

	return result;
}

LongNumber LongNumber::operator- (const LongNumber& other) {
	// If numbers are equal, return zero from constructor
	// so that it has positive sign
	if (*this == other) return LongNumber();

	// If signs are different call operator+
	if (sign != other.sign) {
		if (sign == -1) {
			return -((-(*this)) + other);
		}
		else {
			return (*this + (LongNumber)(-other));
		}
	}

	// If both signs are negative, call operator- in different order
	if (sign == -1) {
		return (-other) - (-(*this));
	}

	// If both signs are positive
	// Compare numbers
	bool compare = ((*this) > other);

	// Copy the biggest number
	vector<int> d1 = vector<int>(compare ? digits : other.digits);

	// Copy the smallest number
	vector<int> d2 = vector<int>(compare ? other.digits : digits);

	// Find new size to insert zeros in the front
    size_t size = MAX(d1.size(), d2.size());

    // Insert zeros in the front
    if (d1.size() < size)
    	d1.insert(d1.begin(), size - d1.size(), 0);
    if (d2.size() < size)
    	d2.insert(d2.begin(), size - d2.size(), 0);

    // Create new number
    LongNumber result;
    result.sign = compare ? 1 : -1;
    result.digits = vector<int>(size, 0);

    // Run and subtract
    int loan = 0; 
    for (int i = size - 1; i >= 0; i--) {
    	if (d1[i] - loan < d2[i]) {
    		// Need to take extra 10 from next digit
    		result.digits[i] = 10 + d1[i] - loan - d2[i];
    		loan = 1;
    	}
    	else {
    		result.digits[i] = d1[i] - loan - d2[i];
    		loan = 0;
    	}
    }

    // Remove zeros in the front
    while (result.digits[0] == 0) {
    	result.digits.erase(result.digits.begin());
    }

    return result;

}
LongNumber LongNumber::operator* (const LongNumber& other) {
	// Create new LongNumber
	LongNumber result;

	// Calculate sign
	result.sign = sign * other.sign;

	// Copy other.digits to successfully put it in Karatsuba function
	vector<int> other_digits = vector<int>(other.digits);

	// Multiply 
	result.digits = Karatsuba(digits, other_digits);

	// If two fixed-point numbers have lengths PRECISION+a and PRECISION+b,
	// then their multiplication must have lenght PRECISION+a+b,
	// but if we multiply two containers vector<int>, the output length will be 2*PRECISION+a+b

	// Fix double precision
	result.digits.erase(result.digits.end() - PRECISION, result.digits.end());

	// Remove zeros in the front
	while (result.digits[0] == 0) {
		result.digits.erase(result.digits.begin());
	}

	return result;
}

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

// User-defined floating-point literal
LongNumber operator""_ln(long double number) { 
	return LongNumber(number);
}

// Return a string form of long number
string LongNumber::ToString() {
	if (!(*this)) return "0";

	string result = "";
	int size = digits.size();

	// Track leading zeros from the back of number
	// So that we print for example 123.45 instead of 123.450000..000
	bool significant = false;

	for (int i = (int)size - 1; i >= 0; --i) {
		significant = significant || (digits[i] != 0);

		// Skip very leading zero
		if (!significant) continue;

		// Write every significant digit
		result = (char)(digits[i] + '0') + result;

		// Write a fixed point when we finish with (PRECISION) digits in the back
		if (i == size - PRECISION) {
			result = "." + result;
		}
	}

	if (sign == -1) {
		result = "-" + result;
	}

	return result;
}

/////////////////////
// KARATSUBA UTILS //
/////////////////////

// Recursive multiplication of two vectors
vector<int> Karatsuba(vector<int>& x, vector<int>& y) {
	size_t size = MAX(x.size(), y.size());

	if (size < KARATSUBA_MIN_LEN) {
		return naive_multiplication(x, y);
	}

	size_t k = size / 2;

	// Split vectors in two parts
	auto mid_x = (x.size() > k) ? (x.end() - k) : x.begin();
	auto mid_y = (y.size() > k) ? (y.end() - k) : y.begin();

	vector<int> Xl {x.begin(), mid_x};
	vector<int> Xr {mid_x, x.end()};
	vector<int> Yl {y.begin(), mid_y};
	vector<int> Yr {mid_y, y.end()};

	// EXPLANATION OF KARARSUBA
	// if 	x = Xl*q + Xr,
	//	 	y = Yl*q + Yr, 
	// where 
	// 		q = 10^k, then 
	//	 xy = 
	// 		= qq*(Xl*Yl) + q*(Xl*Yr + Xr*Yl) + Xr*Yr =
	//		= qq*P1 + q*P3 + P2
	// where 
	//		P1 = Xl*Yl
	// 		P2 = Xr*Yr
	//		P3 = (Xl + Xr)*(Yl + Yr) - P1 - P2

	vector<int> P1 = Karatsuba(Xl, Yl);
	vector<int> P2 = Karatsuba(Xr, Yr);

	vector<int> Xlr = Xl + Xr;
	vector<int> Ylr = Yl + Yr;

	// Calculate P3
	vector<int> P3 = Karatsuba(Xlr, Ylr) - P1 - P2;

	// P1 = P1*qq
	// P3 = P3*q
	P1.insert(P1.end(), 2*k, 0);
	P3.insert(P3.end(), k, 0);

	vector<int> result = P1 + P3 + P2;

	return result;
}

// Multiply two vector<int> with complixity O(n^2)
vector<int> naive_multiplication(vector<int>& left, vector<int>& right) {

	if (MIN(left.size(), right.size()) == 0) return vector<int>(1, 0);

	// Create new vector<int>
	size_t size = left.size() + right.size();
	vector<int> result = vector<int>(size, 0);

	// Run through and add
	for (int i = left.size() - 1; i >= 0; --i) {
		for (int j = right.size() - 1; j >= 0; --j) {
			result[i + j + 1] += left[i]*right[j];
		}
	}

	// fix digit overflow
	for (int i = result.size() - 1; i > 0; --i) {
		result[i - 1] += (result[i] >= 0) ? (result[i] / 10) : (result[i] / 10 - 1);
		result[i] = (result[i] >= 0) ? (result[i] % 10) : (result[i] % 10 + 10);
	}

	return result;
}

// Overload addition of vectors
vector<int> operator+ (const vector<int>& x, const vector<int>& y) {
	// Copy
	vector<int> d1 = vector<int>(x);
	vector<int> d2 = vector<int>(y);

	size_t size = MAX(d1.size(), d2.size());

	// Insert zeros in the front
    if (d1.size() < size)
    	d1.insert(d1.begin(), size - d1.size(), 0);
    if (d2.size() < size)
    	d2.insert(d2.begin(), size - d2.size(), 0);

    // Create new vector
    vector<int> result = vector<int>(1 + size, 0);

    // Go from the back and add
    int overflow = 0, sum = 0;
    for (int i = size - 1; i >= 0; i--) {
    	sum = d1[i] + d2[i] + overflow;
    	result[i+1] = sum % 10;
    	overflow = sum / 10;
    }

    // Insert or pop the first digit
    if (overflow != 0) {
    	result[0] = overflow;
    }
    else {
    	result.erase(result.begin());
    }

	return result;
}

// Overload subtraction of vectors
vector<int> operator- (const vector<int>& x, const vector<int>& y) {
	// Copy
	vector<int> d1 = vector<int>(x);
	vector<int> d2 = vector<int>(y);

	size_t size = MAX(d1.size(), d2.size());

	// Insert zeros in the front
    if (d1.size() < size)
    	d1.insert(d1.begin(), size - d1.size(), 0);
    if (d2.size() < size)
    	d2.insert(d2.begin(), size - d2.size(), 0);

    // Create new vector
    vector<int> result = vector<int>(size, 0);

    // Run and subtract
    int loan = 0; 
    for (int i = size - 1; i >= 0; i--) {
    	if (d1[i] - loan < d2[i]) {
    		// Need to take extra 10 from next digit
    		result[i] = 10 + d1[i] - loan - d2[i];
    		loan = 1;
    	}
    	else {
    		result[i] = d1[i] - loan - d2[i];
    		loan = 0;
    	}
    }

    // Remove first digit if it is 0
    if (result[0] == 0) {
    	result.erase(result.begin());
    }

	return result;
}

