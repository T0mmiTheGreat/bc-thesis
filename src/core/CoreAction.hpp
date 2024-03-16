/**
 * @file CoreAction.hpp
 * @author Tomáš Ludrovan
 * @brief CoreAction class
 * @version 0.1
 * @date 2024-03-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef COREACTION_HPP
#define COREACTION_HPP

#include <memory>
#include <type_traits>
#include <vector>

#include "types.hpp"
#include "core/Common.hpp"

/**
 * @brief Base class for core actions.
 * 
 * @details Core actions are created by Core class methods and returned to their
 *          caller to let them know what the method changed.
 */
class CoreAction {
public:
	enum ActionType {
		ACTION_NONE,
		ACTION_MULTIPLE,
		ACTION_ADD_OBSTACLE,
		ACTION_SET_STAGE_SIZE,
		ACTION_ADD_PLAYER,
		ACTION_REMOVE_PLAYER,
		ACTION_SET_PLAYER_POS,
		ACTION_SET_PLAYER_HP,
		ACTION_SET_PLAYER_SIZE,
		ACTION_ADD_BONUS,
		ACTION_REMOVE_BONUS,
	};

	virtual ~CoreAction() {}

	/**
	 * @brief Returns the action type.
	 */
	virtual ActionType getType() const = 0;
};

typedef std::shared_ptr<CoreAction> CoreActionPtr;

/**
 * @brief Class which inherits from CoreAction.
 */
template <class T>
concept CoreActionDerived = std::is_base_of<CoreAction, T>::value;

/**
 * @brief No action was performed.
 */
class CoreActionNone : public CoreAction {
public:
	CoreActionNone() {}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_NONE; }
};

/**
 * @brief Multiple actions performed at once.
 * 
 * @note The order of actions is significant.
 */
class CoreActionMultiple : public CoreAction {
public:
	typedef std::vector<CoreActionPtr> ActionsCollection;
private:
	ActionsCollection m_actions;
public:
	/**
	 * @brief Constructs a new CoreActionMultiple object.
	 * 
	 * @tparam Args Any descendant of CoreAction.
	 * @param actions Any number of actions (their order is significant).
	 */
	template <CoreActionDerived... Args>
	CoreActionMultiple(std::shared_ptr<Args>... actions) {
		m_actions.reserve(sizeof...(actions));
		(m_actions.push_back(actions), ...);
	}

	/**
	 * @brief Constructs a new CoreActionMultiple object.
	 * 
	 * @param actions The actions (their order is significant).
	 */
	CoreActionMultiple(const ActionsCollection& actions)
		: m_actions{actions}
	{}

	/**
	 * @brief Constructs a new CoreActionMultiple object.
	 * 
	 * @param actions The actions (their order is significant).
	 */
	CoreActionMultiple(ActionsCollection&& actions)
		: m_actions{std::move(actions)}
	{}

	/**
	 * @brief Creates one action from multiple actions.
	 * 
	 * @details The action is created using the following rules:
	 *            - If no action is passed, or all actions are of type
	 *              ACTION_NONE, CoreActionNone is returned.
	 *            - If one action is passed, or all actions except one are of
	 *              type ACTION_NONE, the one action is returned.
	 *            - If multiple actions are passed and at least two of them are
	 *              not of type ACTION_NONE, CoreActionMultiple is
	 *              returned containing only those actions which are not NONE.
	 * 
	 * @param actions The actions to merge.
	 */
	static CoreActionPtr getMergedActions(
		const std::vector<CoreActionPtr>& actions)
	{
		std::vector<CoreActionPtr> actionsGroup;

		for (auto& a : actions) {
			if (a->getType() != CoreAction::ACTION_NONE) {
				actionsGroup.push_back(a);
			}
		}

		if (actionsGroup.empty()) {
			return std::make_shared<CoreActionNone>();
		} else if (actionsGroup.size() == 1) {
			return actionsGroup[0];
		} else {
			auto res = std::make_shared<CoreActionMultiple>(
				std::move(actionsGroup));
			return res;
		}
	}

	/**
	 * @brief Creates one action from multiple actions.
	 * 
	 * @copydetail CoreActionMultiple::getMergedActions(std::vector<CoreActionPtr>)
	 * 
	 * @tparam Args CoreAction descendants.
	 * @param actions The actions to merge.
	 */
	template <CoreActionDerived... Args>
	static CoreActionPtr getMergedActions(std::shared_ptr<Args>&... actions) {
		std::vector<CoreActionPtr> v;
		v.reserve(sizeof...(actions));
		(v.push_back(actions), ...);
		return getMergedActions(v);
	}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_MULTIPLE; }

	/**
	 * @brief The actions list (their order is significant).
	 */
	const ActionsCollection& getActions() const { return m_actions; }
};

/**
 * @brief An obstacle was added.
 * 
 * @details Multiple actions of this type are performed when the game starts.
 *          That should be the only time this action happens.
 */
class CoreActionAddObstacle : public CoreAction {
private:
	PolygonF m_shape;
public:
	/**
	 * @brief Constructs a new CoreActionAddObstacle object.
	 * 
	 * @param shape The shape of the obstacle.
	 */
	CoreActionAddObstacle(const PolygonF& shape)
		: m_shape{shape}
	{}

	/**
	 * @brief Constructs a new CoreActionAddObstacle object.
	 * 
	 * @param shape The shape of the obstacle.
	 */
	CoreActionAddObstacle(PolygonF&& shape)
		: m_shape{std::move(shape)}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_ADD_OBSTACLE; }

	/**
	 * @brief The shape of the obstacle.
	 */
	const PolygonF& getShape() const { return m_shape; }
};

/**
 * @brief Size of the stage has changed.
 * 
 * @details This action is performed when the game starts. That should be the
 *          only time this action happens.
 */
class CoreActionSetStageSize : public CoreAction {
private:
	Size2d m_size;
public:
	/**
	 * @brief Constructs a new CoreActionSetStageSize object.
	 * 
	 * @param size New size.
	 */
	CoreActionSetStageSize(const Size2d& size)
		: m_size{size}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_SET_STAGE_SIZE; }

	/**
	 * @brief New size.
	 */
	const Size2d& getSize() const { return m_size; }
};

/**
 * @brief A player joined the game.
 * 
 * @details Multiple actions of this type are performed when the game starts,
 *          but better NOT assume that this is the only time this action
 *          happens.
 */
class CoreActionAddPlayer : public CoreAction {
private:
	PlayerId m_id;
public:
	/**
	 * @brief Constructs a new CoreActionAddPlayer object.
	 * 
	 * @param id Player ID of the new player.
	 */
	CoreActionAddPlayer(PlayerId id)
		: m_id{id}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_ADD_PLAYER; }

	/**
	 * @brief Player ID of the new player.
	 */
	PlayerId getId() const { return m_id; }
};

/**
 * @brief A player has been eliminated.
 */
class CoreActionRemovePlayer : public CoreAction {
private:
	PlayerId m_id;
public:
	/**
	 * @brief Constructs a new CoreActionRemovePlayer object.
	 * 
	 * @param id ID of the removed player.
	 */
	CoreActionRemovePlayer(PlayerId id)
		: m_id{id}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_REMOVE_PLAYER; }

	/**
	 * @brief ID of the removed player.
	 */
	PlayerId getId() const { return m_id; }
};

/**
 * @brief A player has moved.
 */
class CoreActionSetPlayerPos : public CoreAction {
private:
	PlayerId m_id;
	PointF m_pos;
public:
	/**
	 * @brief Constructs a new CoreActionSetPlayerPos object.
	 * 
	 * @param id ID of the player who moved.
	 * @param pos New player position.
	 */
	CoreActionSetPlayerPos(PlayerId id, const PointF& pos)
		: m_id{id}
		, m_pos{pos}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_SET_PLAYER_POS; }

	/**
	 * @brief ID of the player who moved.
	 */
	PlayerId getId() const { return m_id; }

	/**
	 * @brief New player position.
	 */
	const PointF& getPos() const { return m_pos; }
};

/**
 * @brief The number of player health points has changed.
 */
class CoreActionSetPlayerHp : public CoreAction {
private:
	PlayerId m_id;
	double m_hp;
public:
	/**
	 * @brief Constructs a new CoreActionSetPlayerHp object.
	 * 
	 * @param id ID of the player whose HP value was changed.
	 * @param hp New HP value.
	 */
	CoreActionSetPlayerHp(PlayerId id, double hp)
		: m_id{id}
		, m_hp{hp}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_SET_PLAYER_HP; }

	/**
	 * @brief ID of the player whose HP value was changed.
	 */
	PlayerId getId() const { return m_id; }

	/**
	 * @brief New HP value.
	 */
	double getHp() const { return m_hp; }
};

/**
 * @brief Player size has changed.
 */
class CoreActionSetPlayerSize : public CoreAction {
private:
	PlayerId m_id;
	double m_size;
public:
	/**
	 * @brief Constructs a new CoreActionSetPlayerSize object.
	 * 
	 * @param id ID of the player whose size was changed.
	 * @param hp New size.
	 */
	CoreActionSetPlayerSize(PlayerId id, double size)
		: m_id{id}
		, m_size{size}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_SET_PLAYER_SIZE; }

	/**
	 * @brief ID of the player whose size was changed.
	 */
	PlayerId getId() const { return m_id; }

	/**
	 * @brief New size.
	 */
	double getSize() const { return m_size; }
};

/**
 * @brief A bonus has spawned.
 */
class CoreActionAddBonus : public CoreAction {
private:
	BonusId m_id;
	PointF m_pos;
public:
	/**
	 * @brief Constructs a new CoreActionAddBonus object.
	 * 
	 * @param id ID of the new bonus.
	 * @param pos Bonus position.
	 */
	CoreActionAddBonus(BonusId id, const PointF& pos)
		: m_id{id}
		, m_pos{pos}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_ADD_BONUS; }

	/**
	 * @brief ID of the new bonus.
	 */
	BonusId getId() const { return m_id; }

	/**
	 * @brief Bonus position..
	 */
	const PointF& getPos() const { return m_pos; }
};

/**
 * @brief A bonus has been picked up.
 */
class CoreActionRemoveBonus : public CoreAction {
private:
	BonusId m_id;
	double m_hpRecovery;
public:
	/**
	 * @brief Constructs a new CoreActionRemoveBonus object.
	 * 
	 * @param id ID of the removed bonus.
	 * @param hpRecovery How much HP the bonus will recover in total.
	 */
	CoreActionRemoveBonus(BonusId id, double hpRecovery)
		: m_id{id}
		, m_hpRecovery{hpRecovery}
	{}

	/**
	 * @brief Returns the action type.
	 */
	ActionType getType() const override { return ACTION_REMOVE_BONUS; }

	/**
	 * @brief ID of the removed bonus.
	 */
	BonusId getId() const { return m_id; }

	/**
	 * @brief How much HP the bonus will recover in total.
	 */
	double getHpRecovery() const { return m_hpRecovery; }
};

#endif // COREACTION_HPP
