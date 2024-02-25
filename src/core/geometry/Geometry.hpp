/**
 * @file Geometry.hpp
 * @author Tomáš Ludrovan
 * @brief Geometry-related definitions
 * @version 0.1
 * @date 2024-02-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kern;

typedef CGAL::Point_2<Kern>   Point_2;
typedef CGAL::Vector_2<Kern>  Vector_2;
typedef CGAL::Segment_2<Kern> Segment_2;

#endif // GEOMETRY_HPP
