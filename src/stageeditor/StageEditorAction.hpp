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
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "types.hpp"
#include "stageeditor/Common.hpp"
#include "stageeditor/StageEditorObjects.hpp"

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
		ACTION_ADD_PLAYER_OBJECT,
		ACTION_ADD_OBSTACLE_OBJECT,
		ACTION_DELETE_PLAYER_OBJECT,
		ACTION_DELETE_OBSTACLE_OBJECT,
		ACTION_PLACE_OBSTACLE_CORNER,
		ACTION_UNPLACE_OBSTACLE_CORNER,
		ACTION_ACTIVATE_TOOL,
		ACTION_SELECT_PLAYER_OBJECT,
		ACTION_SELECT_OBSTACLE_OBJECT,
		ACTION_DESELECT_PLAYER_OBJECT,
		ACTION_DESELECT_OBSTACLE_OBJECT,
		ACTION_BEGIN_DRAG_SELECTED,
		ACTION_MOVE_PLAYER_OBJECT,
		ACTION_MOVE_OBSTACLE_OBJECT,
		ACTION_BEGIN_DRAG_STAGE_CORNER,
		ACTION_RESIZE_STAGE,
		ACTION_SET_STAGE_TITLE,
	};

	virtual ~StageEditorAction() {}

	/**
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	virtual std::shared_ptr<StageEditorAction> createInverse() const = 0;
};

/**
 * @brief Class which inherits from StageEditorAction.
 */
template <class T>
concept StageEditorActionDerived = std::is_base_of<StageEditorAction, T>::value;

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

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
};

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
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief The actions list (their order is significant).
	 */
	const ActionsCollection& getActions() const {
		return m_actions;
	}
};

/**
 * @brief A player object was added.
 */
class StageEditorActionAddPlayerObject : public StageEditorAction {
private:
	StageEditorPlayerObject m_playerObject;
public:
	/**
	 * @brief Constructs a new StageEditorActionAddPlayerObject object.
	 * 
	 * @param playerObject The added player.
	 */
	StageEditorActionAddPlayerObject(
		const StageEditorPlayerObject& playerObject)
		: m_playerObject{playerObject}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_ADD_PLAYER_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief The added player.
	 */
	const StageEditorPlayerObject& getPlayerObject() const {
		return m_playerObject;
	}
};

class StageEditorActionAddObstacleObject : public StageEditorAction {
private:
	StageEditorObstacleObject m_obstacleObject;
public:
	/**
	 * @brief Constructs a new StageEditorActionAddObstacleObject object.
	 * 
	 * @param playerObject The added obstacle.
	 */
	StageEditorActionAddObstacleObject(
		const StageEditorObstacleObject& obstacleObject)
		: m_obstacleObject{obstacleObject}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_ADD_OBSTACLE_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief The added obstacle.
	 */
	const StageEditorObstacleObject& getObstacleObject() const {
		return m_obstacleObject;
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
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief New corner position.
	 */
	const PointF& getPos() const {
		return m_pos;
	}
};

/**
 * @brief Undone place obstacle corner.
 * 
 * @remark The deleted corner is always the last placed.
 */
class StageEditorActionUnplaceObstacleCorner : public StageEditorAction {
private:
	PointF m_pos;
public:
	/**
	 * @brief Constructs a new StageEditorActionUnplaceObstacleCorner object.
	 * 
	 * @param pos Former corner position.
	 */
	StageEditorActionUnplaceObstacleCorner(const PointF& pos)
		: m_pos{pos}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_UNPLACE_OBSTACLE_CORNER;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief Former corner position.
	 */
	const PointF& getPos() const {
		return m_pos;
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
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

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
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	virtual std::shared_ptr<StageEditorAction> createInverse() const override = 0;

	/**
	 * @brief The object which was selected or deselected.
	 */
	EditorOID getOid() const {
		return m_oid;
	}
};

class StageEditorActionSelectObjectBase : public StageEditorActionSelectDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionSelectObjectBase object.
	 * 
	 * @param oid The object which was selected or deselected.
	 */
	StageEditorActionSelectObjectBase(EditorOID oid)
		: StageEditorActionSelectDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	virtual std::shared_ptr<StageEditorAction> createInverse() const override = 0;
};

class StageEditorActionSelectPlayerObject : public StageEditorActionSelectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionSelectPlayerObject object.
	 * 
	 * @param oid The object which was selected.
	 */
	StageEditorActionSelectPlayerObject(EditorOID oid)
		: StageEditorActionSelectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_SELECT_PLAYER_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
};

class StageEditorActionSelectObstacleObject : public StageEditorActionSelectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionSelectObstacleObject object.
	 * 
	 * @param oid The object which was selected.
	 */
	StageEditorActionSelectObstacleObject(EditorOID oid)
		: StageEditorActionSelectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_SELECT_OBSTACLE_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
};

class StageEditorActionDeselectObjectBase : public StageEditorActionSelectDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionSelectObjectBase object.
	 * 
	 * @param oid The object which was selected or deselected.
	 */
	StageEditorActionDeselectObjectBase(EditorOID oid)
		: StageEditorActionSelectDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	virtual std::shared_ptr<StageEditorAction> createInverse() const override = 0;
};

class StageEditorActionDeselectPlayerObject : public StageEditorActionDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionDeselectPlayerObject object.
	 * 
	 * @param oid The object which was deselected.
	 */
	StageEditorActionDeselectPlayerObject(EditorOID oid)
		: StageEditorActionDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_DESELECT_PLAYER_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
};

class StageEditorActionDeselectObstacleObject : public StageEditorActionDeselectObjectBase {
public:
	/**
	 * @brief Constructs a new StageEditorActionDeselectObstacleObject object.
	 * 
	 * @param oid The object which was deselected.
	 */
	StageEditorActionDeselectObstacleObject(EditorOID oid)
		: StageEditorActionDeselectObjectBase(oid)
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_DESELECT_OBSTACLE_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
};

class StageEditorActionBeginDragSelected : public StageEditorAction {
private:
	PointF m_where;
	std::unordered_set<EditorOID> m_playerOids;
	std::unordered_set<EditorOID> m_obstacleOids;
public:
	/**
	 * @brief Constructs a new StageEditorActionBeginDragSelected object.
	 * 
	 * @param where Position where the drag started.
	 * @param playerOids OIDs of the selected players.
	 * @param obstacleOids OIDs of the selected obstacles.
	 */
	StageEditorActionBeginDragSelected(
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
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

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
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	virtual std::shared_ptr<StageEditorAction> createInverse() const override = 0;

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

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
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

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
};

class StageEditorActionDeleteObjectBase : public StageEditorAction {
public:
	/**
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	virtual std::shared_ptr<StageEditorAction> createInverse() const override = 0;

	/**
	 * @brief The deleted object.
	 */
	virtual const StageEditorObjectBase& getObject() const = 0;
};

class StageEditorActionDeletePlayerObject : public StageEditorActionDeleteObjectBase {
private:
	StageEditorPlayerObject m_playerObject;
public:
	/**
	 * @brief Constructs a new StageEditorActionDeletePlayerObject object.
	 * 
	 * @param playerObject The deleted player object.
	 */
	StageEditorActionDeletePlayerObject(const StageEditorPlayerObject& playerObject)
		: m_playerObject{playerObject}
	{}
	
	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_DELETE_PLAYER_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief The deleted object.
	 */
	const StageEditorObjectBase& getObject() const override {
		return getPlayerObject();
	}

	/**
	 * @brief The deleted player object.
	 */
	const StageEditorPlayerObject& getPlayerObject() const {
		return m_playerObject;
	}
};

class StageEditorActionDeleteObstacleObject : public StageEditorActionDeleteObjectBase {
private:
	StageEditorObstacleObject m_obstacleObject;
public:
	/**
	 * @brief Constructs a new StageEditorActionDeleteObstacleObject object.
	 * 
	 * @param obstacleObject The deleted obstacle object.
	 */
	StageEditorActionDeleteObstacleObject(const StageEditorObstacleObject& obstacleObject)
		: m_obstacleObject{obstacleObject}
	{}
	
	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_DELETE_OBSTACLE_OBJECT;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief The deleted object.
	 */
	const StageEditorObjectBase& getObject() const override {
		return getObstacleObject();
	}

	/**
	 * @brief The deleted obstacle object.
	 */
	const StageEditorObstacleObject& getObstacleObject() const {
		return m_obstacleObject;
	}
};

class StageEditorActionBeginDragStageCorner : public StageEditorAction {
public:
	/**
	 * @brief Constructs a new StageEditorActionBeginDragStageCorner object.
	 */
	StageEditorActionBeginDragStageCorner()
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_BEGIN_DRAG_STAGE_CORNER;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;
};

class StageEditorActionResizeStage : public StageEditorAction {
private:
	int m_resizeX;
	int m_resizeY;
public:
	/**
	 * @brief Constructs a new StageEditorActionResizeStage object.
	 * 
	 * @param resizeX Resize amount in X dimension.
	 * @param resizeY Resize amount in Y dimension.
	 */
	StageEditorActionResizeStage(int resizeX, int resizeY)
		: m_resizeX{resizeX}
		, m_resizeY{resizeY}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_RESIZE_STAGE;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	/**
	 * @brief Resize amount in X dimension.
	 */
	int getResizeX() const {
		return m_resizeX;
	}

	/**
	 * @brief Resize amount in Y dimension.
	 */
	int getResizeY() const {
		return m_resizeY;
	}
};

class StageEditorActionSetStageTitle : public StageEditorAction {
private:
	std::string m_oldName;
	std::string m_newName;
public:
	StageEditorActionSetStageTitle(const std::string& oldName,
		const std::string& newName)
		: m_oldName{oldName}
		, m_newName{newName}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override {
		return ACTION_RESIZE_STAGE;
	}

	/**
	 * @brief Creates action which does the opposite of this action.
	 * 
	 * @details For undo.
	 */
	std::shared_ptr<StageEditorAction> createInverse() const override;

	const std::string& getOldName() const {
		return m_oldName;
	}

	const std::string& getNewName() const {
		return m_newName;
	}
};

#endif // STAGEEDITORACTION_HPP
