#include <iostream>
#include <ctime>

// // // // // // //
//  SAFETY UTILS  //
// // // // // // //

#define TERMINATE(message) 				\
	std::cout << message << std::endl; 	\
	exit(1);

#define VERIFY_CONTRACT(contract, message) 			\
	if (!(contract)) { 								\
		std::cout << SET_COLOR_RED << message << RESET_COLOR << std::endl; \
		exit(1); 									\
	}

// // // // // //
//  CODE UTILS //
// // // // // //

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// // // // // //
// DEBUG UTILS //
// // // // // //
#define SET_COLOR_RED 		"\033[1;31m"
#define SET_COLOR_GREEN 	"\033[1;32m"
#define SET_COLOR_YELLOW 	"\033[1;33m"
#define SET_COLOR_PURPLE 	"\033[1;35m"
#define SET_COLOR_CYAN 		"\033[1;36m"
#define RESET_COLOR 		"\033[0m"

#define DEBUG_B \
	std::cout << SET_COLOR_CYAN << "[===DEBUG===]: [" << __FUNCTION__ << \
	"] on line " << __LINE__ << RESET_COLOR << std::endl; 

#define DEBUG_M(message) \
	std::cout << SET_COLOR_CYAN << "[===DEBUG===]: [" << __FUNCTION__ << \
	"] on line " << __LINE__ << " | " << message << RESET_COLOR << std::endl;

#define DEBUG_EXEC(func, ...) 																					\
	std::cout << SET_COLOR_CYAN << "[===DEBUG===]: [" << __FUNCTION__ << "] on line " << __LINE__ << " | "; 	\
	func(__VA_ARGS__); 																							\
	std::cout << RESET_COLOR << std::endl;

// // // // // //
// TIME UTILS  //
// // // // // //

#define FUNC_TIMING(msg, func, ...)															\
	{ 																						\
	clock_t start = clock(); 																\
	func(__VA_ARGS__); 																		\
	clock_t end = clock();																	\
	std::cout << msg << (double)(end - start) * 1000.0 / CLOCKS_PER_SEC << "ms" << std::endl;	\
	}

#define EXEC_TIMING(msg, code) \
	{ 																						\
	clock_t start = clock(); 																\
	code; 																					\
	clock_t end = clock();																	\
	std::cout << msg << " " <<(double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;	\
	}
