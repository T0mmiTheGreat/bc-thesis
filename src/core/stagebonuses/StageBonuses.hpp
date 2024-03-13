/**
 * @file StageBonuses.hpp
 * @author Tomáš Ludrovan
 * @brief StageBonuses class
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEBONUSES_HPP
#define STAGEBONUSES_HPP

#include <iterator>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types.hpp"
#include "core/Common.hpp"
#include "core/playerstate/PlayerState.hpp"
#include "utilities/unorderedSetWithIndexes/UnorderedSetWithIndexes.hpp"

class StageBonuses {
private:
	typedef UnorderedSetWithIndexes<PointF, PointF::Hash> PointFSet;

	// Minimum distance between a player and a bonus to allow generating
	// a bonus. This is the distance between their bounds, not centers.
	static constexpr double BONUS_PLAYER_MIN_DISTANCE = 80.0;
	// Spacing between valid bonus positions
	static constexpr double BONUS_GRID_CELL_SIZE = 5.0;

	double m_gridOffsetX;
	double m_gridOffsetY;
	std::unordered_map<BonusId, PointF> m_bonuses;
	// Positions where bonuses may be placed
	PointFSet m_validPositions;
	// Positions invalidated by players
	PointFSet m_playerInvalidPositions;
	// Positions invalidated by bonuses
	PointFSet m_bonusInvalidPositions;

	/**
	 * @brief Generates a unique bonus ID.
	 */
	BonusId generateBonusId() const;
	/**
	 * @brief Initializes the valid positions as a grid of points.
	 */
	void initBonusGrid(const Size2d& stageSize);
	/**
	 * @brief Removes positions from the valid positions collection which are
	 *        blocked by obstacles.
	 * 
	 * @param obstacles Obstacles on the stage.
	 */
	void invalidatePositionsByObstacles(
		const std::vector<StageObstacle>& obstacles);
	/**
	 * @brief Initializes the list of valid bonus positions.
	 * 
	 * @param obstacles Obstacles on the stage.
	 * @param stageSize
	 */
	void initValidPositions(const std::vector<StageObstacle>& obstacles,
		const Size2d& stageSize);
	
	/**
	 * @brief Changes the rectangle position and size to include exactly the
	 *        bonus position grid.
	 * 
	 * @details The passed iso-rectangle `R0`, modified iso-rectangle `R1` and
	 *          the set of all positions of bonuses `S` meet these requirements:
	 *              `intersection(R0, S) = intersection(R1, S)`
	 *              `R1.topLeft in S`
	 *              `R1.topRight in S`
	 *              `R1.bottomRight in S`
	 *              `R1.bottomLeft in S`
	 */
	RectF snapRectangle(const RectF& rect) const;
	/**
	 * @brief Generates a value for the `m_gridOffsetX/Y` variables.
	 * 
	 * @details The generated value is from interval [0, 5) with the fractional
	 *          part having only a few digits to allow representing big numbers
	 *          precisely.
	 */
	static double generateGridOffset();
public:
	/**
	 * @brief Constructs a new StageBonuses object.
	 * 
	 * @param obstacles Obstacles on the stage.
	 * @param stageSize
	 */
	StageBonuses(const std::vector<StageObstacle>& obstacles,
		const Size2d& stageSize);

	/**
	 * @brief Reports player states for invalidating the valid bonus positions.
	 */
	void reportPlayerStates(const std::vector<PlayerState>& states);

	/**
	 * @brief Generates a bonus at random position.
	 * 
	 * @return ID of the generated bonus.
	 * 
	 * @note Call `reportPlayerStates()` right before calling this function.
	 */
	BonusId generateBonus();
	/**
	 * @brief Removes a bonus.
	 * 
	 * @param id ID of the removed bonus.
	 */
	void clearBonus(BonusId id);

	/**
	 * @brief Returns collection of bonuses.
	 */
	const std::unordered_map<BonusId, PointF>& getBonuses() const;
};

#endif // STAGEBONUSES_HPP
