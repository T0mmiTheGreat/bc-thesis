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
		static constexpr double CELL_SIZE = 16.0;

		const Size2d m_size;
		// Stored by rows
		std::vector<CellValue> m_cells;

		static Size2d initSize(const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		static std::vector<StageGridModel::CellValue> initCells(
			const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		static Point_2 getCellPosition(const CellKey& key);
		static double getCellNearestObstacleDistance(const CellKey& key,
			const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		size_t keyToIdx(const CellKey& key) const;
	public:
		GridInternal(const std::vector<StageObstacle>& obstacles,
			const Size2d& stageSize);
		bool isCellAt(const StageGridModel::CellKey& key) const;
		const StageGridModel::CellValue& at(
			const StageGridModel::CellKey& key) const;
		StageGridModel::CellKey getCellAtPos(const Point_2& p) const;
	};
public:
	friend class Cell;
	class Cell {
	public:
		friend struct Hash;
		struct Hash {
			size_t operator()(const Cell& key) const {
				return Point::Hash()(key.m_value.key);
			}
		};
	private:
		const StageGridModel::GridInternal& m_gridInternal;
		const CellValue& m_value;

		StageGridModel::CellKey getNeighKey(Direction8 neighDir) const {
			switch (neighDir) {
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
				case DIR8_NONE:
					break;
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
		Point_2 getPosition() const { return m_value.position; }
		double getNearestObstacleDistance() const {
			return m_value.nearestObstacleDistance;
		}
		bool hasNeighbor(Direction8 neighDir) const {
			CellKey neighKey = getNeighKey(neighDir);
			return m_gridInternal.isCellAt(neighKey);
		}
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
	StageGridModel::Cell getCellAt(const Point_2& p) const;
};

#endif // STAGEGRIDMODEL_HPP
