/**
 * @file test.cpp
 * @author Tomáš Ludrovan
 * @brief Test suite for some of the `Math` module functions.
 * @version 0.1
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <string>

#include "Math.hpp"

// You might want to disable this if your terminal does not support
// ANSI escape codes.
#define ENABLE_COLORED_OUTPUT

#ifdef ENABLE_COLORED_OUTPUT
#	define BEGIN_FAILED_TEXT "\x1B[31m"  // Red
#	define BEGIN_PASSED_TEXT "\x1B[32m"  // Green
#	define RESET_TEXT "\x1B[0m"          // Default
#else // !ENABLE_COLORED_OUTPUT
#	define BEGIN_FAILED_TEXT
#	define BEGIN_PASSED_TEXT
#	define RESET_TEXT
#endif // !ENABLE_COLORED_OUTPUT


/**
 * @brief Test case setup.
 * 
 * @details `testName` is a `const char*` value identifying the test case.
 */
#define BEGIN_TEST(testName) try { \
	std::cout << testName << std::endl;

/**
 * @brief Test case verify and teardown.
 * 
 * @details `isPassed` is a boolean rvalue which identifies the result of test
 *          case.
 */
#define END_TEST(isPassed)                                         \
	std::cout << ((isPassed)                                       \
			? BEGIN_PASSED_TEXT "Passed" RESET_TEXT                \
			: BEGIN_FAILED_TEXT "Failed" RESET_TEXT)               \
		<< std::endl;                                              \
	if (isPassed) ++passCount;                                     \
	++testCount;                                                   \
} catch (...) {                                                    \
	std::cout << BEGIN_FAILED_TEXT "Failed (exception)" RESET_TEXT \
		<< std::endl;                                              \
	++testCount;                                                   \
}

#define TEST_CASE(testName, isPassed) \
	BEGIN_TEST(testName)              \
	END_TEST(isPassed)

#define TEST_CASE_CALL(call, res) \
	TEST_CASE(#call, call == res)

#define TEST_CASE_ROUND_I_F(to, p1, p2, res)          \
	TEST_CASE_CALL(round##to##Multiple(p1, p2), res)  \
	TEST_CASE_CALL(roundf##to##Multiple(static_cast<double>(p1), static_cast<double>(p2)), static_cast<double>(res))


int main()
{
	int passCount = 0, testCount = 0;

	TEST_CASE_ROUND_I_F(To, 14, 5, 15)
	TEST_CASE_ROUND_I_F(To, 16, 5, 15)
	TEST_CASE_ROUND_I_F(To, 20, 5, 20)
	TEST_CASE_ROUND_I_F(To, -14, 5, -15)
	TEST_CASE_ROUND_I_F(To, -16, 5, -15)
	TEST_CASE_ROUND_I_F(To, -20, 5, -20)
	TEST_CASE_ROUND_I_F(To, 14, -5, 15)
	TEST_CASE_ROUND_I_F(To, 16, -5, 15)
	TEST_CASE_ROUND_I_F(To, 20, -5, 20)
	TEST_CASE_ROUND_I_F(To, -14, -5, -15)
	TEST_CASE_ROUND_I_F(To, -16, -5, -15)
	TEST_CASE_ROUND_I_F(To, -20, -5, -20)
	TEST_CASE_ROUND_I_F(To, 9, 6, 12)
	TEST_CASE_ROUND_I_F(To, -9, 6, -12)
	TEST_CASE_ROUND_I_F(To, 9, -6, 12)
	TEST_CASE_ROUND_I_F(To, -9, -6, -12)

	std::cout << std::string(40, '-') << std::endl;

	TEST_CASE_ROUND_I_F(UpTo, 14, 5, 15)
	TEST_CASE_ROUND_I_F(UpTo, 16, 5, 20)
	TEST_CASE_ROUND_I_F(UpTo, 20, 5, 20)
	TEST_CASE_ROUND_I_F(UpTo, -14, 5, -10)
	TEST_CASE_ROUND_I_F(UpTo, -16, 5, -15)
	TEST_CASE_ROUND_I_F(UpTo, -20, 5, -20)
	TEST_CASE_ROUND_I_F(UpTo, 14, -5, 15)
	TEST_CASE_ROUND_I_F(UpTo, 16, -5, 20)
	TEST_CASE_ROUND_I_F(UpTo, 20, -5, 20)
	TEST_CASE_ROUND_I_F(UpTo, -14, -5, -10)
	TEST_CASE_ROUND_I_F(UpTo, -16, -5, -15)
	TEST_CASE_ROUND_I_F(UpTo, -20, -5, -20)
	TEST_CASE_ROUND_I_F(UpTo, 9, 6, 12)
	TEST_CASE_ROUND_I_F(UpTo, -9, 6, -6)
	TEST_CASE_ROUND_I_F(UpTo, 9, -6, 12)
	TEST_CASE_ROUND_I_F(UpTo, -9, -6, -6)

	std::cout << std::string(40, '-') << std::endl;

	TEST_CASE_ROUND_I_F(DownTo, 14, 5, 10)
	TEST_CASE_ROUND_I_F(DownTo, 16, 5, 15)
	TEST_CASE_ROUND_I_F(DownTo, 20, 5, 20)
	TEST_CASE_ROUND_I_F(DownTo, -14, 5, -15)
	TEST_CASE_ROUND_I_F(DownTo, -16, 5, -20)
	TEST_CASE_ROUND_I_F(DownTo, -20, 5, -20)
	TEST_CASE_ROUND_I_F(DownTo, 14, -5, 10)
	TEST_CASE_ROUND_I_F(DownTo, 16, -5, 15)
	TEST_CASE_ROUND_I_F(DownTo, 20, -5, 20)
	TEST_CASE_ROUND_I_F(DownTo, -14, -5, -15)
	TEST_CASE_ROUND_I_F(DownTo, -16, -5, -20)
	TEST_CASE_ROUND_I_F(DownTo, -20, -5, -20)
	TEST_CASE_ROUND_I_F(DownTo, 9, 6, 6)
	TEST_CASE_ROUND_I_F(DownTo, -9, 6, -12)
	TEST_CASE_ROUND_I_F(DownTo, 9, -6, 6)
	TEST_CASE_ROUND_I_F(DownTo, -9, -6, -12)

	std::cout << "Result: " << passCount << "/" << testCount << std::endl;
}
