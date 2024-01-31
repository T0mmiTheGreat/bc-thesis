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

	void addPlayerInternal(double x, double y);
public:
	StageEditor();
	/**
	 * @brief Returns the current state of the stage.
	 */
	const StageState& getState();
	/**
	 * @brief Returns the last performed action.
	 */
	const std::shared_ptr<StageEditorAction> getLastAction();
	/**
	 * @brief Adds a player object to the stage.
	 * 
	 * @param x Approximate X coordiante of the player object.
	 * @param y Approximate Y coordiante of the player object.
	 * @param snapping How the coordinates should be snapped to grid.
	 */
	void addPlayer(double x, double y, ObjectSnap snapping);
	/**
	 * @brief Adds a player object to the stage.
	 * 
	 * @param pos Approximate position of the player object.
	 * @param snapping How the position should be snapped to grid.
	 */
	void addPlayer(const PointF& pos, ObjectSnap snapping);
	void undo();
	void redo();
	bool canUndo();
	bool canRedo();
};

#endif // STAGEEDITOR_HPP
