/**
 * @file functions.hpp
 * @author Tomáš Ludrovan
 * @brief Common-use functions
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <cmath>

inline double powOptimized(double base, double exp) {
	if (base == 1.0 || exp == 1.0) {
		return base;
	} else if (exp == 0.0) {
		return 1.0;
	} else if (exp == 0.5) {
		return std::sqrt(base);
	} else if (base == 2.0) {
		return std::exp2(exp);
	} else {
		return std::pow(base, exp);
	}
}

#endif // FUNCTIONS_HPP
