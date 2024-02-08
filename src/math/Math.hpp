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
void triangulatePolygon(std::vector<std::array<double,2>> corners,
	std::vector<std::array<std::array<double,2>,3>> triangles);

#endif // MATH_HPP
