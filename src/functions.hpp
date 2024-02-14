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

/**
 * @brief Removes the last character from string treating it as UTF-8 string.
 */
void popBackUtf8(std::string& s);

template <typename... Args>
std::string concatStrings(const Args&... strings)
{
	std::string res = (std::string() + ... + strings);
	return res;
}

#endif // FUNCTIONS_HPP
