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
#include "stageeditor/Common.hpp"
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
	EditorTool m_activeTool;
	StageEditorHistory m_history;
	std::shared_ptr<StageEditorAction> m_lastAction;
	// Corners of the obstacle under construction
	PolygonF::CollectionType m_obstacleCorners;
	std::vector<EditorOID> m_selectedPlayers;
	std::vector<EditorOID> m_selectedObstacles;
	// The position of the mouse in stage space where the dragging began
	PointF m_dragStart;
	bool m_isDragging;

	/**
	 * @brief Snaps the coordinates to the grid based on `snapping`.
	 * 
	 * @param p The point to snap
	 * @param snapping The desired snapping.
	 * @param pSnap `p` snapped to grid based on `snapping`.
	 */
	void getSnappedPoint(const PointF& p, ObjectSnap snapping, PointF& pSnap);

	std::shared_ptr<StageEditorAction> addPlayerAction(const PointF& pos);
	std::shared_ptr<StageEditorAction> addObstacleCornerAction(const PointF& pos);
	std::shared_ptr<StageEditorAction> completeObstacleAction();
	std::shared_ptr<StageEditorAction> selectObjectAction(const PointF& pos);
	std::shared_ptr<StageEditorAction> deselectAllObjectsAction();
	std::shared_ptr<StageEditorAction> movePlayerObjectAction(EditorOID oid,
		double dx, double dy);
	std::shared_ptr<StageEditorAction> moveObstacleObjectAction(EditorOID oid,
		double dx, double dy);
	std::shared_ptr<StageEditorAction> moveSelectedObjectsAction(double dx,
		double dy);
	EditorOID getPlayerObjectAt(const PointF& pos);
	EditorOID getObstacleObjectAt(const PointF& pos);

	/**
	 * @brief Creates a new player object.
	 * 
	 * @note Updates last action.
	 * 
	 * @details Adds a player object to the player list in the stage state,
	 *          and deselects all selected objects, if any.
	 */
	void addPlayer(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Adds a corner to the currently constructed obstacle.
	 * 
	 * @note Updates last action.
	 * 
	 * @details Modifies the `m_obstacleCorners` variable by adding a new
	 *          corner, and deselects all selected objects, if any.
	 */
	void addObstacleCorner(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Finishes construction of an obstacle.
	 * 
	 * @note Updates last action.
	 * 
	 * @details Creates an obstacle object from the `m_obstacleCorners`
	 *          variable, adds it to the obstacle list in the stage state,
	 *          and deselects all selected objects, if any.
	 */
	void completeObstacle();
	/**
	 * @brief Selects an object at given coordinates.
	 * 
	 * @note Updates last action.
	 * 
	 * @details If there's any object at `pos`, adds its OID to the respective
	 *          selected objects list.
	 * 
	 * @param pos 
	 * @param deselectPrevious If false, the currently selected objects will
	 *                         be deselected.
	 */
	void selectObject(const PointF& pos, bool keepCurrent);
	/**
	 * @brief Begins a dragging operation.
	 * 
	 * @note Updates last action.
	 * 
	 * @details Records the position in the `m_dragStart` variable and changes
	 *          the `m_isDragging` variable to true.
	 */
	void beginDragSelected(const PointF& pos);
	void endDragSelected(const PointF& pos, ObjectSnap snapping);

	/**
	 * @brief Returns the OID of the player object at the given position,
	 *        if it's selected.
	 * 
	 * @param pos Exact position where the check should be performed.
	 * @return The player object at the given position, or EDITOR_OID_NULL
	 *         if there's none.
	 */
	EditorOID getSelectedPlayerAt(const PointF& pos);
	/**
	 * @brief Returns the OID of the obstacle object at the given position,
	 *        if it's selected.
	 * 
	 * @param pos Exact position where the check should be performed.
	 * @return The obstacle object at the given position, or EDITOR_OID_NULL
	 *         if there's none.
	 */
	EditorOID getSelectedObstacleAt(const PointF& pos);
	/**
	 * @brief Returns true if there is any selected object at the given
	 *        coordinates.
	 * 
	 * @param pos Exact position where the check should be performed.
	 */
	bool isSelectedObjectAt(const PointF& pos);

	/**
	 * @brief Left mouse button pressed over workspace while the "select tool"
	 *        is active.
	 */
	void mouseLeftBtnDownToolSelect(const PointF& pos, ObjectSnap snapping,
		bool isShiftPressed);
	/**
	 * @brief Left mouse button pressed over workspace while the "players tool"
	 *        is active.
	 */
	void mouseLeftBtnDownToolPlayers(const PointF& pos, ObjectSnap snapping,
		bool isShiftPressed);
	/**
	 * @brief Left mouse button pressed over workspace while the "obstacles
	 *        tool" is active.
	 */
	void mouseLeftBtnDownToolObstacles(const PointF& pos, ObjectSnap snapping,
		bool isShiftPressed);
	/**
	 * @brief Left mouse button pressed over workspace while the "delete tool"
	 *        is active.
	 */
	void mouseLeftBtnDownToolDelete(const PointF& pos, ObjectSnap snapping,
		bool isShiftPressed);

	/**
	 * @brief Left mouse button released over workspace while the "select tool"
	 *        is active.
	 */
	void mouseLeftBtnUpToolSelect(const PointF& pos, ObjectSnap snapping);

	/**
	 * @brief Right mouse button pressed over workspace while the "obstacles
	 *        tool" is active.
	 */
	void mouseRightBtnDownToolObstacles();
public:
	StageEditor();
	/**
	 * @brief Returns the current state of the stage.
	 */
	const StageState& getState() const;
	/**
	 * @brief Returns the last performed action.
	 */
	const std::shared_ptr<StageEditorAction> getLastAction() const;
	/**
	 * @brief Return the currently selected tool.
	 */
	EditorTool getActiveTool() const;
	const PolygonF::CollectionType& getObstacleCorners() const;
	
	/**
	 * @brief Sets active tool.
	 * 
	 * @note Updates last action.
	 * 
	 * @param newTool The activated tool.
	 */
	void activateTool(EditorTool newTool);

	/**
	 * @brief Left mouse button pressed over workspace.
	 * 
	 * @note Updates last action.
	 * 
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 * @param isShiftPressed Whether the Shift key is pressed at the same time.
	 */
	void mouseLeftBtnDown(const PointF& pos, ObjectSnap snapping,
		bool isShiftPressed);
	/**
	 * @brief Left mouse button released over workspace.
	 * 
	 * @note Updates last action.
	 * 
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 */
	void mouseLeftBtnUp(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Right mouse button pressed over workspace.
	 * 
	 * @note Updates last action.
	 */
	void mouseRightBtnDown();
	
	void undo();
	void redo();
	bool canUndo();
	bool canRedo();
};

#endif // STAGEEDITOR_HPP
