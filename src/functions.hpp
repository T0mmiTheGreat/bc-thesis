/**
 * @file functions.hpp
 * @author Tomáš Ludrovan
 * @brief Common-use functions
 * @version 0.1
 * @date 2024-02-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Removes the last character from string treating it as UTF-8 string.
 */
void popBackUtf8(std::string& s);

template <typename... Args>
inline std::string concatStrings(const Args&... strings)
{
	std::string res = (std::string() + ... + strings);
	return res;
}

/**
 * @brief Inserts copies of elements from `v2` at the end of `v1`.
 * 
 * @tparam T Element type.
 * @param v1 Vector to insert into.
 * @param v2 Vector to insert from.
 * 
 * @remark `std::vector<T>.append_range()` was added in C++23. At the time this
 *         project was made GCC supported only C++20.
 */
template <typename T>
inline void vectorAppendRange(std::vector<T>& v1, const std::vector<T>& v2)
{
	v1.reserve(v1.size() + v2.size());
	for (const T& it : v2) {
		v1.push_back(it);
	}
}

/**
 * @brief Checks whether vector `v` contains element `it`.
 * 
 * @tparam T Element type. Must implement `operator ==`.
 * 
 * @remark O(n) time complexity.
 */
template <typename T>
inline bool vectorContains(const std::vector<T>& v, const T& it)
{
	for (const T& it_ : v) {
		if (it == it_) return true;
	}
	return false;
}

/**
 * @brief Checks whether vector `v` contains duplicate values.
 * 
 * @tparam T Element type. Must implement `operator ==`.
 * 
 * @remark O(n^2) time complexity (I think...?).
 */
template <typename T>
inline bool vectorHasDuplicates(const std::vector<T>& v)
{
	if (v.empty()) return false;

	for (size_t i = 0; i < v.size(); i++) {
		for (size_t j = i + 1; j < v.size(); j++) {
			if (v[i] == v[j]) return true;
		}
	}

	return false;
}

#endif // FUNCTIONS_HPP
