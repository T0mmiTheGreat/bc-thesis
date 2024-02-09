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
#include <unordered_set>
#include <vector>

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
		ACTION_MULTIPLE,
		ACTION_ADD_PLAYER,
		ACTION_PLACE_OBSTACLE_CORNER,
		ACTION_COMPLETE_OBSTACLE,
		ACTION_ABORT_OBSTACLE,
		ACTION_ACTIVATE_TOOL,
		ACTION_SELECT_PLAYER_OBJECT,
		ACTION_SELECT_OBSTACLE_OBJECT,
		ACTION_DESELECT_PLAYER_OBJECT,
		ACTION_DESELECT_OBSTACLE_OBJECT,
		ACTION_BEGIN_DRAG_SELECTED,
		ACTION_MOVE_PLAYER_OBJECT,
		ACTION_MOVE_OBSTACLE_OBJECT,
	};

	virtual ~StageEditorAction() {}

	/**
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;
};

/**
 * @brief Class which inherits from StageEditorAction.
 */
template <class T>
concept StageEditorActionDerived = std::is_base_of<StageEditorAction, T>::value;

/**
 * @brief Multiple actions performed at once.
 * 
 * @note The order of actions is significant.
 */
class StageEditorActionMultiple : public StageEditorAction {
public:
	typedef std::vector<std::shared_ptr<StageEditorAction>> ActionsCollection;
private:
	ActionsCollection m_actions;
public:
	/**
	 * @brief Constructs a new StageEditorActionMultiple object.
	 * 
	 * @tparam Args Any descendant of StageEditorAction.
	 * @param actions Any number of actions.
	 */
	template <StageEditorActionDerived... Args>
	StageEditorActionMultiple(std::shared_ptr<Args>... actions) {
		m_actions.reserve(sizeof...(actions));
		(m_actions.push_back(actions), ...);
	}

	StageEditorActionMultiple(const ActionsCollection& actions)
		: m_actions{actions}
	{}

	StageEditorActionMultiple(ActionsCollection&& actions)
		: m_actions{std::move(actions)}
	{}
	
	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_MULTIPLE;
	}

	/**
	 * @brief The actions list (their order is significant).
	 */
	const ActionsCollection& getActions() const {
		return m_actions;
	}
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

/**
 * @brief Construction of an obstacle has been aborted, removing all placed
 *        corners of the unfinished obstacle.
 */
class StageEditorActionAbortObstacle : public StageEditorAction {
private:
	std::vector<PointF> m_obstacleCorners;
public:
	/**
	 * @brief Constructs a new StageEditorActionAbortObstacle object.
	 * 
	 * @param obstacleCorners Corners of the aborted obstacle.
	 */
	StageEditorActionAbortObstacle(const std::vector<PointF>& obstacleCorners)
		: m_obstacleCorners{obstacleCorners}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_ABORT_OBSTACLE;
	}

	/**
	 * @brief Corners of the aborted obstacle.
	 */
	const std::vector<PointF>& getObstacleCorners() const {
		return m_obstacleCorners;
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

/**
 * @brief Abstract class representing action of selection or deselection of
 *        an object.
 */
class StageEditorActionSelectDeselectObjectBase : public StageEditorAction {
private:
	EditorOID m_oid;
public:
	/**
	 * @brief Constructs a new StageEditorActionSelectDeselectObjectBase object.
	 * 
	 * @param oid The object which was selected or deselected.
	 */
	StageEditorActionSelectDeselectObjectBase(EditorOID oid)
		: m_oid{oid}
	{}

	/**
	 * @brief The object which was selected or deselected.
	 */
	EditorOID getOid() const {
		return m_oid;
	}
};

class StageEditorActionSelectPlayerObject : public StageEditorActionSelectDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionSelectPlayerObject object.
	 * 
	 * @param oid The object which was selected.
	 */
	StageEditorActionSelectPlayerObject(EditorOID oid)
		: StageEditorActionSelectDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_SELECT_PLAYER_OBJECT;
	}
};

class StageEditorActionSelectObstacleObject : public StageEditorActionSelectDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionSelectObstacleObject object.
	 * 
	 * @param oid The object which was selected.
	 */
	StageEditorActionSelectObstacleObject(EditorOID oid)
		: StageEditorActionSelectDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_SELECT_OBSTACLE_OBJECT;
	}
};

class StageEditorActionDeselectPlayerObject : public StageEditorActionSelectDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionDeselectPlayerObject object.
	 * 
	 * @param oid The object which was deselected.
	 */
	StageEditorActionDeselectPlayerObject(EditorOID oid)
		: StageEditorActionSelectDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_DESELECT_PLAYER_OBJECT;
	}
};

class StageEditorActionDeselectObstacleObject : public StageEditorActionSelectDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionDeselectObstacleObject object.
	 * 
	 * @param oid The object which was deselected.
	 */
	StageEditorActionDeselectObstacleObject(EditorOID oid)
		: StageEditorActionSelectDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_DESELECT_OBSTACLE_OBJECT;
	}
};

class StageEditorBeginDragSelected : public StageEditorAction {
private:
	PointF m_where;
	std::unordered_set<EditorOID> m_playerOids;
	std::unordered_set<EditorOID> m_obstacleOids;
public:
	/**
	 * @brief Constructs a new StageEditorBeginDragSelected object.
	 * 
	 * @param where Position where the drag started.
	 * @param playerOids OIDs of the selected players.
	 * @param obstacleOids OIDs of the selected obstacles.
	 */
	StageEditorBeginDragSelected(
		const PointF& where,
		const std::unordered_set<EditorOID>& playerOids,
		const std::unordered_set<EditorOID>& obstacleOids)
		: m_where(where)
		, m_playerOids{playerOids}
		, m_obstacleOids{obstacleOids}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_BEGIN_DRAG_SELECTED;
	}

	/**
	 * @brief Position where the drag started.
	 */
	const PointF& getWhere() const {
		return m_where;
	}

	/**
	 * @brief OIDs of the selected players.
	 */
	const std::unordered_set<EditorOID>& getPlayerOids() const {
		return m_playerOids;
	}

	/**
	 * @brief OIDs of the selected obstacles.
	 */
	const std::unordered_set<EditorOID>& getObstacleOids() const {
		return m_obstacleOids;
	}
};

class StageEditorActionMoveObjectBase : public StageEditorAction {
private:
	EditorOID m_oid;
	double m_dx;
	double m_dy;
public:
	/**
	 * @brief Constructs a new StageEditorActionMoveObjectBase object.
	 * 
	 * @param oid The object which was moved.
	 * @param dx Increment in X axis.
	 * @param dy Increment in Y axis.
	 */
	StageEditorActionMoveObjectBase(EditorOID oid, double dx, double dy)
		: m_oid{oid}
		, m_dx{dx}
		, m_dy{dy}
	{}

	/**
	 * @brief The object which was moved.
	 */
	EditorOID getOid() const {
		return m_oid;
	}

	/**
	 * @brief Increment in X axis.
	 */
	double getDx() const {
		return m_dx;
	}

	/**
	 * @brief Increment in Y axis.
	 */
	double getDy() const {
		return m_dy;
	}
};

class StageEditorActionMovePlayerObject : public StageEditorActionMoveObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionMovePlayerObject object.
	 * 
	 * @param oid The object which was moved.
	 * @param dx Increment in X axis.
	 * @param dy Increment in Y axis.
	 */
	StageEditorActionMovePlayerObject(EditorOID oid, double dx, double dy)
		: StageEditorActionMoveObjectBase(oid, dx, dy)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_MOVE_PLAYER_OBJECT;
	}
};

class StageEditorActionMoveObstacleObject : public StageEditorActionMoveObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionMoveObstacleObject object.
	 * 
	 * @param oid The object which was moved.
	 * @param dx Increment in X axis.
	 * @param dy Increment in Y axis.
	 */
	StageEditorActionMoveObstacleObject(EditorOID oid, double dx, double dy)
		: StageEditorActionMoveObjectBase(oid, dx, dy)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_MOVE_OBSTACLE_OBJECT;
	}
};

#endif // STAGEEDITORACTION_HPP
