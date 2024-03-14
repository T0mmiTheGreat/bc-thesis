/**
 * @file StageBonuses.cpp
 * @author Tomáš Ludrovan
 * @brief StageBonuses class
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "core/stagebonuses/StageBonuses.hpp"

#include "functions.hpp"

#include <ostream>

BonusId StageBonuses::generateBonusId() const
{
	static BonusId lastBonusId = 0;
	return ++lastBonusId;
}

void StageBonuses::initBonusGrid(const Size2d& stageSize)
{
	m_gridOffsetX = generateGridOffset();
	m_gridOffsetY = generateGridOffset();

	RectF stageBounds(BONUS_RADIUS, BONUS_RADIUS,
		stageSize.w - (2*BONUS_RADIUS), stageSize.h - (2*BONUS_RADIUS));
	stageBounds = snapRectangle(stageBounds);
	PointF pt;

	for (pt.y = stageBounds.y; pt.y <= stageBounds.getBottom();
		pt.y += BONUS_GRID_CELL_SIZE)
	{
		for (pt.x = stageBounds.x; pt.x <= stageBounds.getRight();
			pt.x += BONUS_GRID_CELL_SIZE)
		{
			m_validPositions.insert(pt);
		}
	}
}

void StageBonuses::invalidatePositionsByObstacles(
	const std::vector<StageObstacle>& obstacles)
{
	RectF bbox;
	PointF pt;

	for (const auto& obstacle: obstacles) {
		bbox = obstacle.getBoundingBox();
		bbox = snapRectangle(bbox);
		bbox = bbox.getInflated(2*BONUS_RADIUS);

		for (pt.y = bbox.y; pt.y <= bbox.getBottom();
			pt.y += BONUS_GRID_CELL_SIZE)
		{
			for (pt.x = bbox.x; pt.x <= bbox.getRight();
				pt.x += BONUS_GRID_CELL_SIZE)
			{
				if (obstacle.sqrDistance(pt) < sqr(BONUS_RADIUS)) {
					m_validPositions.erase(pt);
				}
			}
		}
	}
}

void StageBonuses::initValidPositions(
	const std::vector<StageObstacle>& obstacles, const Size2d& stageSize)
{
	initBonusGrid(stageSize);
	invalidatePositionsByObstacles(obstacles);

}

RectF StageBonuses::snapRectangle(const RectF& rect) const
{
	double l = roundfUpToMultiple(rect.x - m_gridOffsetX, BONUS_GRID_CELL_SIZE)
		+ m_gridOffsetX;
	double t = roundfUpToMultiple(rect.y - m_gridOffsetY, BONUS_GRID_CELL_SIZE)
		+ m_gridOffsetY;
	double r = roundfDownToMultiple(rect.getRight() - m_gridOffsetX,
		BONUS_GRID_CELL_SIZE) + m_gridOffsetX;
	double b = roundfDownToMultiple(rect.getBottom() - m_gridOffsetY,
		BONUS_GRID_CELL_SIZE) + m_gridOffsetY;
	
	RectF res(PointF(l, t), PointF(r, b));
	return res;
}

double StageBonuses::generateGridOffset()
{
	// The number of binary digits in the fractional part of the generated
	// number. The value should not be too high in order to represent big
	// enough numbers. (Well not that big, about 2^16 should be enough, but
	// the `DECIMAL_PLACES` doesn't have to be too high either.)
	static constexpr int DECIMAL_PLACES = 4;
	// The number of unique fractional parts of the result
	static constexpr int FRACTION_COUNT = (1 << DECIMAL_PLACES);
	// The number of unique results
	static constexpr int VALUE_COUNT =
		static_cast<int>(BONUS_GRID_CELL_SIZE) * FRACTION_COUNT;

	int r = rand() % VALUE_COUNT;
	double res = static_cast<double>(r) / static_cast<double>(FRACTION_COUNT);
	return res;
}

StageBonuses::StageBonuses(const std::vector<StageObstacle>& obstacles,
	const Size2d& stageSize)
{
	initValidPositions(obstacles, stageSize);
}

void StageBonuses::reportPlayerStates(const std::vector<PlayerState>& states)
{
	(void)states;
	// TODO
}

BonusId StageBonuses::generateBonus()
{
	BonusId id = generateBonusId();
	std::uniform_int_distribution<size_t> distrib(0, m_validPositions.size());
	size_t posIdx = distrib(getRNGine());
	PointF pt = m_validPositions.atIndex(static_cast<size_t>(posIdx));
	m_bonuses[id] = pt;
	return id;
}

void StageBonuses::clearBonus(BonusId id)
{
	(void)id;
	// TODO
}

const std::unordered_map<BonusId,PointF>& StageBonuses::getBonuses() const
{
	return m_bonuses;
}
