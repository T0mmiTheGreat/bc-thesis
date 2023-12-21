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

inline double sqr(double x) {
	return x*x;
}

inline double distance(double x1, double y1, double x2, double y2) {
	return std::sqrt(sqr(x2 - x1) + sqr(y2 - y1));
}

#endif // FUNCTIONS_HPP
