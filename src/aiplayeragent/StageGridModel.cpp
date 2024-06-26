/**
 * @file StageGridModel.cpp
 * @author Tomáš Ludrovan
 * @brief StageGridModel class
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/StageGridModel.hpp"

#include <algorithm>

//#define LOG_CELLS

#ifdef LOG_CELLS
#include <fstream>
#include <iomanip>
#include <iostream>
#endif // LOG_CELLS

Size2d StageGridModel::GridInternal::initSize(
	const std::vector<StageObstacle>& obstacles, const Size2d& stageSize)
{
	(void)obstacles;

	// ceiling(x / CELL_SIZE)
	Size2d res(
		(stageSize.w - 1) / static_cast<int>(CELL_SIZE) + 1,
		(stageSize.h - 1) / static_cast<int>(CELL_SIZE) + 1
	);
	return res;
}

std::vector<StageGridModel::CellValue> StageGridModel::GridInternal::initCells(
	const std::vector<StageObstacle>& obstacles, const Size2d& stageSize)
{
	Size2d gridSize = initSize(obstacles, stageSize);
	std::vector<StageGridModel::CellValue> res;
	res.reserve(gridSize.w * gridSize.h);

	for (int row = 0; row < gridSize.h; ++row) {
		for (int col = 0; col < gridSize.w; ++col) {
			CellKey newCellKey(col, row);
			CellValue newCellValue = {
				newCellKey,                  // key
				getCellPosition(newCellKey), // position
				getCellNearestObstacleDistance(newCellKey, obstacles,
					stageSize),              // nearestObstacleDistance
			};
			res.push_back(std::move(newCellValue));
		}
	}

#ifdef LOG_CELLS
	std::ofstream log("stage-grid-model.log");
	for (int row = 0; row < gridSize.h; ++row) {
		for (int col = 0; col < gridSize.w; ++col) {
			log << std::setw(5) << std::setfill(' ') << static_cast<int>(res[row * gridSize.w + col].nearestObstacleDistance) << ' ';
		}
		log << '\n';
	}
#endif // LOG_CELLS

	return res;
}

Point_2 StageGridModel::GridInternal::getCellPosition(const CellKey& key)
{
	Point_2 res(
		// Top left corner  // Center
		key.x * CELL_SIZE + (CELL_SIZE / 2),
		key.y * CELL_SIZE + (CELL_SIZE / 2)
	);
	return res;
}

double StageGridModel::GridInternal::getCellNearestObstacleDistance(
	const CellKey& key, const std::vector<StageObstacle>& obstacles,
	const Size2d& stageSize)
{
	// The distance is measured as the squared distance from the cell center to
	// the stage bounds or an obstacle. If it overlaps with an obstacle, the
	// distance is 0. This is because of thin obstacles -- if an obstacle is
	// thin enough, small players might think there is a path between cells
	// separated by such obstacle.

	Point_2 pos = getCellPosition(key);
	// Square PQRS is the square of the cell
	Point_2 p(key.x * CELL_SIZE, key.y * CELL_SIZE);             // top left
	Point_2 q((key.x + 1) * CELL_SIZE, key.y * CELL_SIZE);       // top right
	Point_2 r((key.x + 1) * CELL_SIZE, (key.y + 1) * CELL_SIZE); // bottom right
	Point_2 s(key.x * CELL_SIZE, (key.y + 1) * CELL_SIZE);       // bottom left

	// CGAL doesn't have a function to calculate the distance between a square
	// and a triangle, so we have to split the square to two triangles.

	// Top right triangle
	Triangle_2 trg1(p, q, r);
	// Bottom left triangle
	Triangle_2 trg2(r, s, p);
	
	double lSqdist = sqr(pos.x());
	double tSqdist = sqr(pos.y());
	double rSqdist = sqr(stageSize.w - pos.x());
	double bSqdist = sqr(stageSize.h - pos.y());

	// Result is first initialized as the minimum distance from the stage bounds
	double res = std::min({lSqdist, tSqdist, rSqdist, bSqdist});

	for (const auto& obstacle : obstacles) {
		auto obstTrg = toCgalTriangle(obstacle);
		if ((CGAL::squared_distance(trg1, obstTrg) == 0.0)
			|| (CGAL::squared_distance(trg2, obstTrg) == 0.0))
		{
			// Cell overlaps with the obstacle
			return 0.0;
		}
		// Otherwise, the distance is measured from the cell center.
		double sqdist = CGAL::squared_distance(pos, toCgalTriangle(obstacle));
		// res = min(res, sqdist)
		if (res > sqdist) res = sqdist;
	}

	return res;
}

size_t StageGridModel::GridInternal::keyToIdx(const CellKey& key) const
{
	       // skip rows       // skip columns
	return key.y * m_size.w + key.x;
}

StageGridModel::GridInternal::GridInternal(
	const std::vector<StageObstacle>& obstacles, const Size2d& stageSize)
	: m_size{initSize(obstacles, stageSize)}
	, m_cells{initCells(obstacles, stageSize)}
{}

bool StageGridModel::GridInternal::isCellAt(
	const StageGridModel::CellKey& key) const
{
	return (0 <= key.x && key.x < m_size.w) // Within vertical bounds
		&& (0 <= key.y && key.y < m_size.h) // Within horizontal bounds
		&& (at(key).nearestObstacleDistance != 0.0); // Not overlapping with
		                                             // and obstacle
}

const StageGridModel::CellValue& StageGridModel::GridInternal::at(
	const StageGridModel::CellKey& key) const
{
	return m_cells.at(keyToIdx(key));
}

StageGridModel::CellKey StageGridModel::GridInternal::getCellAtPos(
	const Point_2& p) const
{
	// floor(xy / CELL_SIZE)
	CellKey res(
		static_cast<int>(p.x() / CELL_SIZE),
		static_cast<int>(p.y() / CELL_SIZE)
	);
	return res;
}

StageGridModel::StageGridModel(const std::vector<StageObstacle>& obstacles,
	const Size2d& stageSize)
	: m_gridInternal(obstacles, stageSize)
{}

StageGridModel::Cell StageGridModel::getCellAt(const Point_2& p) const
{
	Cell res(m_gridInternal, m_gridInternal.getCellAtPos(p));
	return res;
}
