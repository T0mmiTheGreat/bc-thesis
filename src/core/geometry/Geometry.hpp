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

#include <functional>
#include <vector>

#ifdef OLD_TRAJECTORY_ALGORITHM
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#else
#include <CGAL/Simple_cartesian.h>
#endif

#include "types.hpp"

#ifdef OLD_TRAJECTORY_ALGORITHM
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt Kern;
#else
typedef CGAL::Simple_cartesian<double> Kern;
#endif

typedef CGAL::Point_2<Kern>              Point_2;
typedef CGAL::Vector_2<Kern>             Vector_2;
typedef CGAL::Direction_2<Kern>          Direction_2;
typedef CGAL::Line_2<Kern>               Line_2;
typedef CGAL::Ray_2<Kern>                Ray_2;
typedef CGAL::Segment_2<Kern>            Segment_2;
typedef CGAL::Triangle_2<Kern>           Triangle_2;
typedef CGAL::Circle_2<Kern>             Circle_2;
typedef CGAL::Aff_transformation_2<Kern> Aff_transformation_2;

#ifdef OLD_TRAJECTORY_ALGORITHM
/**
 * @brief Circular arc covering at most one quarter of the circle.
 */
class Quadrant_arc_2 {
private:
	Point_2 m_center;
	Point_2 m_source;
	Point_2 m_target;
public:
	Quadrant_arc_2(const Point_2& center, const Point_2& source,
		const Point_2& target)
		: m_center{center}
		, m_source{source}
		, m_target{target}
	{
		// Both `source` and `target` must lie on a circle with the center at
		// the `center` point.
		assert(CGAL::squared_distance(center, source) 
			== CGAL::squared_distance(center, target));
		// The angle must be <= 90 deg
		assert(CGAL::angle(center, target, source) != CGAL::OBTUSE);
	}

	Quadrant_arc_2()
		: Quadrant_arc_2(Point_2(), Point_2(), Point_2())
	{}

	/**
	 * @brief Returns the center of the arc.
	 */
	Point_2 center() const { return m_center; }
	/**
	 * @brief Returns the first end point of the arc.
	 */
	Point_2 source() const { return m_source; }
	/**
	 * @brief Returns the second end point of the arc.
	 */
	Point_2 target() const { return m_target; }
	/**
	 * @brief Returns the radius of the arc, squared.
	 */
	decltype(auto) squared_radius() const {
		return CGAL::squared_distance(m_center, m_source);
	}
	/**
	 * @brief Returns the circle passing through the arc.
	 */
	Circle_2 supporting_circle() const {
		Circle_2 res(m_center, squared_radius());
		return res;
	}
	/**
	 * @brief Checks if `p` lies on the supporting circle of the arc.
	 */
	bool is_concyclic(const Point_2& p) const {
		return (CGAL::squared_distance(p, m_center) == squared_radius());
	}
	/**
	 * @brief Checks if `p` lies on the arc.
	 * 
	 * @details Faster than `has_on()`, because this method doesn't check
	 *          whether the point is concyclic with the arc.
	 */
	bool concyclic_has_on(const Point_2& p) const {
		assert(is_concyclic(p));
		if (m_source == p || m_target == p) {
			// Equal to source or target

			return true;
		} else {
			Line_2 l(m_center, p);

			// This will reliably check that the point lies between the source
			// and target as long as the length of the arc is less than `pi*r`.
			return (l.oriented_side(m_source) != l.oriented_side(m_target));
		}
	}
	/**
	 * @brief Checks if `p` lies on the arc.
	 */
	bool has_on(const Point_2& p) const {
		if (!is_concyclic(p)) {
			// Doesn't lie on the supporting circle

			return false;
		} else {
			return concyclic_has_on(p);
		}
	}
	/**
	 * @brief Returns the arc orientation.
	 */
	CGAL::Orientation orientation() const {
		Line_2 l(m_center, m_source);
		return l.oriented_side(m_target);
	}
};
#endif // OLD_TRAJECTORY_ALGORITHM

/**
 * @brief Converts `p` to CGAL point type.
 * 
 * @tparam T Scalar type.
 */
template <typename T>
inline Point_2 toCgalPoint(const PointGeneric<T>& p)
{
	return Point_2(p.x, p.y);
}

/**
 * @brief Converts `trg` to CGAL triangle type.
 */
inline Triangle_2 toCgalTriangle(const TriangleF& trg)
{
	Point_2 p(toCgalPoint(trg.corners[0]));
	Point_2 q(toCgalPoint(trg.corners[1]));
	Point_2 r(toCgalPoint(trg.corners[2]));
	
	return Triangle_2(p, q, r);
}

#ifdef OLD_TRAJECTORY_ALGORITHM
/**
 * @brief Helper function for `intersection` functions.
 * 
 * @details Functions defined here which look for intersection of two objects
 *          usually use other intersection functions and then filter out the
 *          points which don't lie on one of the objects. 
 * 
 * @param input Nothing, or a vector of points.
 * @param filter Function which returns `false` if a point should be filtered
 *               out.
 * @return Nothing if all points were filtered out, or a vector of points which
 *         passed through the filter.
 */
inline boost::optional<boost::variant<std::vector<Point_2>>> filterPoints(
	const boost::optional<boost::variant<std::vector<Point_2>>>& input,
	const std::function<bool(const Point_2&)>& filter)
{
	if (!input) {
		return boost::none;
	} else {
		auto inputCast = boost::get<std::vector<Point_2>>(&*input);

		std::vector<Point_2> res;
		for (const auto& pt : *inputCast) {
			if (filter(pt)) {
				res.push_back(pt);
			}
		}

		if (res.empty()) {
			return boost::none;
		} else {
			return boost::variant<std::vector<Point_2>>(res);
		}
	}
}

/**
 * @brief Returns the intersection of a circle bounds and a line.
 * 
 * @return Nothing if the objects don't intersect, or up to 2 intersection
 *         points.
 * 
 * @remark CGAL conventions says that the circle is considered a bounded region.
 *         To prevent inconsistency, the function explicitly statse that it
 *         looks for intersection with the circle bounds rather than the region.
 */
inline boost::optional<boost::variant<std::vector<Point_2>>> bounds_intersection(
    const Circle_2& c, const Line_2& l)
{
    // Circle diameter perpendicular to the line
    Line_2 diam = l.perpendicular(c.center());
    // Intersection of the diameter and the line
    auto isec1 = CGAL::intersection(l, diam);
    auto l_diam_isec = boost::get<Point_2>(&*isec1);
    // Distance from the line to circle center (squared)
    auto l_c_sqdist = CGAL::squared_distance(c.center(), *l_diam_isec);
    // Circle radius (squared)
    auto c_sqradius = c.squared_radius();
    // Length of the circle chord (squared, divided by 2)
    auto chord_sqlendiv2 = c_sqradius - l_c_sqdist;
    if (chord_sqlendiv2 < 0) {
        // No intersection
        return boost::none;
    }
    else if (chord_sqlendiv2 == 0) {
        // `l` is a tangent

		std::vector<Point_2> res({*l_diam_isec});
		return boost::variant<std::vector<Point_2>>(res);
    }
    else {
        // `l` is a secant

        // Direction of the line
        auto lvec = l.to_vector();
        // Length of the line's direction
        auto lvec_sqlen = lvec.squared_length();
        // Value to multiply the line's direction to get the desired length
        // (squared)
        auto len_factor_sqr = chord_sqlendiv2 / lvec_sqlen;
        // Value to multiply the line's direction to get the desired length
        auto len_factor = CGAL::sqrt(len_factor_sqr);
        // The vector between the intersections with the circle boundary
        // (divided by 2)
        auto chord_halfvec = lvec * len_factor;
        // Intersections with the boundary
        auto l_c_isec1 = *l_diam_isec + chord_halfvec;
        auto l_c_isec2 = *l_diam_isec - chord_halfvec;

		std::vector<Point_2> res({l_c_isec1, l_c_isec2});
		return boost::variant<std::vector<Point_2>>(res);
    }
}

/**
 * @brief Returns the intersection of a ray and circle bounds.
 * 
 * @return Nothing if the objects don't intersect, or up to 2 intersection
 *         points.
 * 
 * @remark CGAL conventions says that the circle is considered a bounded region.
 *         To prevent inconsistency, the function explicitly statse that it
 *         looks for intersection with the circle bounds rather than the region.
 */
inline boost::optional<boost::variant<std::vector<Point_2>>> bounds_intersection(
	const Ray_2& r, const Circle_2& c)
{
	// Find collisions of the circle and the ray's supporting line
	auto isec = bounds_intersection(c, r.supporting_line());

	// Keep only those which lie on the ray
	auto filter = [r](const Point_2& pt) {
		return r.collinear_has_on(pt);
	};
	auto res = filterPoints(isec, filter);

	return res;
}

/**
 * @brief Returns the intersection of a line segment and circle bounds.
 * 
 * @return Nothing if the objects don't intersect, or up to 2 intersection
 *         points.
 * 
 * @remark CGAL conventions says that the circle is considered a bounded region.
 *         To prevent inconsistency, the function explicitly statse that it
 *         looks for intersection with the circle bounds rather than the region.
 */
inline boost::optional<boost::variant<std::vector<Point_2>>> bounds_intersection(
	const Segment_2& s, const Circle_2& c)
{
	// Find collisions of the circle and the segment's supporting line
	auto isec = bounds_intersection(c, s.supporting_line());

	// Keep only those which lie on the segment
	auto filter = [s](const Point_2& pt) {
		return s.collinear_has_on(pt);
	};
	auto res = filterPoints(isec, filter);

	return res;
}

/**
 * @brief Returns the intersection of a quadrant arc and a line segment.
 * 
 * @return Nothing if the objects don't intersect, or up to 2 intersection
 *         points.
 */
inline boost::optional<boost::variant<std::vector<Point_2>>> intersection(
	const Quadrant_arc_2& a, const Segment_2& s)
{
	// Find collisions of the segment and the arc's supporting circle
	auto isec = bounds_intersection(s, a.supporting_circle());

	// Keep only those which lie on the arc
	auto filter = [a](const Point_2& pt) {
		return a.concyclic_has_on(pt);
	};
	auto res = filterPoints(isec, filter);

	return res;
}

/**
 * @brief Returns the intersection of a quadrant arc and circle bounds.
 * 
 * @return Nothing if the objects don't intersect, or up to 2 intersection
 *         points.
 * 
 * @remark CGAL conventions says that the circle is considered a bounded region.
 *         To prevent inconsistency, the function explicitly statse that it
 *         looks for intersection with the circle bounds rather than the region.
 */
inline boost::optional<boost::variant<std::vector<Point_2>>>
bounds_intersection(const Quadrant_arc_2& a, const Circle_2& c)
{
	if (a.center() == c.center()) {
		// Cocentric -- there is no radical line

		if (a.squared_radius() > c.squared_radius()) {
			// No intersection
			return boost::none;
		} else {
			// The arc either lies on the boundary or inside the boundary

			std::vector<Point_2> res({a.source(), a.target()});
			return boost::variant<std::vector<Point_2>>(res);
		}
	}

	// The radical line crosses through both intersections if they intersect,
	// their point of tangency if they touch, or nothing if they neither
	// intersect nor touch.
	auto rline = CGAL::radical_line(a.supporting_circle(), c);
	auto isec = bounds_intersection(c, rline);

	// Keep only the points which lie on the arc
	auto filter = [a](const Point_2& pt) {
		return a.concyclic_has_on(pt);
	};
	auto res = filterPoints(isec, filter);

	return res;
}
#endif // OLD_TRAJECTORY_ALGORITHM

#endif // GEOMETRY_HPP
