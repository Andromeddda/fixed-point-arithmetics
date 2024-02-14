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

// Check if number is valid
bool LongNumber::ok() const {
	// Check if sign is valid
	if ((sign != 1) && (sign != -1)) return false;

	// Check if digits not empty
	if (digits.size() == 0) return false;

	// Check if all digits are actually digits
	for (size_t i = 0; i < digits.size(); ++i) {
		if ((digits[i] > 9) && (digits[i] < 0)) return false;
	}

	return true;
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
LongNumber::LongNumber(const LongNumber& other) {
	VERIFY_CONTRACT(other.ok(), "ERROR: unable to counstruct LongNumber from invalid origin");

	// Copy sign
	sign = other.sign; 
	// Copy digits
	digits = vector<int>(other.digits);

	VERIFY_CONTRACT(this->ok(), "ERROR: unable to counstruct LongNumber by copying");
}

// Move constructor
LongNumber::LongNumber(LongNumber&& other) {
	VERIFY_CONTRACT(other.ok(), "ERROR: unable to counstruct LongNumber from invalid origin");

	// Copy sign
	sign = other.sign; 
	// Copy digits
	digits = vector<int>(other.digits);

	// delete origin
	other.digits.clear();
	other.digits.resize(0);

	VERIFY_CONTRACT(this->ok(), "ERROR: unable to counstruct LongNumber by moving");
	VERIFY_CONTRACT(!other.ok(), "ERROR: move counstructor is note destructive for origin");
}

// Copy assignment
LongNumber& LongNumber::operator= (const LongNumber& other) {
	VERIFY_CONTRACT(other.ok(), "ERROR: unable to assign LongNumber to invalid origin");

	sign = other.sign;
	digits = vector<int>(other.digits);
	
	VERIFY_CONTRACT(this->ok(), "ERROR: unable to assign LongNumber");

	return *this;
}

// Move assignment
LongNumber& LongNumber::operator= (LongNumber&& other) {
	VERIFY_CONTRACT(other.ok(), "ERROR: unable to counstruct LongNumber from invalid origin");

	// Copy sign
	sign = other.sign; 
	// Copy digits
	digits = vector<int>(other.digits);

	// delete origin
	other.digits.clear();
	other.digits.resize(0);

	VERIFY_CONTRACT(this->ok(), "ERROR: unable to assign LongNumber by moving");
	VERIFY_CONTRACT(!other.ok(), "ERROR: move assignment is note destructive for origin");

	return *this;
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
		significant |= (str[index] != '0') || (count_points > 0);

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
LongNumber::LongNumber(const long double number) : LongNumber(std::to_string(number)) {}

// Construct from vector<int> and sign
LongNumber::LongNumber(const vector<int>& array, bool positive) {
	sign = positive ? 1 : -1;
	digits = vector<int>(array);

	// Fill the zeros
	if (array.size() < PRECISION) {
		digits.insert(digits.begin(), PRECISION - digits.size(), 0);
	}
}


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

bool LongNumber::operator== (const LongNumber& other) const {
	return (sign == other.sign) && (digits == other.digits);
}

bool LongNumber::operator> (const LongNumber& other) const {
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

bool LongNumber::operator>= (const LongNumber& other) const {
	return (*this > other) || (*this == other);
}

bool LongNumber::operator< (const LongNumber& other) const {
	return !(*this >= other);
}

bool LongNumber::operator<= (const LongNumber& other) const {
	return !(*this > other);
}

bool LongNumber::operator!= (const LongNumber& other) const {
	return !(*this == other);
}

///////////////////////
// NUMERIC OPERATORS //
///////////////////////

LongNumber LongNumber::operator- () const {
	if (!(*this)) {
		return LongNumber();
	}

	LongNumber result;
	result.sign = -sign;
	result.digits = vector<int>(digits);
	return result;
}

LongNumber LongNumber::operator+ (const LongNumber& other) const {
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
	for (int i = (int)size - 1; i >= 0; --i) {
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

LongNumber LongNumber::operator- (const LongNumber& other) const {
	// If numbers are equal, return zero from constructor
	// so that it has positive sign
	if ((*this) == other) {
		return LongNumber();
	}

	// If signs are different call operator+
	if (sign != other.sign) {
		if (sign == -1) {
			return -((-(*this)) + other);
		}
		else {
			return (*this + (LongNumber)(-other));
		}
	}

	// Signs are equal
	bool compare = ((*this) > other) ^ (sign == -1);

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

    //Remove zeros in the front
    while ((result.digits[0] == 0) && (result.digits.size() > PRECISION)) {
    	result.digits.erase(result.digits.begin());
    }

    return result;
}

LongNumber LongNumber::operator* (const LongNumber& other) const {
	// Create new LongNumber
	LongNumber result;

	// Calculate sign
	result.sign = sign * other.sign;
	
	// Copy digits to successfully put it in Karatsuba function
	vector<int> d1 = vector<int>(digits);
	vector<int> d2 = vector<int>(other.digits);

	// Remove zeros in the back to avoid multiplying zeros in Karatsuba()
	int back_zeros = 0;
	int i = d1.size() - 1;
	int j = d2.size() - 1;
	while ((back_zeros < PRECISION) && (d1[i] == 0) && (d2[j] == 0) && (d1.size() > 0) && (d2.size() > 0)) {
		back_zeros++;
		d1.pop_back();
		d2.pop_back();
		--i;
		--j;
	}

	// Multiply 
	result.digits = Karatsuba(d1, d2);

	// If two fixed-point numbers have lengths PRECISION+a and PRECISION+b
	// with N zeros in the back which we deleted,
	// then their production must have lenght PRECISION+a+b,
	// but when we multiply two vectors, the output length is 2*PRECISION-2*N + a + b

	bool round = false;

	// Fix precision
	if (PRECISION >= 2*back_zeros) {
		// Round (or not) numbers after multiplying
		#ifdef ROUND
		if (result.digits[result.digits.size() - PRECISION + 2*back_zeros] >= 5) {
			round = true;
		}
		#endif

		result.digits.erase(result.digits.end() - PRECISION + 2*back_zeros, result.digits.end());
	}
	else if (PRECISION < 2*back_zeros) {
		result.digits.insert(result.digits.end(), 2*back_zeros - PRECISION, 0);
	}

	if (round) {
		result = result + LongNumber(vector<int>{1}, (sign == 1));
	}
	
	//Remove zeros in the front
	while ((result.digits[0] == 0) && (result.digits.size() > PRECISION)) {
		result.digits.erase(result.digits.begin());
	}

	return result;
}

LongNumber LongNumber::operator/ (const LongNumber& other) const {
	// Copy numbers
	LongNumber A = LongNumber(*this);
	LongNumber B = LongNumber(other);

	// Copy them one more time to fix the result in the end
	LongNumber N = LongNumber(*this);
	LongNumber D = LongNumber(other);

	if (B.get_sign() == -1) {
		return (-A) / (-B);
	}

	// Handle division by zero
	VERIFY_CONTRACT(B != 0.0_ln, "ERROR: division by zero");

	// Handle division by 1.0
	if (B == 1.0_ln) return A;

	// Calculate first approximation of F between 0.1 and 1
	LongNumber F = 1.0_ln;
	if (B.get_digits().size() == PRECISION) {
		// If B < 0.1, find such F that 0.1 <= F*B < 1
		// multiplying F by ten for every zero in the front of B
		int i = 0;
		while (B.get_digits()[i] == 0) {
			F = F * 10.0_ln;
			++i;
		}
	}
	else {
		// if B > 1, find such F that 0.1 < F*B < 1
		// dividing F by ten for every digit in B
		int i = B.get_digits().size();
		while (i > PRECISION) {
			F = F * 0.1_ln;
			--i;
		}
	}

	LongNumber B_inverse = 1.0_ln;

	/* Explanation of Goldschmidt:
		both A and B are multiplied by some sequence of numbers F(i)
		until B(i) == 1.0.
		Therefore A(i) == A(0) / B(0)
	*/
	while ( B != 1.0_ln) {
		A = A * F;
		B = B * F;
		B_inverse = B_inverse * F;
		F = 2.0_ln - B;
	}

	/* Explanation of correction:
		Due to rounding of numbers, B can become 1.0 earlier, than A become the precise result.
		Therefore, there is a difference between A*D and N

		Let BI = 1/D,
			N/D ~ A

		d = N - D*A
		d*BI = N*BI - BI*D*A
		d*BI = N/D - A
		N/D = A + d*BI 

	*/

	LongNumber d = N + D*(-A);
	LongNumber fix = d*B_inverse;
	LongNumber res = fix + A;

	return res;
}

////////////////////////////////
// UTILITIES FOR LONG NUMBERS //
////////////////////////////////

// Print all elements of vector
void vector_print(const vector<int>& vec) {
	std::cout << "{";
	for (size_t i = 0; i < vec.size(); i++) {
		cout << vec[i] << ((i + 1 == vec.size()) ? "" : ", ");
	}
	std::cout << "} " << endl;
}

// User-defined floating-point literal
LongNumber operator""_ln(long double number) { 
	return LongNumber(number);
}

// Return a string form of long number
string LongNumber::ToString() const {
	if (!(*this)) return "0";
	
	string result = "";
	int size = digits.size();

	// Track leading zeros from the back of number
	// So that we print for example 123.45 instead of 123.450000..000
	bool significant = false;

	for (int i = (int)size - 1; i >= 0; --i) {
		significant = significant || (digits[i] != 0) || (i <= size - PRECISION);

		// Skip very leading zero
		if (!significant) {
			continue;
		}

		// Write every significant digit
		result = to_string(digits[i]) + result;

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

LongNumber LongNumber::abs() const {
	LongNumber result;
	result.digits = vector<int>(digits);
	result.sign = 1;
	return result;
}

/////////////////////
// KARATSUBA UTILS //
/////////////////////

// Recursive multiplication of two vectors
vector<int> Karatsuba(vector<int>& X, vector<int>& Y) {
	vector<int> x = vector<int>(X);
	vector<int> y = vector<int>(Y);

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

	// EXPLANATION OF KARATSUBA
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

    	// Add with shift to i+1 because of extra digit, insertet in the front
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