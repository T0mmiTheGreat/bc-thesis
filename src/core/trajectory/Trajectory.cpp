/**
 * @file Trajectory.cpp
 * @author Tomáš Ludrovan
 * @brief Trajectory structure
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "core/trajectory/Trajectory.hpp"

#include "math/Math.hpp"

double Trajectory::minSqdist(const Trajectory& rhs) const
{
	// See documentation - too complex to describe here
	// FIXME?

	// auto Ap1x = this->segment.lineSegment.seg.source().x();
	// auto Ap1y = this->segment.lineSegment.seg.source().y();
	// auto Ap2x = rhs.segment.lineSegment.seg.source().x();
	// auto Ap2y = rhs.segment.lineSegment.seg.source().y();

	// auto up1x = this->segment.lineSegment.seg.to_vector().x();
	// auto up1y = this->segment.lineSegment.seg.to_vector().y();
	// auto up2x = rhs.segment.lineSegment.seg.to_vector().x();
	// auto up2y = rhs.segment.lineSegment.seg.to_vector().y();

	// auto a = CGAL::square(up1x - up2x) + CGAL::square(up1y - up2y);
	// auto b = 2*((Ap1x - Ap2x)*(up1x - up2x) + (Ap1y - Ap2y)*(up1y - up2y));
	// auto c = CGAL::square(Ap1x - Ap2x) + CGAL::square(Ap1y - Ap2y);

	// auto t = -b / 2*a;

	// // The time must be within the interval <0,1>. If the `t` is less than
	// // 0, we know that the minimum value for t => <0,1> is at t=0. Likewise,
	// // if t > 1, then the minimum value for t => <0,1> is at t=1.

	// if (t <= 0) {
	// 	return CGAL::to_double(c);
	// } else if (t >= 1) {
	// 	return CGAL::to_double(a + b + c);
	// } else {
	// 	return CGAL::to_double(CGAL::square(t)*a + t*b + c);
	// }
	(void)rhs;
	return 10000000.0;
}
