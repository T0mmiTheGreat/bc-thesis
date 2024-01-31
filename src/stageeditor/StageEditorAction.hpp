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

#include <memory>
#include <type_traits>

#include "stageeditor/Common.hpp"

class StageEditorAction : public std::enable_shared_from_this<StageEditorAction> {
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
public:
	PointF pos;    // Player position
	EditorOID oid; // Player object ID

	StageEditorActionAddPlayer(const PointF& pos, EditorOID oid)
		: pos{pos}
		, oid{oid}
	{}

	ActionType getType() const override {
		return ACTION_ADD_PLAYER;
	}
};

class StageEditorActionPlaceObstacleCorner : public StageEditorAction {
public:
	PointF p0; // Previous corner position
	PointF p1; // New corner position

	StageEditorActionPlaceObstacleCorner(const PointF& p0, const PointF& p1)
		: p0{p0}
		, p1{p1}
	{}

	ActionType getType() const override {
		return ACTION_PLACE_OBSTACLE_CORNER;
	}
};

class StageEditorActionCompleteObstacle : public StageEditorAction {
public:
	PolygonF shape; // Obstacle shape
	EditorOID oid;  // Obstacle object ID

	StageEditorActionCompleteObstacle(const PolygonF& shape, EditorOID oid)
		: shape{shape}
		, oid{oid}
	{}

	ActionType getType() const override {
		return ACTION_COMPLETE_OBSTACLE;
	}
};

#endif // STAGEEDITORACTION_HPP
