/**
 * @file StageEditorAction.cpp
 * @author Tomáš Ludrovan
 * @brief StageEditorAction class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageeditor/StageEditorAction.hpp"

#include <vector>

std::shared_ptr<StageEditorAction>
	StageEditorActionMultiple::createInverse() const
{
	// Invert each action and create ActionMultiple with reverse order.

	std::vector<std::shared_ptr<StageEditorAction>> actionsGroup;
	actionsGroup.reserve(m_actions.size());
	std::shared_ptr<StageEditorAction> revAction;

	for (auto i = m_actions.rbegin(); i != m_actions.rend(); ++i) {
		revAction = (*i)->createInverse();
		actionsGroup.push_back(revAction);
	}

	auto res = std::make_shared<StageEditorActionMultiple>(actionsGroup);
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionNone::createInverse() const
{
	return std::make_shared<StageEditorActionNone>();
}

std::shared_ptr<StageEditorAction>
	StageEditorActionAddPlayerObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionDeletePlayerObject>(
		m_playerObject);
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionAddObstacleObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionDeleteObstacleObject>(
		m_obstacleObject);
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionPlaceObstacleCorner::createInverse() const
{
	auto res = std::make_shared<StageEditorActionUnplaceObstacleCorner>(
		m_pos);
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionUnplaceObstacleCorner::createInverse() const
{
	auto res = std::make_shared<StageEditorActionPlaceObstacleCorner>(
		m_pos);
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionActivateTool::createInverse() const
{
	auto res = std::make_shared<StageEditorActionActivateTool>(m_newTool, m_oldTool);
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionSelectPlayerObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionDeselectPlayerObject>(getOid());
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionSelectObstacleObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionDeselectObstacleObject>(getOid());
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionDeselectPlayerObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionSelectPlayerObject>(getOid());
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionDeselectObstacleObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionSelectObstacleObject>(getOid());
	return res;
}

std::shared_ptr<StageEditorAction> StageEditorActionBeginDragSelected::createInverse() const
{
	// This action should not appear in the action history; it is more of
	// a helper action for backend and controller, so it there is no reason
	// to invert it

	return std::make_shared<StageEditorActionNone>();
}

std::shared_ptr<StageEditorAction>
	StageEditorActionMovePlayerObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionMovePlayerObject>(getOid(),
		-getDx(), -getDy());
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionMoveObstacleObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionMoveObstacleObject>(getOid(),
		-getDx(), -getDy());
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionDeletePlayerObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionAddPlayerObject>(
		m_playerObject);
	return res;
}

std::shared_ptr<StageEditorAction>
	StageEditorActionDeleteObstacleObject::createInverse() const
{
	auto res = std::make_shared<StageEditorActionAddObstacleObject>(
		m_obstacleObject);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditorActionBeginDragStageCorner::createInverse() const
{
	// This action should not appear in the action history; it is more of
	// a helper action for backend and controller, so it there is no reason
	// to invert it

	return std::make_shared<StageEditorActionNone>();
}

std::shared_ptr<StageEditorAction> StageEditorActionResizeStage::createInverse() const
{
	auto res = std::make_shared<StageEditorActionResizeStage>(-m_resizeX,
		-m_resizeY);
	return res;
}
