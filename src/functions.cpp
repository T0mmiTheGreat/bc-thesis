/**
 * @file functions.cpp
 * @author Tomáš Ludrovan
 * @brief 
 * @version 0.1
 * @date 2024-02-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "functions.hpp"

void popBackUtf8(std::string& s)
{
	// Looking at the bytes of UTF-8 characters, we can tell whether the byte
	// is the leftmost one or any other byte of the character just by looking
	// at its two most significant bits. If they're "00", "01" (ASCII), or "11"
	// (multibyte), the byte is the leftmost one, otherwise ("10") it is not.

	// Check if there's anything to pop
	if (s.empty()) return;

	size_t i = s.size() - 1;
	while ((i != 0) && ((s[i] & 0xC0) == 0x80)) {
		i--;
	}
	s.resize(i);
}
