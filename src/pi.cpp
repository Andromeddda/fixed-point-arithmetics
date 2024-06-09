using namespace std;

#include <iostream>
#include <ctime>
#include <string>

#include "utils.hpp"
#include "long-number.hpp"
#include "pi.hpp"

int main(void) {
	unsigned N = 20;
	std::cout << SET_COLOR_CYAN << "Enter the number of decimal places of pi (<= " << MAX_PRECISION << ") : " << SET_COLOR_PURPLE;
	int correct = scanf("%u", &N);

	VERIFY_CONTRACT(correct == 1, "ERROR: incorrect input");
	VERIFY_CONTRACT(N <= MAX_PRECISION, "ERROR: cannot count more than " << MAX_PRECISION << " digits correctly");

	clock_t start = clock();
	LongNumber pi = Chudnovsky(N);
	clock_t end = clock();

	string str = pi.ToString();
	std::cout << SET_COLOR_CYAN << "Here is your pi, sir: " << SET_COLOR_PURPLE;
	for (unsigned i = 0; i < N + 2; ++i) {
		if (str[i] != pi_str[i]) std::cout << SET_COLOR_RED;
		std::cout << str[i];
	}
	std::cout << '\n'<< SET_COLOR_CYAN << "Time: " << SET_COLOR_PURPLE <<(double)(end - start) / CLOCKS_PER_SEC << "s" << RESET_COLOR << '\n';
	return 0;
}

// Input: number of decimal places of pi to calculate (unsigned N).
// Output: pi with given precision (LongNumber).
LongNumber Chudnovsky(unsigned N) {

	// PROOF OF THE ALGORITHM: https://arxiv.org/abs/1809.00533

	// EXPLANATION:
	// pi = 426880*sqrt(10005) / (13591409 + S(n))
	// where 
	// S(n) = sum of f(k)*(545140134*k + 13591409) with k = 1 to n
	// f(0) = 1
	// f(k) = f(k-1)*(1 - 6k)*(1 - 2k)*(5 - 6k) / 10939058860032000k^3

	const LongNumber Q = LongNumber("426880.0");
	const LongNumber R = LongNumber("10005.0").sqrt();
	const LongNumber C = LongNumber("13591409");
	const LongNumber D = LongNumber("545140134");
	const LongNumber I = 1.0_ln / LongNumber("10939058860032000.0");

	LongNumber Sum = LongNumber("13591409");
	LongNumber k = 1.0_ln;
	LongNumber Fk = 1.0_ln;

	// Precision N/10 obtained empirically.

	if (N > 13) {
		for (unsigned i = 1; i <= N / 10; i++) {
			LongNumber k_inv = 1.0_ln / k;
			Fk = Fk*(k_inv - 6.0_ln)*(k_inv - 2.0_ln)*(5.0_ln*k_inv - 6.0_ln)*I;
			Sum = Sum + Fk*(D*k + C);
			k = k + 1.0_ln;
		}
	}

	return Q*R / Sum;
}
