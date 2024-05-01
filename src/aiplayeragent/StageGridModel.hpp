/**
 * @file StageGridModel.hpp
 * @author Tomáš Ludrovan
 * @brief StageGridModel class
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEGRIDMODEL_HPP
#define STAGEGRIDMODEL_HPP

#include <cassert>
#include <array>
#include <vector>

#include "types.hpp"
#include "core/Common.hpp"
#include "core/geometry/Geometry.hpp"

/**
 * @brief Grid-like model of the stage.
 */
class StageGridModel {
private:
	typedef Point CellKey;
	struct CellValue {
		CellKey key;
		Point_2 position;
		double nearestObstacleDistance;
	};

	class GridInternal {
	private:
		// Cell width/height
		// It would make sense to keep this value equal to the maximum distance
		// a player may move by during a single turn. Given the implementation
		// in the Core it is MAX_SPEED * TICK_INTERVAL (1.0 * 17). Ideally, this
		// constant should be derived from that instead of having a fixed value,
		// but at this point I don't even care...
		static constexpr double CELL_SIZE = 17.0;

		const Size2d m_size;
		// Stored by rows
		std::vector<CellValue> m_cells;

		/**
		 * @brief Creates initial value for the `m_size` variable.
		 */
		static Size2d initSize(const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		/**
		 * @brief Creates initial value for the `m_cells` variable.
		 */
		static std::vector<StageGridModel::CellValue> initCells(
			const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		/**
		 * @brief Calcualtes the position of a cell with the given `key`.
		 */
		static Point_2 getCellPosition(const CellKey& key);
		/**
		 * @brief Calcualtes the square of the shortest distance between an
		 *        obstacle and a cell with the given `key`.
		 */
		static double getCellNearestObstacleDistance(const CellKey& key,
			const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		/**
		 * @brief Returns the index of a cell within the `m_cells` variable.
		 */
		size_t keyToIdx(const CellKey& key) const;
	public:
		GridInternal(const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		/**
		 * @brief Checks if a cell with the given `key` exists.
		 * 
		 * @details The cell exists iff its center is within the stage bounds
		 *          and it does not overlap with an obstacle.
		 */
		bool isCellAt(const StageGridModel::CellKey& key) const;
		/**
		 * @brief Returns the cell with the given `key`.
		 * 
		 * @remark UB if `!isCellAt(key)`.
		 */
		const StageGridModel::CellValue& at(
			const StageGridModel::CellKey& key) const;
		/**
		 * @brief Returns key of the cell within which's bounds lies the
		 *        `p` point.
		 */
		StageGridModel::CellKey getCellAtPos(const Point_2& p) const;
	};
public:
	friend class Cell;
	/**
	 * @brief Cell of the stage grid model.
	 */
	class Cell {
	public:
		friend struct Hash;
		/**
		 * @brief Hash function object for the cell.
		 */
		struct Hash {
			size_t operator()(const Cell& key) const {
				return Point::Hash()(key.m_value.key);
			}
		};
	private:
		const StageGridModel::GridInternal& m_gridInternal;
		const CellValue& m_value;

		/**
		 * @brief Returns key of the neighbor cell.
		 */
		StageGridModel::CellKey getNeighKey(Direction8 neighDir) const {
			switch (neighDir) {
				case DIR8_NONE: return StageGridModel::CellKey(
					m_value.key.x    , m_value.key.y    );
				case DIR8_N: return StageGridModel::CellKey(
					m_value.key.x    , m_value.key.y - 1);
				case DIR8_NE: return StageGridModel::CellKey(
					m_value.key.x + 1, m_value.key.y - 1);
				case DIR8_E: return StageGridModel::CellKey(
					m_value.key.x + 1, m_value.key.y    );
				case DIR8_SE: return StageGridModel::CellKey(
					m_value.key.x + 1, m_value.key.y + 1);
				case DIR8_S: return StageGridModel::CellKey(
					m_value.key.x    , m_value.key.y + 1);
				case DIR8_SW: return StageGridModel::CellKey(
					m_value.key.x - 1, m_value.key.y + 1);
				case DIR8_W: return StageGridModel::CellKey(
					m_value.key.x - 1, m_value.key.y    );
				case DIR8_NW: return StageGridModel::CellKey(
					m_value.key.x - 1, m_value.key.y - 1);
			}
			assert(((void)"Invalid Direction8 value", false));
			return StageGridModel::CellKey();
		}
	public:
		Cell(const StageGridModel::GridInternal& gridInternal,
			const CellKey& key)
			: m_gridInternal{gridInternal}
			, m_value{m_gridInternal.at(key)}
		{}
		/**
		 * @brief Returns the position of the center of the cell.
		 */
		Point_2 getPosition() const { return m_value.position; }
		/**
		 * @brief Returns the *squared* distance from the nearest obstacle.
		 */
		double getNearestObstacleDistance() const {
			return m_value.nearestObstacleDistance;
		}
		/**
		 * @brief Checks if there is a neighbor cell in the given direction.
		 */
		bool hasNeighbor(Direction8 neighDir) const {
			CellKey neighKey = getNeighKey(neighDir);
			return m_gridInternal.isCellAt(neighKey);
		}
		/**
		 * @brief Returns the neighbor cell in the given direction.
		 */
		Cell getNeighbor(Direction8 neighDir) const {
			CellKey neighKey = getNeighKey(neighDir);
			Cell res(m_gridInternal, neighKey);
			return res;
		}

		bool operator== (const Cell& rhs) const {
			return m_value.key == rhs.m_value.key;
		}
		bool operator!= (const Cell& rhs) const {
			return !this->operator==(rhs);
		}
	};
private:
	GridInternal m_gridInternal;
public:
	StageGridModel(const std::vector<StageObstacle>& obstacles,
		const Size2d& stageSize);
	/**
	 * @brief Returns the cell within which's bounds lies the `p` point.
	 */
	StageGridModel::Cell getCellAt(const Point_2& p) const;
};

#endif // STAGEGRIDMODEL_HPP
