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

// This enables constraints for bonus placement. Formerly, the rules stated
// that there should be some radius around a player where the bonuses must not
// spawn, and that bonuses must not spawn atop each other. With this disabled,
// there is only one constraint: bonuses may not spawn inside obstacles. The
// reason to disable this was because the previous solution caused quite a big
// lag; plus it didn't work...
//#define ENABLE_BONUS_CONSTRAINTS

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

#ifdef ENABLE_BONUS_CONSTRAINTS
	// Minimum distance between a player and a bonus to allow generating
	// a bonus. This is the distance between their bounds, not centers.
	static constexpr double BONUS_PLAYER_MIN_DISTANCE = 80.0;
#endif // ENABLE_BONUS_CONSTRAINTS
	// Spacing between valid bonus positions
	static constexpr double BONUS_GRID_CELL_SIZE = 5.0;

	double m_gridOffsetX;
	double m_gridOffsetY;
	std::unordered_map<BonusId, PointF> m_bonuses;

	// Rules:
	//  This must be always true:
	//    `intersection(m_vP, union(m_pIP, m_bIP)) = {}`
	//  When set S: `in(S, {m_pIP, m_bIP})` is being "validated":
	//    `m_vP <- union(m_vP, S - Sc): Sc = complement(S, union(m_pIP, mbIP))`
	//    `S <- {}`

	// Positions where bonuses may be placed
	PointFSet m_validPositions;
#ifdef ENABLE_BONUS_CONSTRAINTS
	// Positions invalidated by players
	PointFSet m_playerInvalidPositions;
	// Positions invalidated by bonuses
	PointFSet m_bonusInvalidPositions;
#endif // ENABLE_BONUS_CONSTRAINTS

	/**
	 * @brief Initializes the `m_gridOffsetX/Y` variables.
	 */
	void initGridOffsets();
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
#ifdef ENABLE_BONUS_CONSTRAINTS
	/**
	 * @brief Invalidates points within a circular area and inserts them to
	 *        `invalidatedOut` set.
	 * 
	 * @param center Center of the circular area.
	 * @param radius Radius of the area.
	 * @param invalidatedOut Set which will contain the invalidated positions.
	 */
	void invalidateCircle(const PointF& center, double radius,
		PointFSet& invalidatedOut);
#endif // ENABLE_BONUS_CONSTRAINTS
	
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
	 * @brief Returns a valid position which is nearest to `pt`.
	 */
	PointF snapPoint(const PointF& pt) const;

	/**
	 * @brief Generates a unique bonus ID.
	 */
	BonusId generateBonusId() const;
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

#ifdef ENABLE_BONUS_CONSTRAINTS
	/**
	 * @brief Reports player states for invalidating the valid bonus positions.
	 */
	void reportPlayerStates(const std::vector<PlayerState>& states);
#endif // ENABLE_BONUS_CONSTRAINTS

	/**
	 * @brief Generates a bonus at random position.
	 * 
	 * @return ID of the generated bonus, or BONUS_ID_NULL if the bonus cannot
	 *         be generated.
	 * 
	 * @note (#ifdef ENABLE_BONUS_CONSTRAINTS) Call `reportPlayerStates()` right
	 *       before calling this function.
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
