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

double Trajectory::minSqdist(const Trajectory & rhs)
{
	// See documentation - too complex to describe here

	double Ap1x = this->segment.lineSegment.Ax;
	double Ap1y = this->segment.lineSegment.Ay;
	double Ap2x = rhs.segment.lineSegment.Ax;
	double Ap2y = rhs.segment.lineSegment.Ay;

	double up1x = this->segment.lineSegment.ux;
	double up1y = this->segment.lineSegment.uy;
	double up2x = rhs.segment.lineSegment.ux;
	double up2y = rhs.segment.lineSegment.uy;

	double a = sqr(up1x - up2x) + sqr(up1y - up2y);
	double b = 2*((Ap1x - Ap2x)*(up1x - up2x) + (Ap1y - Ap2y)*(up1y - up2y));
	double c = sqr(Ap1x - Ap2x) + sqr(Ap1y - Ap2y);

	double t = -b / 2*a;

	// The time must be within the interval <0,1>. If the `t` is less than
	// 0, we know that the minimum value for t => <0,1> is at t=0. Likewise,
	// if t > 1, then the minimum value for t => <0,1> is at t=1.

	if (t <= 0) {
		return c;
	} else if (t >= 1) {
		return a + b + c;
	} else {
		return sqr(t)*a + t*b + c;
	}
}

void Trajectory::getDelta(double& x, double& y) const
{
	x = segment.lineSegment.ux;
	y = segment.lineSegment.uy;
}
