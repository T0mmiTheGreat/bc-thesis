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

#ifndef OLD_TRAJECTORY_ALGORITHM

double Trajectory::calculateTStop(const Segment_2& seg, const Vector_2& mvVec)
{
	if (seg.source() == seg.target()) {
		// Zero length

		return 1.0;
	} else {
		assert(mvVec != CGAL::NULL_VECTOR);
		//assert(seg.direction() == mvVec.direction());

		auto segVector = seg.to_vector();

		double num = (segVector.x() != 0.0 ? segVector.x() : segVector.y());
		double denom = (mvVec.x() != 0.0 ? mvVec.x() : mvVec.y());

		return num / denom;
	}
}

Trajectory::Trajectory(const Segment_2& seg, const Vector_2& mvVec)
	: m_seg{seg}
	, m_tStop{calculateTStop(seg, mvVec)}
{}

Trajectory::Trajectory(const Point_2& pt)
	: Trajectory(Segment_2(pt, pt), CGAL::NULL_VECTOR)
{}

Trajectory::Trajectory()
	: Trajectory(Point_2())
{}

double Trajectory::minSqdist(const Trajectory& rhs) const
{
	// See documentation - too complex to describe here
	// FIXME

	auto Ap1x = this->seg().source().x();
	auto Ap1y = this->seg().source().y();
	auto Ap2x = rhs.seg().source().x();
	auto Ap2y = rhs.seg().source().y();

	auto up1x = this->seg().to_vector().x();
	auto up1y = this->seg().to_vector().y();
	auto up2x = rhs.seg().to_vector().x();
	auto up2y = rhs.seg().to_vector().y();

	auto a = CGAL::square(up1x - up2x) + CGAL::square(up1y - up2y);
	auto b = 2*((Ap1x - Ap2x)*(up1x - up2x) + (Ap1y - Ap2y)*(up1y - up2y));
	auto c = CGAL::square(Ap1x - Ap2x) + CGAL::square(Ap1y - Ap2y);

	auto t = -b / 2*a;

	// The time must be within the interval <0,1>. If the `t` is less than
	// 0, we know that the minimum value for t => <0,1> is at t=0. Likewise,
	// if t > 1, then the minimum value for t => <0,1> is at t=1.

	if (t <= 0) {
		return CGAL::to_double(c);
	} else if (t >= 1) {
		return CGAL::to_double(a + b + c);
	} else {
		return CGAL::to_double(CGAL::square(t)*a + t*b + c);
	}
}

#else // OLD_TRAJECTORY_ALGORITHM

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

#endif // OLD_TRAJECTORY_ALGORITHM
