/**
 * @file StageEditorAction.hpp
 * @author Tomáš Ludrovan
 * @brief StageEditorAction class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEEDITORACTION_HPP
#define STAGEEDITORACTION_HPP

#include "types.hpp"

class StageEditorAction {
public:
	enum ActionType {
		ACTION_ADD_PLAYER,
		ACTION_PLACE_OBSTACLE_CORNER,
		ACTION_COMPLETE_OBSTACLE,
	};

	virtual ~StageEditorAction() {}

	virtual ActionType getType() const = 0;
};

class StageEditorActionAddPlayer : public StageEditorAction {
private:
	PointF m_pos;    // Player position
	EditorOID m_oid; // Player object ID
public:

	StageEditorActionAddPlayer(const PointF& pos, EditorOID oid)
		: m_pos{pos}
		, m_oid{oid}
	{}

	ActionType getType() const override {
		return ACTION_ADD_PLAYER;
	}

	const PointF& getPos() const {
		return m_pos;
	}
	EditorOID getOid() const {
		return m_oid;
	}
};

class StageEditorActionPlaceObstacleCorner : public StageEditorAction {
private:
	PointF m_p0; // Previous corner position
	PointF m_p1; // New corner position
public:

	StageEditorActionPlaceObstacleCorner(const PointF& p0, const PointF& p1)
		: m_p0{p0}
		, m_p1{p1}
	{}

	ActionType getType() const override {
		return ACTION_PLACE_OBSTACLE_CORNER;
	}

	const PointF& getP0() const {
		return m_p0;
	}
	const PointF& getP1() const {
		return m_p1;
	}
};

class StageEditorActionCompleteObstacle : public StageEditorAction {
private:
	PolygonF m_shape; // Obstacle shape
	EditorOID m_oid;  // Obstacle object ID
public:

	StageEditorActionCompleteObstacle(const PolygonF& shape, EditorOID oid)
		: m_shape{shape}
		, m_oid{oid}
	{}

	ActionType getType() const override {
		return ACTION_COMPLETE_OBSTACLE;
	}

	const PolygonF& getShape() const {
		return m_shape;
	}
	EditorOID getOid() const {
		return m_oid;
	}
};

#endif // STAGEEDITORACTION_HPP
