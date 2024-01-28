/**
 * @file StageEditorAction.hpp
 * @author Tomáš Ludrovan
 * @brief StageEditorAction structure
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEEDITORACTION_HPP
#define STAGEEDITORACTION_HPP

#include <memory>

struct StageEditorAction {
	enum ActionType {
		ACTION_ADD_PLAYER,
	};

	ActionType type;
	union {
		struct {
			double x;
			double y;
		} addPlayer;
	};

	static std::shared_ptr<StageEditorAction>
	createActionAddPlayer(double x, double y);
};

#endif // STAGEEDITORACTION_HPP
