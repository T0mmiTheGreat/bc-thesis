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
#include <unordered_set>
#include <vector>

#include "types.hpp"
#include "stageeditor/Common.hpp"
#include "stageeditor/StageEditorAction.hpp"
#include "stageeditor/StageEditorHistory.hpp"
#include "stageeditor/StageState.hpp"

class StageEditor {
private:
	static constexpr int STAGE_WIDTH_INITIAL = 1280;
	static constexpr int STAGE_HEIGHT_INITIAL = 720;

	StageState m_stageState;
	EditorTool m_activeTool;
	StageEditorHistory m_history;
	// Corners of the obstacle under construction
	PolygonF::CollectionType m_obstacleCorners;
	std::unordered_set<EditorOID> m_selectedPlayers;
	std::unordered_set<EditorOID> m_selectedObstacles;
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

	std::shared_ptr<StageEditorAction> createActionNone();
	std::shared_ptr<StageEditorAction> createActionAddPlayerObject(const PointF& pos);
	std::shared_ptr<StageEditorAction> createActionPlaceObstacleCorner(
		const PointF& pos);
	std::shared_ptr<StageEditorAction> createActionAddObstacleObject();
	std::shared_ptr<StageEditorAction> createActionAbortObstacle();
	std::shared_ptr<StageEditorAction> createActionActivateTool(
		EditorTool newTool);
	std::shared_ptr<StageEditorAction> createActionSelectObject(
		const PointF& pos);
	std::shared_ptr<StageEditorAction> createActionDeselectPlayerObject(
		EditorOID oid);
	std::shared_ptr<StageEditorAction> createActionDeselectObstacleObject(
		EditorOID oid);
	std::shared_ptr<StageEditorAction> createActionDeselectPlayerIfSelected(
		EditorOID oid);
	std::shared_ptr<StageEditorAction> createActionDeselectObstacleIfSelected(
		EditorOID oid);
	std::shared_ptr<StageEditorAction> createActionDeselectAll();
	std::shared_ptr<StageEditorAction> createActionBeginDragSelected(
		const PointF& where);
	std::shared_ptr<StageEditorAction> createActionMovePlayerObject(
		EditorOID oid, double dx, double dy);
	std::shared_ptr<StageEditorAction> createActionMoveObstacleObject(
		EditorOID oid, double dx, double dy);
	std::shared_ptr<StageEditorAction> createActionMoveSelectedObjects(double dx,
		double dy);
	std::shared_ptr<StageEditorAction> createActionDeletePlayerObject(
		EditorOID oid);
	std::shared_ptr<StageEditorAction> createActionDeleteObstacleObject(
		EditorOID oid);
	std::shared_ptr<StageEditorAction> createActionDeleteSelectedObjects();

	/**
	 * @brief Creates one action from multiple actions.
	 * 
	 * @details The action is created using the following rules:
	 *            - If no action is passed, or all actions are of type
	 *              ACTION_NONE, StageEditorActionNone is returned.
	 *            - If one action is passed, or all actions except one are of
	 *              type ACTION_NONE, the one action is returned.
	 *            - If multiple actions are passed and at least two of them are
	 *              not of type ACTION_NONE, StageEditorActionMultiple is
	 *              returned containing only those actions which are not NONE.
	 * 
	 * @tparam Args StageEditorAction descendant.
	 * @param actions The actions to merge.
	 */
	template <StageEditorActionDerived... Args>
	std::shared_ptr<StageEditorAction> getMergedActions(
		std::shared_ptr<Args>&... actions);
	/**
	 * @brief Creates one action from multiple actions.
	 * 
	 * @copydetail StageEditor::getMergedActions(Args&...)
	 * 
	 * @param actions The actions to merge.
	 */
	std::shared_ptr<StageEditorAction> getMergedActions(
		std::vector<std::shared_ptr<StageEditorAction>> actions);

	void doAction(const std::shared_ptr<StageEditorAction> action);
	void doActionMultiple(const std::shared_ptr<StageEditorAction> action);
	void doActionAddPlayerObject(const std::shared_ptr<StageEditorAction> action);
	void doActionAddObstacleObject(const std::shared_ptr<StageEditorAction> action);
	void doActionPlaceObstacleCorner(const std::shared_ptr<StageEditorAction> action);
	void doActionUnplaceObstacleCorner(const std::shared_ptr<StageEditorAction> action);
	void doActionActivateTool(const std::shared_ptr<StageEditorAction> action);
	void doActionSelectPlayerObject(const std::shared_ptr<StageEditorAction> action);
	void doActionSelectObstacleObject(const std::shared_ptr<StageEditorAction> action);
	void doActionDeselectPlayerObject(const std::shared_ptr<StageEditorAction> action);
	void doActionDeselectObstacleObject(const std::shared_ptr<StageEditorAction> action);
	void doActionBeginDragSelected(const std::shared_ptr<StageEditorAction> action);
	void doActionMovePlayerObject(const std::shared_ptr<StageEditorAction> action);
	void doActionMoveObstacleObject(const std::shared_ptr<StageEditorAction> action);
	void doActionDeletePlayerObject(const std::shared_ptr<StageEditorAction> action);
	void doActionDeleteObstacleObject(const std::shared_ptr<StageEditorAction> action);

	EditorOID getPlayerObjectAt(const PointF& pos);
	EditorOID getObstacleObjectAt(const PointF& pos);

	/**
	 * @brief Creates a new player object.
	 * 
	 * @details Adds a player object to the player list in the stage state,
	 *          and deselects all selected objects, if any.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> addPlayer(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Adds a corner to the currently constructed obstacle.
	 * 
	 * @details Modifies the `m_obstacleCorners` variable by adding a new
	 *          corner, and deselects all selected objects, if any.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> addObstacleCorner(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Finishes construction of an obstacle.
	 * 
	 * @details Creates an obstacle object from the `m_obstacleCorners`
	 *          variable, adds it to the obstacle list in the stage state,
	 *          and deselects all selected objects, if any.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> completeObstacle();
	/**
	 * @brief Activates a tool.
	 * 
	 * @details Activation takes place only if the new tool is different than
	 *          the current active tool. Also, it aborts the construction of
	 *          an obstacle if it's active.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> activateTool(EditorTool tool);
	/**
	 * @brief Selects an object at given coordinates.
	 * 
	 * @details If there's any object at `pos`, adds its OID to the respective
	 *          selected objects list.
	 * 
	 * @param pos 
	 * @param deselectPrevious If false, the currently selected objects will
	 *                         be deselected.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> selectObject(const PointF& pos, bool keepCurrent);
	/**
	 * @brief Begins a dragging operation.
	 * 
	 * @details Records the position in the `m_dragStart` variable and changes
	 *          the `m_isDragging` variable to true.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> beginDragSelected(const PointF& pos);
	/**
	 * @brief Finishes a dragging operation.
	 * 
	 * @details Moves all selected objects and changes the `m_isDragging`
	 *          variable to false.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> endDragSelected(const PointF& pos, ObjectSnap snapping);
	const std::shared_ptr<StageEditorAction> deleteSelected();
	const std::shared_ptr<StageEditorAction> deleteObject(const PointF& pos);

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
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseLeftBtnDownToolSelect(
		const PointF& pos, ObjectSnap snapping, bool isShiftPressed);
	/**
	 * @brief Left mouse button pressed over workspace while the "players tool"
	 *        is active.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseLeftBtnDownToolPlayers(
		const PointF& pos, ObjectSnap snapping, bool isShiftPressed);
	/**
	 * @brief Left mouse button pressed over workspace while the "obstacles
	 *        tool" is active.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseLeftBtnDownToolObstacles(
		const PointF& pos, ObjectSnap snapping, bool isShiftPressed);
	/**
	 * @brief Left mouse button pressed over workspace while the "delete tool"
	 *        is active.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseLeftBtnDownToolDelete(
		const PointF& pos, ObjectSnap snapping, bool isShiftPressed);

	/**
	 * @brief Left mouse button released over workspace while the "select tool"
	 *        is active.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseLeftBtnUpToolSelect(
		const PointF& pos, ObjectSnap snapping);

	/**
	 * @brief Right mouse button pressed over workspace while the "obstacles
	 *        tool" is active.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseRightBtnDownToolObstacles();

	/**
	 * @brief Checks whether the given player object has valid position.
	 */
	bool canPlacePlayer(const StageEditorPlayerObject& player);
	/**
	 * @brief Checks whether the given player object has valid position.
	 * 
	 * @param player 
	 * @param ignoredPlayers Player objects to ignore collisions with.
	 * @param ignoredObstacles Obstacle objects to ignore collisions with.
	 */
	bool canPlacePlayer(const StageEditorPlayerObject& player,
		const std::unordered_set<EditorOID>& ignoredPlayers,
		const std::unordered_set<EditorOID>& ignoredObstacles);
	/**
	 * @brief Checks whether the given obstacle object has valid position.
	 */
	bool canPlaceObstacle(const StageEditorObstacleObject& obstacle);
	/**
	 * @brief Checks whether the given obstacle object has valid position.
	 * 
	 * @param obstacle 
	 * @param ignoredPlayers Player objects to ignore collisions with.
	 */
	bool canPlaceObstacle(const StageEditorObstacleObject& obstacle,
		const std::unordered_set<EditorOID>& ignoredPlayers);
	/**
	 * @brief Checks whether a new player object can be placed at the given
	 *        position.
	 */
	bool canCreatePlayer(const PointF& pos);
	/**
	 * @brief Checks whether an edge can be constructed from the last point of
	 *        `m_obstacleEdges` to `pos`.
	 * 
	 * @param pos 
	 * @param isClosing Whether the new edge is the last one (completes the
	 *                  obstacle). If `true`, the `pos` parameter is ignored
	 *                  and replaced by the first element of `m_obstacleCorners`.
	 */
	bool canConstructEdge(const PointF& pos, bool isClosing);
	/**
	 * @brief Checks whether a new obstacle corner can be placed at the given
	 *        position.
	 */
	bool canPlaceCorner(const PointF& pos);
	/**
	 * @brief Checks whether an obstacle can be created from the current state
	 *        of `m_obstacleCorners`.
	 */
	bool canCompleteObstacle();
	/**
	 * @brief Checks whether a selected player object's position can be changed
	 *        by `(dx, dy)`.
	 */
	bool canMoveSelectedPlayer(EditorOID oid, double dx, double dy);
	/**
	 * @brief Checks whether a selected obstacle object's position can be
	 *        changed by `(dx, dy)`.
	 */
	bool canMoveSelectedObstacle(EditorOID oid, double dx, double dy);
	/**
	 * @brief Checks whether the position of every selected object can be
	 *        changed by `(dx, dy)`.
	 */
	bool canMoveSelectedObjects(double dx, double dy);
public:
	StageEditor();
	/**
	 * @brief Returns the current state of the stage.
	 */
	const StageState& getState() const;
	/**
	 * @brief Return the currently selected tool.
	 */
	EditorTool getActiveTool() const;
	const PolygonF::CollectionType& getObstacleCorners() const;
	const std::unordered_set<EditorOID>& getSelectedPlayers() const;
	const std::unordered_set<EditorOID>& getSelectedObstacles() const;
	
	/**
	 * @brief Left mouse button pressed over a tool button.
	 * 
	 * @param tool The tool ID.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> toolLeftBtnDown(EditorTool tool);

	/**
	 * @brief Left mouse button pressed over workspace.
	 * 
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 * @param isShiftPressed Whether the Shift key is pressed at the same time.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseLeftBtnDown(const PointF& pos, ObjectSnap snapping,
		bool isShiftPressed);
	/**
	 * @brief Left mouse button released over workspace.
	 * 
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseLeftBtnUp(const PointF& pos, ObjectSnap snapping);
	/**
	 * @brief Right mouse button pressed over workspace.
	 * 
	 * @return The action performed.
	 */
	const std::shared_ptr<StageEditorAction> mouseRightBtnDown();

	/**
	 * @brief Checks what would happen if a player was added.
	 * 
	 * @details This method is used for tool brush outline. When a "players
	 *          tool" is active, a circle appears where the player object would
	 *          be placed when the user clicks the mouse.
	 * 
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 * @param isSuccess Whether the action would succeed.
	 * @return The action that would be performed.
	 */
	const std::shared_ptr<StageEditorAction> predictAddPlayer(const PointF& pos,
		ObjectSnap snapping, bool& isSuccess);
	/**
	 * @brief Checks what would happen if an obstacle corner was added.
	 * 
	 * @details This method is used for tool brush outline. When an "obstacles
	 *          tool" is active, a dot appears where the first corner would
	 *          be placed if the user clicks the mouse. If there is at least
	 *          one corner placed already, instead, a line appears between the
	 *          last placed corner and the position where the next corner would
	 *          be placed if the user clicks the mouse.
	 * 
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 * @param isSuccess Whether the action would succeed.
	 * @return The action that would be performed.
	 */
	const std::shared_ptr<StageEditorAction> predictPlaceObstacleCorner(
		const PointF& pos, ObjectSnap snapping, bool& isSuccess);
	/**
	 * @brief Checks what would happen if a player object was dropped
	 *        after dragging.
	 * 
	 * @details This method is used for drag preview. When the dragging is
	 *          active (mouse had been pressed over a selected object and hasn't
	 *          been released yet), the dragged objects move along with the
	 *          moving cursor.
	 * 
	 * @param oid OID of the player object.
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 * @param isSuccess Whether the action would succeed.
	 * @return The action that would be performed.
	 */
	const std::shared_ptr<StageEditorAction> predictEndDragPlayerObject(
		EditorOID oid, const PointF& pos, ObjectSnap snapping, bool& isSuccess);
	/**
	 * @brief Checks what would happen if an obstacle object was dropped
	 *        after dragging.
	 * 
	 * @copydetail StageEditor::predictEndDragPlayerObject()
	 * 
	 * @param oid OID of the player object.
	 * @param pos Mouse position projected to the stage space.
	 * @param snapping How the coordinates should be snapped to grid.
	 * @param isSuccess Whether the action would succeed.
	 * @return The action that would be performed.
	 */
	const std::shared_ptr<StageEditorAction> predictEndDragObstacleObject(
		EditorOID oid, const PointF& pos, ObjectSnap snapping, bool& isSuccess);
	
	const std::shared_ptr<StageEditorAction> undo();
	const std::shared_ptr<StageEditorAction> redo();
	bool canUndo();
	bool canRedo();
};

#endif // STAGEEDITOR_HPP
