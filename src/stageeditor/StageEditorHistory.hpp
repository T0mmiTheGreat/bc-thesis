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

class StageEditorHistory {
private:
	std::vector<std::shared_ptr<StageEditorAction>> m_undoHistory;
	std::vector<std::shared_ptr<StageEditorAction>> m_redoHistory;
public:
	StageEditorHistory();
	void pushAction(std::shared_ptr<StageEditorAction> action);
	std::shared_ptr<StageEditorAction> undoAction();
	std::shared_ptr<StageEditorAction> redoAction();
	bool canUndo() const;
	bool canRedo() const;
};

#endif // STAGEEDITORHISTORY_HPP
