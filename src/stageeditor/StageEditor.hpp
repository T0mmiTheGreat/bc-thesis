/**
 * @file StageEditor.hpp
 * @author Tomáš Ludrovan
 * @brief StageEditor class
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEEDITOR_HPP
#define STAGEEDITOR_HPP

#include <memory>

#include "stageeditor/StageEditorAction.hpp"
#include "stageeditor/StageEditorHistory.hpp"
#include "stageeditor/StageState.hpp"

class StageEditor {
public:
	enum Tool {
		TOOL_SELECT = 0,
		TOOL_PLAYERS,
		TOOL_OBSTACLES,
	};
	enum ObjectSnap {
		SNAP_NONE = 0, // Do not snap
		SNAP_STEP1,    // Snap every step
		SNAP_STEP10,   // Snap every 10 steps
		SNAP_STEP100,  // Snap every 100 steps
	};
private:
	static constexpr int STAGE_WIDTH_INITIAL = 1280;
	static constexpr int STAGE_HEIGHT_INITIAL = 720;

	StageState m_stageState;
	Tool m_activeTool;
	StageEditorHistory m_history;
	std::shared_ptr<StageEditorAction> m_lastAction;

	/**
	 * @brief Snaps the coordinates to the grid based on `snapping`.
	 * 
	 * @param x The X coordinate to snap.
	 * @param y The Y coordinate to snap.
	 * @param snapping The desired snapping.
	 * @param xSnap X coordinate snapped to grid based on `snapping`.
	 * @param ySnap Y coordinate snapped to grid based on `snapping`.
	 */
	void getSnappedXY(double x, double y, ObjectSnap snapping, double& xSnap,
		double& ySnap);

	void toolSelectClick(double x, double y, ObjectSnap snapping);
	void toolPlayersClick(double x, double y, ObjectSnap snapping);
	void toolObstaclesClick(double x, double y, ObjectSnap snapping);
public:
	StageEditor();
	/**
	 * @brief Returns the current state of the stage.
	 */
	const StageState& getState();
	/**
	 * @brief Returns the currently selected editor tool.
	 */
	Tool getActiveTool();
	/**
	 * @brief Selects an editor tool.
	 */
	void setActiveTool(Tool tool);
	/**
	 * @brief Returns the last performed action.
	 */
	const std::shared_ptr<StageEditorAction> getLastAction();
	/**
	 * @brief Left mouse button click in the workspace.
	 * 
	 * @param x 
	 * @param y 
	 * @param snapping How the location should be snapped to grid.
	 */
	void click(double x, double y, ObjectSnap snapping);
	void undo();
	void redo();
	bool canUndo();
	bool canRedo();
};

#endif // STAGEEDITOR_HPP
