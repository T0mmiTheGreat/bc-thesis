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
#include <vector>

template<typename T>
inline constexpr T sqr(T x)
{
	return x*x;
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
