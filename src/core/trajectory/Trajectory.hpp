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

#include "core/geometry/Geometry.hpp"

#ifndef OLD_TRAJECTORY_ALGORITHM

class Trajectory { // TODO
private:
	Segment_2 m_seg;
	// The time at the end of the player trajectory. `1.0` is the end of the
	// turn (also means that there was no collision with an obstacle). Should
	// never be 0 -- if the length of `m_seg` is 0, the `m_tStop` should
	// be `1.0`.
	double m_tStop;

	/**
	 * @brief Calcualtes the `tStop` value.
	 * 
	 * @param seg Line segment representing the trajectory.
	 * @param mvVec Player movement vector.
	 * @return 
	 */
	static double calculateTStop(const Segment_2& seg, const Vector_2& mvVec);
public:
	/**
	 * @brief Constructs a new Trajectory object.
	 * 
	 * @param seg Line segment representing the trajectory.
	 * @param mvVec Player movement vector.
	 */
	Trajectory(const Segment_2& seg, const Vector_2& mvVec);
	/**
	 * @brief Constructs a new Trajectory object with zero length.
	 * 
	 * @param pt The start and end point of the trajectory.
	 */
	Trajectory(const Point_2& pt);
	/**
	 * @brief Constructs a new Trajectory object with zero length.
	 */
	Trajectory();

	Segment_2& seg() { return m_seg; }
	const Segment_2& seg() const { return m_seg; }

	double& tStop() { return m_tStop; }
	const double& tStop() const { return m_tStop; }

	Point_2 end() const { return m_seg.target(); }

	/**
	 * @brief Calculates the minimum squared distance between 2 trajectories in time.
	 * 
	 * @details The returned value is the square of the Euclidean distance. The
	 *          reason to not calculate the actual distance is because it would
	 *          require one extra operation (sqrt), moreover, a rather slow
	 *          operation. The caller may calculate the sqrt itself, if they
	 *          desire to.
	 */
	double minSqdist(const Trajectory& rhs) const;
};

#else // OLD_TRAJECTORY_ALGORITHM

class TrajectorySegment {
public:
	enum Type {
		TRAJECTORY_LINE_SEGMENT,
		TRAJECTORY_ARC,
	};
	struct LineSegment {
		Segment_2 seg;
	};
	struct Arc {
		Quadrant_arc_2 arc;
	};
private:
	Type m_type;
	LineSegment m_lineSegment;
	Arc m_arc;
public:
	/**
	 * @brief Constructs a new TrajectorySegment object of type `LINE_SEGMENT`.
	 */
	TrajectorySegment(LineSegment&& s)
		: m_type{TRAJECTORY_LINE_SEGMENT}
		, m_lineSegment{std::move(s)}
	{}
	/**
	 * @brief Constructs a new TrajectorySegment object of type `LINE_SEGMENT`.
	 */
	TrajectorySegment(const LineSegment& s)
		: m_type{TRAJECTORY_LINE_SEGMENT}
		, m_lineSegment{s}
	{}
	/**
	 * @brief Constructs a new TrajectorySegment object of type `ARC`.
	 */
	TrajectorySegment(Arc&& a)
		: m_type{TRAJECTORY_ARC}
		, m_arc{std::move(a)}
	{}
	/**
	 * @brief Constructs a new TrajectorySegment object of type `ARC`.
	 */
	TrajectorySegment(const Arc& a)
		: m_type{TRAJECTORY_ARC}
		, m_arc{a}
	{}

	/**
	 * @brief Constructs a new TrajectorySegment object of type `LINE_SEGMENT`.
	 */
	static TrajectorySegment constructLineSegment(const Segment_2& seg) {
		LineSegment s{.seg = seg};
		return TrajectorySegment(std::move(s));
	}
	/**
	 * @brief Constructs a new TrajectorySegment object of type `ARC`.
	 */
	static TrajectorySegment constructArc(const Quadrant_arc_2& arc) {
		Arc a{.arc = arc};
		return TrajectorySegment(std::move(a));
	}

	/**
	 * @brief Returns the type/shape of the segment.
	 */
	Type getType() const {
		return m_type;
	}

	/**
	 * @brief Gives access to the line segment.
	 * 
	 * @remark The type of the trajectory segment must be `LINE_SEGMENT`.
	 */
	LineSegment& lineSegment() {
		assert(m_type == TRAJECTORY_LINE_SEGMENT);
		return m_lineSegment;
	}
	/**
	 * @brief Gives access to the line segment.
	 * 
	 * @remark The type of the trajectory segment must be `LINE_SEGMENT`.
	 */
	const LineSegment& lineSegment() const {
		assert(m_type == TRAJECTORY_LINE_SEGMENT);
		return m_lineSegment;
	}
	
	/**
	 * @brief Gives access to the arc.
	 * 
	 * @remark The type of the trajectory segment must be `ARC`.
	 */
	Arc& arc() {
		assert(m_type == TRAJECTORY_ARC);
		return m_arc;
	}
	/**
	 * @brief Gives access to the arc.
	 * 
	 * @remark The type of the trajectory segment must be `ARC`.
	 */
	const Arc& arc() const {
		assert(m_type == TRAJECTORY_ARC);
		return m_arc;
	}

	/**
	 * @brief Returns the source point of the trajectory segment.
	 */
	Point_2 getPStart() const {
		switch (m_type) {
			case TRAJECTORY_LINE_SEGMENT: return m_lineSegment.seg.source();
			case TRAJECTORY_ARC: return m_arc.arc.source();
		}

		// Default
		return Point_2();
	}
	/**
	 * @brief Returns the target point of the trajectory segment.
	 */
	Point_2 getPEnd() const {
		switch (m_type) {
			case TRAJECTORY_LINE_SEGMENT: return m_lineSegment.seg.target();
			case TRAJECTORY_ARC: return m_arc.arc.target();
		}

		// Default
		return Point_2();
	}
	/**
	 * @brief Changes the source point of the trajectory segment.
	 */
	void setPStart(const Point_2& value) {
		switch (m_type) {
			case TRAJECTORY_LINE_SEGMENT:
				m_lineSegment.seg = Segment_2(value,
					m_lineSegment.seg.target());
				break;
			case TRAJECTORY_ARC:
				m_arc.arc = Quadrant_arc_2(m_arc.arc.center(), value,
					m_arc.arc.target());
				break;
		}
	}
	/**
	 * @brief Changes the target point of the trajectory segment.
	 */
	void setPEnd(const Point_2& value) {
		switch (m_type) {
			case TRAJECTORY_LINE_SEGMENT:
				m_lineSegment.seg = Segment_2(m_lineSegment.seg.source(),
					value);
				break;
			case TRAJECTORY_ARC:
				m_arc.arc = Quadrant_arc_2(m_arc.arc.center(),
					m_arc.arc.source(), value);
				break;
		}
	}
	
	/**
	 * @brief Checks if `p` lies on the segment.
	 */
	bool hasOn(const Point_2& p) const {
		switch (m_type) {
			case TRAJECTORY_LINE_SEGMENT:
				return m_lineSegment.seg.has_on(p);
			case TRAJECTORY_ARC:
				return m_arc.arc.has_on(p);
		}

		// Default
		return false;
	}
	
	/**
	 * @brief Returns the direction of the segment at the point `pt`.
	 */
	Direction_2 directionAt(const Point_2& pt) const {
		assert(hasOn(pt));
		switch (m_type) {
			case TRAJECTORY_LINE_SEGMENT:
				return m_lineSegment.seg.direction();
			case TRAJECTORY_ARC: {
				auto ori = m_arc.arc.orientation();
				auto v = Vector_2(m_arc.arc.center(), pt);
				auto dir = v.direction();
				auto res = dir.perpendicular(ori);
				return res;
			} break;

		}

		// Default
		return Direction_2();
	}
};

class Trajectory {
private:
	std::vector<TrajectorySegment> m_segments;
public:
	/**
	 * @brief Appends a new segment at the end of the trajectory.
	 */
	void push(const TrajectorySegment& seg) {
		m_segments.push_back(seg);
	}
	/**
	 * @brief Appends a new segment at the end of the trajectory.
	 */
	void push(TrajectorySegment&& seg) {
		m_segments.push_back(std::move(seg));
	}
	/**
	 * @brief Gives access to the last trajectory segment.
	 */
	TrajectorySegment& last() {
		return m_segments.back();
	}

	/**
	 * @brief Calculates the minimum squared distance between 2 trajectories in time.
	 * 
	 * @details The returned value is the square of the Euclidean distance. The
	 *          reason to not calculate the actual distance is because it would
	 *          require one extra operation (sqrt), moreover, a rather slow
	 *          operation. The caller may calculate the sqrt itself, if they
	 *          desire to.
	 */
	double minSqdist(const Trajectory& rhs) const;
};

#endif // OLD_TRAJECTORY_ALGORITHM

#endif // TRAJECTORY_HPP
