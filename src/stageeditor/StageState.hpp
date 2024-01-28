/**
 * @file StageState.hpp
 * @author Tomáš Ludrovan
 * @brief StageState structure
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGESTATE_HPP
#define STAGESTATE_HPP

#include <vector>

#include "types.hpp"

struct StageState {
	int width;
	int height;
	std::vector<PolygonF> obstacles;
	std::vector<PointF> players;
};

#endif // STAGESTATE_HPP
