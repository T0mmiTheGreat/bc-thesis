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

template <typename T>
concept FloatingPoint = std::is_floating_point<T>::value;

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
 * @brief Returns 1 if `x` is positive, -1 if `x` is negative or 0 if it's zero.
 */
template <Scalar T>
inline constexpr T sign(T x)
{
	if (x > 0) {
		return 1;
	} else if (x < 0) {
		return -1;
	} else {
		return 0;
	}
}

/**
 * @brief Rounds `x` to the nearest multiple of `y`.
 * 
 * @example `roundToMultiple(14, 5) == 15`
 *          `roundToMultiple(16, 5) == 15`
 *          `roundToMultiple(20, 5) == 20`
 *          `roundToMultiple(-14, 5) == -15`
 *          `roundToMultiple(-16, 5) == -15`
 *          `roundToMultiple(-20, 5) == -20`
 *          `roundToMultiple(14, -5) == 15`
 *          `roundToMultiple(16, -5) == 15`
 *          `roundToMultiple(20, -5) == 20`
 *          `roundToMultiple(-14, -5) == -15`
 *          `roundToMultiple(-16, -5) == -15`
 *          `roundToMultiple(-20, -5) == -20`
 *          `roundToMultiple(9, 6) == 12`
 *          `roundToMultiple(-9, 6) == -12`
 *          `roundToMultiple(9, -6) == 12`
 *          `roundToMultiple(-9, -6) == -12`
 */
template <Integral T>
inline constexpr T roundToMultiple(T x, T y)
{
	T xabs = std::abs(x);
	// The sign of `y` does not matter.
	// `multiples(5) == multiples(-5)`, considering that `multiples(n)` is
	// a set (mathematical).
	// `15 == 3*5 == (-3)*(-5)`
	T yabs = std::abs(y);
	
	T resabs = ((xabs + yabs/2) / yabs) * yabs;

	return (x < 0 ? -resabs : resabs);
}

/**
 * @brief Rounds `x` to the nearest multiple of `y`.
 */
template <FloatingPoint T>
inline constexpr T roundfToMultiple(T x, T y)
{
	return std::round(x / y) * y;
}

/**
 * @brief Rounds `x` UP to the nearest multiple of `y`.
 * 
 * @example `roundUpToMultiple(14, 5) == 15`
 *          `roundUpToMultiple(16, 5) == 20`
 *          `roundUpToMultiple(20, 5) == 20`
 *          `roundUpToMultiple(-14, 5) == -10`
 *          `roundUpToMultiple(-16, 5) == -15`
 *          `roundUpToMultiple(-20, 5) == -20`
 *          `roundUpToMultiple(14, -5) == 15`
 *          `roundUpToMultiple(16, -5) == 20`
 *          `roundUpToMultiple(20, -5) == 20`
 *          `roundUpToMultiple(-14, -5) == -10`
 *          `roundUpToMultiple(-16, -5) == -15`
 *          `roundUpToMultiple(-20, -5) == -20`
 *          `roundUpToMultiple(9, 6) == 12`
 *          `roundUpToMultiple(-9, 6) == -6`
 *          `roundUpToMultiple(9, -6) == 12`
 *          `roundUpToMultiple(-9, -6) == -6`
 */
template <Integral T>
inline constexpr T roundUpToMultiple(T x, T y)
{
	T xabs = std::abs(x);
	// The sign of `y` does not matter (15 == 3*5 == (-3)*(-5))
	T yabs = std::abs(y);

	if (x < 0) {
		// Round absolute values DOWN

		// Don't forget the sign---`x` is negative!
		return -((xabs / yabs) * yabs);
	} else {
		// Round absolute values UP

		T xmody = xabs % yabs;

		// If `x` is multiple of `y`, return `x`, else round up
		return (xmody == 0 ? xabs : xabs + (yabs - xmody));
	}
}

/**
 * @brief Rounds `x` UP to the nearest multiple of `y`.
 */
template <FloatingPoint T>
inline constexpr T roundfUpToMultiple(T x, T y)
{
	T yabs = std::abs(y);
	return std::ceil(x / yabs) * yabs;
}

/**
 * @brief Rounds `x` DOWN to the nearest multiple of `y`.
 * 
 * @example `roundDownToMultiple(14, 5) == 10`
 *          `roundDownToMultiple(16, 5) == 15`
 *          `roundDownToMultiple(20, 5) == 20`
 *          `roundDownToMultiple(-14, 5) == -15`
 *          `roundDownToMultiple(-16, 5) == -20`
 *          `roundDownToMultiple(-20, 5) == -20`
 *          `roundDownToMultiple(14, -5) == 10`
 *          `roundDownToMultiple(16, -5) == 15`
 *          `roundDownToMultiple(20, -5) == 20`
 *          `roundDownToMultiple(-14, -5) == -15`
 *          `roundDownToMultiple(-16, -5) == -20`
 *          `roundDownToMultiple(-20, -5) == -20`
 *          `roundDownToMultiple(9, 6) == 6`
 *          `roundDownToMultiple(-9, 6) == -12`
 *          `roundDownToMultiple(9, -6) == 6`
 *          `roundDownToMultiple(-9, -6) == -12`
 */
template <Integral T>
inline constexpr T roundDownToMultiple(T x, T y)
{
	// This works because rounding negative `x` UP is equivalent to rounding
	// its absolute value DOWN and then negating it. Similarly, rounding
	// negative `x` DOWN is rounding its absolute value UP and then negating it.
	return -roundUpToMultiple(-x, y);
}

/**
 * @brief Rounds `x` DOWN to the nearest multiple of `y`.
 */
template <FloatingPoint T>
inline constexpr T roundfDownToMultiple(T x, T y)
{
	T yabs = std::abs(y);
	return std::floor(x / yabs) * yabs;
}

/**
 * @brief Calculates the squared distance between a point and a line segment.
 * 
 * @remark The returned value is the square of the distance. The caller may
 *         apply square root on the returned value if they need to.
 * 
 * @param px X coordinate of the point.
 * @param py Y coordinate of the point.
 * @param ls0x X coordinate of the first endpoint of the segment.
 * @param ls0y Y coordinate of the first endpoint of the segment.
 * @param ls1x X coordinate of the second endpoint of the segment.
 * @param ls1y Y coordinate of the second endpoint of the segment.
 */
double sqrDistancePointLineSegment(double px, double py, double ls0x,
	double ls0y, double ls1x, double ls1y);

/**
 * @brief Checks whether two line segments cross each other.
 * 
 * @param lsAp0x X coordinate of the first endpoint of the segment A.
 * @param lsAp0y Y coordinate of the first endpoint of the segment A.
 * @param lsAp1x X coordinate of the second endpoint of the segment A.
 * @param lsAp1y Y coordinate of the second endpoint of the segment A.
 * @param lsBp0x X coordinate of the first endpoint of the segment B.
 * @param lsBp0y Y coordinate of the first endpoint of the segment B.
 * @param lsBp1x X coordinate of the second endpoint of the segment B.
 * @param lsBp1y Y coordinate of the second endpoint of the segment B.
 */
bool isLineSegmentsCross(double lsAp0x, double lsAp0y, double lsAp1x,
	double lsAp1y, double lsBp0x, double lsBp0y, double lsBp1x, double lsBp1y);

/**
 * @brief Checks whether two vectors are antiparallel.
 * 
 * @details Two vectors are antiparallel if they have opposite direction but
 *          not necessarily the same magnitude
 * 
 * @param v1x X coordinate of the first vector.
 * @param v1y Y coordinate of the first vector.
 * @param v2x X coordinate of the second vector.
 * @param v2y Y coordinate of the second vector.
 * @return 
 */
bool areVectorsAntiparallel(double v1x, double v1y, double v2x, double v2y);

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
void triangulatePolygon(const std::vector<std::array<double,2>>& corners,
	std::vector<std::array<std::array<double,2>,3>>& triangles);

#endif // MATH_HPP
