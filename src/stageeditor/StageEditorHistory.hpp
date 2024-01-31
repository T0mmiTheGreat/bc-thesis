/**
 * @file StageEditorHistory.hpp
 * @author Tomáš Ludrovan
 * @brief StageEditorHistory class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEEDITORHISTORY_HPP
#define STAGEEDITORHISTORY_HPP

#include <memory>
#include <vector>

#include "stageeditor/StageEditorAction.hpp"

/**
 * @brief Keeps history of actions made in the editor for undo/redo.
 */
class StageEditorHistory {
private:
	std::vector<std::shared_ptr<StageEditorAction>> m_undoHistory;
	std::vector<std::shared_ptr<StageEditorAction>> m_redoHistory;
public:
	StageEditorHistory();
	/**
	 * @brief Adds a new action to the history.
	 */
	void pushAction(std::shared_ptr<StageEditorAction> action);
	/**
	 * @brief Undoes previous action and places it to the redo stack.
	 * 
	 * @remark Undefined behavior if the undo stack is empty; use canUndo() to
	 *         check if undo is possible.
	 */
	std::shared_ptr<StageEditorAction> undoAction();
	/**
	 * @brief Redoes an undone action and places it back to the undo stack.
	 * 
	 * @remark Undefined behavior if the redo stack is empty; use canRedo() to
	 *         check if redo is possible.
	 */
	std::shared_ptr<StageEditorAction> redoAction();
	/**
	 * @brief Checks whether undo can be performed.
	 */
	bool canUndo() const;
	/**
	 * @brief Checks whether redo can be performed.
	 */
	bool canRedo() const;
};

#endif // STAGEEDITORHISTORY_HPP
