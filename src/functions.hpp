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

// FIXME: constexpr
/**
 * @brief Calculates the square of `x`.
 */
inline double sqr(double x) {
	return x*x;
}

/**
 * @brief Calculates the distance between points [x1,y1], [x2,y2].
 */
inline double distance(double x1, double y1, double x2, double y2) {
	return std::sqrt(sqr(x2 - x1) + sqr(y2 - y1));
}

#endif // FUNCTIONS_HPP
