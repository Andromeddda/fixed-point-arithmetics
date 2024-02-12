using namespace std;

#include <iostream>
#include <vector>
#include <string>

#define PRECISION (1000)
#define KARATSUBA_MIN_LEN (100)
#define NEWTON_RAPHSON_ITERATIONS_DIV (15)
#define NEWTON_RAPHSON_ITERATIONS_SQRT (3)

// The number is represented as a big integer divided by 10^PRECISION 
class LongNumber {
	// This vector represents the big integer number
	std::vector<int> digits;
	int sign;

public:
	/////////////
	// GETTERS //
	/////////////

	int get_sign();
	vector<int> get_digits(); // return copy of digits

	//////////////////
	// CONSTRUCTORS //
	//////////////////

	LongNumber(); 					// Construct 0.0
	LongNumber(const LongNumber& other); 	// Copy constructor
	LongNumber(long double number); // Construct from double
	LongNumber(const string& str); 	// Construct from string

	// Construct from vector<int> and sign
	// 		NOTE: it may be used to create really small numbers
	// 		such as 0.000...0001 (with maximum precision)
	LongNumber(const vector<int>& digits, bool positive = true);

	/////////////////////
	// LOGIC OPERATORS //
	/////////////////////

	operator bool() const; // Return false if number == 0 or empty, return true otherwise
	bool operator== (const LongNumber& other);
	bool operator> (const LongNumber& other);
	bool operator>= (const LongNumber& other);
	bool operator< (const LongNumber& other);
	bool operator<= (const LongNumber& other);
	bool operator!= (const LongNumber& other);

	///////////////////////////
	// SIDE EFFECT OPERATORS //
	///////////////////////////
	LongNumber operator= (const LongNumber& other);

	///////////////////////
	// NUMERIC OPERATORS //
	///////////////////////

	LongNumber operator- (); 						// Unary minus
	LongNumber operator+ (const LongNumber& other); 
	LongNumber operator- (const LongNumber& other);
	LongNumber operator* (const LongNumber& other); // Karatsuba algorithm
	LongNumber operator/ (const LongNumber& other); // Newton-Raphson method

	//////////////////////
	// USABLE FUNCTIONS //
	//////////////////////

	// Return a string form of long number
	string ToString();

	// Return absolute value
	LongNumber abs();

};

////////////////////////////////
// UTILITIES FOR LONG NUMBERS //
////////////////////////////////

// Print all elements of vector
void vector_print(const vector<int>& vec);

// User-defined floating-point literal
LongNumber operator""_ln(const long double number);


/////////////////////
// KARATSUBA UTILS //
/////////////////////

// Recursive multiplication of two vectors
vector<int> Karatsuba(vector<int>& left, vector<int>& right);

// Multiply two vector<int> with complixity O(n^2)
vector<int> naive_multiplication(vector<int>& left, vector<int>& right);

// overload addition of vectors
vector<int> operator+ (const vector<int>& x, const vector<int>& y);

// overload subtraction of vectors
vector<int> operator- (const vector<int>& x, const vector<int>& y);


LongNumber GoldSchmidt(const LongNumber& a, const LongNumber& b);
