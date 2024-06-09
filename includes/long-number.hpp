// WARNING: does not work with char (neither sign or unsigned)
typedef short digit_t;

#include <iostream>
#include <vector>
#include <string>

#define PRECISION (1020)
#define KARATSUBA_MIN_LEN (100)

// The number is represented as a big integer divided by 10^PRECISION 
class LongNumber {
	// This std::vector represents the big integer number
	std::vector<digit_t> digits;
	int sign;

	bool ok() const; // check if number is valid

public:
	//////////////////
	// CONSTRUCTORS //
	//////////////////

	// THE RULE OF FIVE
	LongNumber(); 									// Construct 0.0

	LongNumber(const LongNumber& other) = default; 			// Copy constructor
	LongNumber(LongNumber&& other) = default;					// Move constructor

	LongNumber& operator= (const LongNumber& other) = default; // Copy assignment
	LongNumber& operator= (LongNumber&& other) = default;		// Move Assignment

	// MOVE CONSTRUCTORS
	LongNumber(long double number); // Construct from double
	LongNumber(const std::string& str); 	// Construct from std::string

	// Construct from std::vector<int> and sign
	// 		NOTE: it may be used to create really small numbers
	// 		such as 0.000...0001 (with maximum precision)
	LongNumber(const std::vector<digit_t>& digits, bool positive = true);

	/////////////////////
	// LOGIC OPERATORS //
	/////////////////////

	operator bool() const; // Return false if number == 0 or empty, return true otherwise
	bool operator== (const LongNumber& other) const;
	bool operator> (const LongNumber& other) const;
	bool operator>= (const LongNumber& other) const;
	bool operator< (const LongNumber& other) const;
	bool operator<= (const LongNumber& other) const;
	bool operator!= (const LongNumber& other) const;

	///////////////////////
	// NUMERIC OPERATORS //
	///////////////////////

	LongNumber operator- () const; 						// Unary minus
	LongNumber operator+ (const LongNumber& other) const; 
	LongNumber operator- (const LongNumber& other) const;
	LongNumber operator* (const LongNumber& other) const; // Karatsuba algorithm
	LongNumber operator/ (const LongNumber& other) const; // Goldschmidt method

	//////////////////////
	// USABLE FUNCTIONS //
	//////////////////////

	// Return a std::string form of long number
	std::string ToString() const;

	LongNumber sqrt(unsigned int iterations = 20, const LongNumber& initial = LongNumber(0.1)) const;

	// Return absolute value
	LongNumber abs() const;
};

////////////////////////////////
// UTILITIES FOR LONG NUMBERS //
////////////////////////////////

// User-defined floating-point literal
LongNumber operator""_ln(const long double number);
