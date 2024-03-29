/**
 * @file StageEditorHistory.cpp
 * @author Tomáš Ludrovan
 * @brief StageEditorHistory class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageeditor/StageEditorHistory.hpp"

StageEditorHistory::StageEditorHistory()
{
}

void StageEditorHistory::pushAction(std::shared_ptr<StageEditorAction> action)
{
	m_undoHistory.push_back(action);

	// Can not redo after an action has been successfully performed
	m_redoHistory.clear();
}

std::shared_ptr<StageEditorAction> StageEditorHistory::undoAction()
{
	// Remove from undo history
	auto action = m_undoHistory.back();
	m_undoHistory.pop_back();

	// Push to redo history
	m_redoHistory.push_back(action);

	return action;
}

std::shared_ptr<StageEditorAction> StageEditorHistory::redoAction()
{
	// Remove from redo history
	auto action = m_redoHistory.back();
	m_redoHistory.pop_back();

	// Push back to undo history
	m_undoHistory.push_back(action);

	return action;
}

bool StageEditorHistory::canUndo() const
{
	return (m_undoHistory.size() > 0);
}

bool StageEditorHistory::canRedo() const
{
	return (m_redoHistory.size() > 0);
}
