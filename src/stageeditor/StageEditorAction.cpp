/**
 * @file StageEditorAction.cpp
 * @author Tomáš Ludrovan
 * @brief StageEditorAction structure
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageeditor/StageEditorAction.hpp"

std::shared_ptr<StageEditorAction>
StageEditorAction::createActionAddPlayer(double x, double y)
{
	auto res = std::make_shared<StageEditorAction>(StageEditorAction{
		.type = ACTION_ADD_PLAYER,
		.addPlayer = {
			.x = x,
			.y = y
		}
	});
	return res;
}
