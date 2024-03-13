/**
 * @file test.cpp
 * @author Tomáš Ludrovan
 * @brief Test suite for `UnorderedSetWithIndexes` class.
 * @version 0.1
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <unordered_set>

#include "UnorderedSetWithIndexes.hpp"

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

typedef std::pair<double, double> DDPair;

// Function object for hashing `std::pair<double, double>` type
struct DDPairHash {
	size_t operator()(const DDPair& k) const noexcept {
		return static_cast<size_t>(k.first) ^ static_cast<size_t>(k.second);
	}
};


/**
 * @brief Fill `s` with `args`.
 */
template <typename SetType, typename... Args>
void initializeSet(SetType& s, const Args&... args)
{
	(s.insert(args), ...);
}

/**
 * @brief Check if `s` contains exactly `args`.
 */
template <typename T, typename... Args>
bool setEquals(const UnorderedSetWithIndexes<T>& s, const Args&... args)
{
	if (s.size() != sizeof...(args)) return false;

	return ((s.find(args) != s.end()) && ...);
}


/**
 * @brief Test case setup.
 * 
 * @details `testName` is a `const char*` value identifying the test case.
 *          Other arguments are passed as template arguments to the
 *          `UnorderedSetWithIndexes`.
 */
#define BEGIN_TEST(testName, ...) try { \
	std::cout << testName << std::endl; \
	UnorderedSetWithIndexes<__VA_ARGS__> s;

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


int main()
{
	int passCount = 0, testCount = 0;

	BEGIN_TEST("Empty", double)
	END_TEST(s.size() == 0)

	BEGIN_TEST("Insert", double)
		s.insert(4.4);
	END_TEST(s.size() == 1)

	BEGIN_TEST("Iterators empty", double)
		int cnt = 0;
		for (auto i = s.begin(); i != s.end(); ++i) {
			++cnt;
		}
	END_TEST(cnt == 0)

	BEGIN_TEST("Iterators non-empty", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		int cnt = 0;
		for (auto i = s.begin(); i != s.end(); ++i) {
			++cnt;
		}
	END_TEST(cnt == 3)

	BEGIN_TEST("Constant iterators", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		int cnt = 0;
		for (auto i = s.cbegin(); i != s.cend(); ++i) {
			++cnt;
		}
	END_TEST(cnt == 3)

	BEGIN_TEST("For-range empty", double)
		int cnt = 0;
		for (const auto& e : s) {
			(void)e;
			++cnt;
		}
	END_TEST(cnt == 0)

	BEGIN_TEST("For-range non-empty", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		int cnt = 0;
		for (const auto& e : s) {
			(void)e;
			++cnt;
		}
	END_TEST(cnt == 3)

	BEGIN_TEST("Find non-existent (empty set)", double)
		auto i = s.find(2.2);
	END_TEST(i == s.end())

	BEGIN_TEST("Find non-existent (non-empty set)", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		auto i = s.find(6.0);
	END_TEST(i == s.end())

	BEGIN_TEST("Find existent", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		auto i = s.find(55.14);
	END_TEST(i != s.end())

	BEGIN_TEST("Iterator access", double)
		initializeSet(s, 2.2);
		auto i = s.begin();
	END_TEST(*i == 2.2)

	BEGIN_TEST("Arrow operator", DDPair, DDPairHash)
		initializeSet(s, std::make_pair(2.2, 55.14));
		auto i = s.begin();
	END_TEST(i->first == 2.2 && i->second == 55.14)

	BEGIN_TEST("Erase non-existent (empty set)", double)
		auto i = s.erase(2.2);
	END_TEST(i == 0 && setEquals(s))

	BEGIN_TEST("Erase non-existent (non-empty set)", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		auto i = s.erase(6.0);
	END_TEST(i == 0 && setEquals(s, 2.2, 55.14, 92.01))

	BEGIN_TEST("Erase existent", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		auto i = s.erase(55.14);
	END_TEST(i == 1 && setEquals(s, 2.2, 92.01))

	BEGIN_TEST("Erase using iterator", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		auto i = s.find(55.14);
		s.erase(i);
	END_TEST(setEquals(s, 2.2, 92.01))

	BEGIN_TEST("Indexed access", double)
		initializeSet(s, 2.2, 55.14, 92.01);
		std::unordered_set<double> sRef({2.2, 55.14, 92.01});
		size_t eraseCount = 0;
		for (size_t i = 0; i < s.size(); ++i) {
			double f = s.atIndex(i);
			auto eraseRes = sRef.erase(f);
			eraseCount += eraseRes;
		}
	END_TEST(eraseCount == 3 && sRef.empty())

	std::cout << "Result: " << passCount << "/" << testCount << std::endl;
}
