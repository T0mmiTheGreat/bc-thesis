/**
 * @file Math.hpp
 * @author Tomáš Ludrovan
 * @brief Mathematical functions
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MATH_HPP
#define MATH_HPP

#include <array>
#include <cmath>
#include <type_traits>
#include <vector>

template <typename T>
concept Scalar = std::is_scalar<T>::value;

template <typename T>
concept Integral = std::is_integral<T>::value;

template <Scalar T>
inline constexpr T sqr(T x)
{
	return x*x;
}

template <Integral T>
inline constexpr bool isDivisibleBy(T divident, T divisor)
{
	return ((divident % divisor) == 0);
}

template <Integral T>
inline constexpr bool isOdd(T x)
{
	return !isDivisibleBy(x, 2);
}

template <Integral T>
inline constexpr bool isEven(T x)
{
	return isDivisibleBy(x, 2);
}

/**
 * @brief Rounds `x` to the neares multiple of `y`.
 * 
 * @tparam T Scalar type.
 */
template <Scalar T>
inline constexpr T roundToMultiple(T x, T y)
{
	if (std::is_integral_v<T>) {
		// Integral type

		return ((x + y/2) / y) * y;
	} else {
		// Floating point type or error

		return std::round(x / y) * y;
	}
}

/**
 * @brief Triangulates a polygon (converts it to a collection of triangles).
 * 
 * @param corners X and Y coordinates of the polygon corners.
 * @param triangles The generated triangles, each stored as array of corners,
 *                  which are arrays consisting of the corner's X and Y
 *                  coordinate.
 * 
 * @remark The polygon must be simple, i.e., without self-intersecting
 *         edges.
 */
void triangulatePolygon(std::vector<std::array<double,2>> corners,
	std::vector<std::array<std::array<double,2>,3>> triangles);

#endif // MATH_HPP
