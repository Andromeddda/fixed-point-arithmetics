using namespace std;

#include <iostream>
#include <vector>
#include <string>

#define PRECISION (10000)

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
	LongNumber(LongNumber& other); 	// Copy constructor
	LongNumber(long double number); // Construct from double
	LongNumber(const string& str); 	// Construct from string

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

	///////////////////////
	// NUMERIC OPERATORS //
	///////////////////////

	LongNumber operator- (); 						// Unary minus
	LongNumber operator+ (const LongNumber& other); 
	LongNumber operator- (const LongNumber& other);
	LongNumber operator* (const LongNumber& other); // Karatsuba algorithm
	LongNumber operator/ (const LongNumber& other); // Newton-Raphson method

};

////////////////////////////////
// UTILITIES FOR LONG NUMBERS //
////////////////////////////////

// Print all elements of vector
void vector_print(const vector<int>& vec);

// Return a string form of long number
string ToString(LongNumber& number);
