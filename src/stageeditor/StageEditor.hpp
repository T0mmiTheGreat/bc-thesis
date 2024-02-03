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
#include <vector>

#include "types.hpp"
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
	PolygonF::CollectionType m_obstacleCorners;
	std::vector<EditorOID> m_selectedPlayers;
	std::vector<EditorOID> m_selectedObstacles;

	/**
	 * @brief Snaps the coordinates to the grid based on `snapping`.
	 * 
	 * @param p The point to snap
	 * @param snapping The desired snapping.
	 * @param pSnap `p` snapped to grid based on `snapping`.
	 */
	void getSnappedPoint(const PointF& p, ObjectSnap snapping, PointF& pSnap);

	void addPlayerInternal(const PointF& pos);
	void addObstacleCornerInternal(const PointF& pos);
	void completeObstacleInternal();
	EditorOID getPlayerObjectAt(const PointF& pos);
	EditorOID getObstacleObjectAt(const PointF& pos);
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
	 * @brief Sets active tool.
	 * 
	 * @note Updates last action.
	 * 
	 * @param oldTool The tool which was replaced.
	 * @param newTool The activated tool.
	 * 
	 * @remark Activation of a tool is an action and must be recorded in the
	 *         action history so it can be undone.
	 */
	void activateTool(EditorTool oldTool, EditorTool newTool);
	/**
	 * @brief Adds a player object to the stage.
	 * 
	 * @note Updates last action.
	 * 
	 * @param x Approximate X coordiante of the player object.
	 * @param y Approximate Y coordiante of the player object.
	 * @param snapping How the coordinates should be snapped to grid.
	 */
	void addPlayer(double x, double y, ObjectSnap snapping);
	/**
	 * @brief Adds a player object to the stage.
	 * 
	 * @note Updates last action.
	 * 
	 * @param pos Approximate position of the player object.
	 * @param snapping How the position should be snapped to grid.
	 */
	void addPlayer(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Adds an obstacle corner to the stage.
	 * 
	 * @note Updates last action.
	 * 
	 * @param x Approximate X coordiante of the corner.
	 * @param y Approximate Y coordiante of the corner.
	 * @param snapping How the coordinates should be snapped to grid.
	 */
	void addObstacleCorner(double x, double y, ObjectSnap snapping);
	/**
	 * @brief Adds an obstacle corner to the stage.
	 * 
	 * @note Updates last action.
	 * 
	 * @param pos Approximate position of the corner.
	 * @param snapping How the position should be snapped to grid.
	 */
	void addObstacleCorner(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Closes an open obstacle and adds it to the stage.
	 * 
	 * @note Updates last action.
	 * 
	 * @remark If the obstacle cannot be closed, doesn't do anything.
	 */
	void completeObstacle();
	/**
	 * @brief Selects an object at given position, if possible.
	 * 
	 * @note Updates last action.
	 * 
	 * @param x Exact X coordinate where the check should be performed.
	 * @param y Exact Y coordinate where the check should be performed.
	 */
	void selectObject(double x, double y);
	/**
	 * @brief Selects an object at given position, if possible.
	 * 
	 * @note Updates last action.
	 * 
	 * @param pos Exact position where the check should be performed.
	 */
	void selectObject(const PointF& pos);
	/**
	 * @brief Deselects all selected objects
	 * 
	 * @note Updates last action.
	 */
	void deselectAllObjects();
	void undo();
	void redo();
	bool canUndo();
	bool canRedo();

	const PolygonF::CollectionType& getObstacleCorners() const;
};

#endif // STAGEEDITOR_HPP
