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
#include "stageeditor/Common.hpp"

/**
 * @brief Base class for stage editor actions.
 * 
 * @details Stage editor actions are stored in the action history and can be
 *          undone/redone. The history is a collection, so it stores members
 *          of the same data type or the same base class. This class is the
 *          base for all actions. They always have to be stored as pointer,
 *          likely a `shared_ptr`.
 * 
 *          In order to get the concrete action, the pointer has to be cast
 *          to that action object using `dynamic_cast()` or, in the case of
 *          `shared_ptr`, `std::dynamic_pointer_cast()`.
 * 
 *          The concrete action is obtained through the `getType()` virtual
 *          method. Typical use would be to `switch` the result of `getType()`
 *          and then cast the pointer to the corresponding object.
 */
class StageEditorAction {
public:
	enum ActionType {
		ACTION_NONE,
		ACTION_ADD_PLAYER,
		ACTION_PLACE_OBSTACLE_CORNER,
		ACTION_COMPLETE_OBSTACLE,
		ACTION_ACTIVATE_TOOL,
	};

	virtual ~StageEditorAction() {}

	/**
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;
};

/**
 * @brief No action was performed.
 * 
 * @details Pretty sure this will find its use.
 */
class StageEditorActionNone : public StageEditorAction {
public:
	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_NONE;
	}
};

/**
 * @brief A player object was added.
 */
class StageEditorActionAddPlayer : public StageEditorAction {
private:
	PointF m_pos;
	EditorOID m_oid;
public:
	/**
	 * @brief Constructs a new StageEditorActionAddPlayer object.
	 * 
	 * @param pos Player object position.
	 * @param oid Player object ID.
	 */
	StageEditorActionAddPlayer(const PointF& pos, EditorOID oid)
		: m_pos{pos}
		, m_oid{oid}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_ADD_PLAYER;
	}

	/**
	 * @brief Player object position.
	 */
	const PointF& getPos() const {
		return m_pos;
	}
	/**
	 * @brief Player object ID.
	 */
	EditorOID getOid() const {
		return m_oid;
	}
};

class StageEditorActionPlaceObstacleCorner : public StageEditorAction {
private:
	PointF m_pos;
public:
	/**
	 * @brief Constructs a new StageEditorActionPlaceObstacleCorner object.
	 * 
	 * @param pos New corner position.
	 */
	StageEditorActionPlaceObstacleCorner(const PointF& pos)
		: m_pos{pos}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_PLACE_OBSTACLE_CORNER;
	}

	/**
	 * @brief New corner position.
	 */
	const PointF& getPos() const {
		return m_pos;
	}
};

class StageEditorActionCompleteObstacle : public StageEditorAction {
private:
	PolygonF m_shape;
	EditorOID m_oid;
public:
	/**
	 * @brief Constructs a new StageEditorActionCompleteObstacle object.
	 * 
	 * @param shape Obstacle object shape.
	 * @param oid Obstacle object ID.
	 */
	StageEditorActionCompleteObstacle(const PolygonF& shape, EditorOID oid)
		: m_shape{shape}
		, m_oid{oid}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_COMPLETE_OBSTACLE;
	}

	/**
	 * @brief Obstacle object shape.
	 */
	const PolygonF& getShape() const {
		return m_shape;
	}
	/**
	 * @brief Obstacle object ID.
	 */
	EditorOID getOid() const {
		return m_oid;
	}
};

class StageEditorActionActivateTool : public StageEditorAction {
private:
	EditorTool m_oldTool;
	EditorTool m_newTool;
public:
	/**
	 * @brief Constructs a new StageEditorActionActivateTool object.
	 * 
	 * @param oldTool The tool which was replaced.
	 * @param newTool The activated tool.
	 */
	StageEditorActionActivateTool(EditorTool oldTool, EditorTool newTool)
		: m_oldTool{oldTool}
		, m_newTool{newTool}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_ACTIVATE_TOOL;
	}

	/**
	 * @brief The tool which was replaced.
	 */
	EditorTool getPrevTool() const {
		return m_oldTool;
	}
	/**
	 * @brief The activated tool.
	 */
	EditorTool getNewTool() const {
		return m_newTool;
	}
};

#endif // STAGEEDITORACTION_HPP
