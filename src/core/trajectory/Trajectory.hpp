/**
 * @file Trajectory.hpp
 * @author Tomáš Ludrovan
 * @brief Trajectory structure
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include <vector>

#include "math/Math.hpp"

// enum TrajectorySegmentType {
// 	TRAJECTORY_LINE_SEGMENT,
// };

struct TrajectorySegment {
	// double t0, t1;
	// TrajectorySegmentType type;
	// union {
		struct {
			double Ax, Ay, ux, uy;
		} lineSegment;
	// };
};

struct Trajectory {
	TrajectorySegment segment;

	/**
	 * @brief Calculates the minimum squared distance between 2 trajectories in time.
	 * 
	 * @details The returned value is the square of the Euclidean distance. The
	 *          reason to not calculate the actual distance is because it would
	 *          require one extra operation (sqrt), moreover, a rather slow
	 *          operation. The caller may calculate the sqrt itself, if it
	 *          desires to.
	 */
	double minSqdist(const Trajectory& rhs);
	/**
	 * @brief Returns the delta (i.e., vector) between initial and end point.
	 */
	void getDelta(double& x, double& y) const;
};

#endif // TRAJECTORY_HPP
