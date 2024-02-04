/**
 * @file StageEditor.cpp
 * @author Tomáš Ludrovan
 * @brief StageEditor class
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageeditor/StageEditor.hpp"

#include "math/Math.hpp"
#include "stageeditor/Common.hpp"

void StageEditor::getSnappedPoint(const PointF& p, ObjectSnap snapping,
	PointF& pSnap)
{
	switch (snapping) {
		case SNAP_NONE:
			pSnap = p;
			break;
		case SNAP_STEP1:
			pSnap.x = roundToMultiple(p.x, 1.0);
			pSnap.y = roundToMultiple(p.y, 1.0);
			break;
		case SNAP_STEP10:
			pSnap.x = roundToMultiple(p.x, 10.0);
			pSnap.y = roundToMultiple(p.y, 10.0);
			break;
		case SNAP_STEP100:
			pSnap.x = roundToMultiple(p.x, 100.0);
			pSnap.y = roundToMultiple(p.y, 100.0);
			break;
	}
}

std::shared_ptr<StageEditorAction> StageEditor::addPlayerInternal(const PointF& pos)
{
	EditorOID oid = generateEditorOID();

	m_stageState.players.emplace(oid, StageEditorPlayerObject(oid, pos));
	auto res = std::make_shared<StageEditorActionAddPlayer>(pos, oid);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::addObstacleCornerInternal(const PointF& pos)
{
	m_obstacleCorners.push_back(pos);
	auto res = std::make_shared<StageEditorActionPlaceObstacleCorner>(pos);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::completeObstacleInternal()
{
	// Must be a valid Euclidean polygon
	if (m_obstacleCorners.size() < 3) {
		return std::make_shared<StageEditorActionNone>();
	} else {
		EditorOID oid = generateEditorOID();
		PolygonF shape(std::move(m_obstacleCorners));

		m_stageState.obstacles.emplace(oid, StageEditorObstacleObject(oid,
			std::move(shape)));

		m_obstacleCorners = PolygonF::CollectionType();

		auto res = std::make_shared<StageEditorActionCompleteObstacle>(shape, oid);
		return res;
	}
}

std::shared_ptr<StageEditorAction> StageEditor::selectObjectInternal(const PointF& pos)
{
	EditorOID objectId = EDITOR_OID_NULL;

	if ((objectId = getPlayerObjectAt(pos)) != EDITOR_OID_NULL) {
		// Selected player object

		m_selectedPlayers.push_back(objectId);
		auto res = std::make_shared<StageEditorActionSelectPlayerObject>(objectId);
		return res;
	} else if ((objectId = getObstacleObjectAt(pos)) != EDITOR_OID_NULL) {
		// Selected obstacle object

		m_selectedObstacles.push_back(objectId);
		auto res = std::make_shared<StageEditorActionSelectObstacleObject>(objectId);
		return res;
	} else {
		// No object selected

		return std::make_shared<StageEditorActionNone>();
	}
}

std::shared_ptr<StageEditorAction> StageEditor::deselectAllObjectsInternal()
{
	// The actions performed (all the deselections)
	std::vector<std::shared_ptr<StageEditorAction>> actionGroup;

	// Lambda expression, which performs deselection (removes selected object
	// from the selected objects vector) and adds a deselection action to the
	// `actionGroup` list.
	// The parameters:
	//  - `selectedObjsVector`: Vector of selected objects. Should be either
	//                          `m_selectedPlayers` or `m_selectedObstacles`.
	//  - `makeAction`: Function, which creates a deselection action based on
	//                  the provided OID. Should create either
	//                  `StageEditorActionDeselectPlayerObject` or
	//                  `StageEditorActionDeselectObstacleObject` (both should
	//                  be `shared_ptr`).
	auto f = [&actionGroup](std::vector<EditorOID>& selectedObjsVector,
		std::shared_ptr<StageEditorAction> (*makeAction)(EditorOID))
	{
		std::shared_ptr<StageEditorAction> newAction;
		while (!selectedObjsVector.empty()) {
			EditorOID oid = selectedObjsVector.back();
			newAction = makeAction(oid);
			actionGroup.push_back(newAction);
			selectedObjsVector.pop_back();
		}
	};

	// Lambda expression, creates `StageEditorActionDeselectPlayerObject`
	auto makeActionDeselectPlayer = [](EditorOID oid) -> std::shared_ptr<StageEditorAction>
	{
		return std::make_shared<StageEditorActionDeselectPlayerObject>(oid);
	};

	// Lambda expression, creates `StageEditorActionDeselectObstacleObject`
	auto makeActionDeselectObstacle = [](EditorOID oid) -> std::shared_ptr<StageEditorAction>
	{
		return std::make_shared<StageEditorActionDeselectObstacleObject>(oid);
	};
	
	// Fill the `actionGroup` list
	f(m_selectedPlayers, makeActionDeselectPlayer);
	f(m_selectedObstacles, makeActionDeselectObstacle);

	if (actionGroup.empty()) {
		// Didn't deselect anything

		return std::make_shared<StageEditorActionNone>();
	} else {
		// Deselected at least one editor object

		auto res = std::make_shared<StageEditorActionMultiple>(std::move(actionGroup));
		return res;
	}
}

EditorOID StageEditor::getPlayerObjectAt(const PointF& pos)
{
	for (const auto& playerObjPair : m_stageState.players) {
		const EditorOID& oid = playerObjPair.first;
		const StageEditorPlayerObject& player = playerObjPair.second;

		if (player.containsPoint(pos)) {
			return oid;
		}
	}

	return EDITOR_OID_NULL;
}

EditorOID StageEditor::getObstacleObjectAt(const PointF& pos)
{
	for (const auto& obstacleObjPair : m_stageState.obstacles) {
		const EditorOID& oid = obstacleObjPair.first;
		const StageEditorObstacleObject& obstacle = obstacleObjPair.second;

		if (obstacle.containsPoint(pos)) {
			return oid;
		}
	}

	return EDITOR_OID_NULL;
}

StageEditor::StageEditor()
	: m_stageState{
		.width = STAGE_WIDTH_INITIAL,
		.height = STAGE_HEIGHT_INITIAL,
	}
	, m_activeTool{TOOL_SELECT}
	, m_lastAction{std::make_shared<StageEditorActionNone>()}
{}

const StageState& StageEditor::getState() const
{
	return m_stageState;
}

const std::shared_ptr<StageEditorAction> StageEditor::getLastAction() const
{
	return m_lastAction;
}

EditorTool StageEditor::getActiveTool() const
{
	return m_activeTool;
}

const PolygonF::CollectionType& StageEditor::getObstacleCorners() const
{
	return m_obstacleCorners;
}

EditorOID StageEditor::getSelectedPlayerAt(const PointF& pos)
{
	for (auto oid : m_selectedPlayers) {
		const StageEditorPlayerObject& player = m_stageState.players.at(oid);
		if (player.containsPoint(pos)) {
			return oid;
		}
	}

	return EDITOR_OID_NULL;
}

EditorOID StageEditor::getSelectedObstacleAt(const PointF& pos)
{
	for (auto oid : m_selectedObstacles) {
		const StageEditorObstacleObject& obstacle = m_stageState.obstacles.at(oid);
		if (obstacle.containsPoint(pos)) {
			return oid;
		}
	}

	return EDITOR_OID_NULL;
}

bool StageEditor::isSelectedObjectAt(const PointF& pos)
{
return false;
}

void StageEditor::activateTool(EditorTool newTool)
{
	m_lastAction = std::make_shared<StageEditorActionActivateTool>(m_activeTool, newTool);
	m_history.pushAction(m_lastAction);
	m_activeTool = newTool;
}

void StageEditor::undo()
{
	// TODO
}

void StageEditor::redo()
{
	// TODO
}

bool StageEditor::canUndo()
{
	return m_history.canUndo();
}

bool StageEditor::canRedo()
{
	return m_history.canRedo();
}

void StageEditor::mouseLeftBtnDownToolSelect(const PointF& pos,
	ObjectSnap snapping, bool isShiftPressed)
{
	std::vector<std::shared_ptr<StageEditorAction>> actionsGroup;

	if (!isShiftPressed) {
		// Deselect objects, if any are selected
		auto actionDeselect = deselectAllObjectsInternal();
		if (actionDeselect->getType() != StageEditorAction::ACTION_NONE) {
			// Objects were deselected

			actionsGroup.push_back(actionDeselect);
		}
		// Else no objects were deselected
	}
	// Else keep the selected objects

	// Select object
	auto actionSelect = selectObjectInternal(pos);
	if (actionSelect->getType() != StageEditorAction::ACTION_NONE) {
		// Object was selected

		actionsGroup.push_back(actionSelect);
	}
	// Else no object was selected

	if (actionsGroup.size() == 0) {
		// No selections nor deselections

		m_lastAction = std::make_shared<StageEditorActionNone>();
	} else if (actionsGroup.size() == 1) {
		// Either only deselected all objects or only selected one object

		m_lastAction = actionsGroup[0];
		m_history.pushAction(m_lastAction);
	} else {
		// Deselected all objects and then selected one

		m_lastAction = std::make_shared<StageEditorActionMultiple>(
			std::move(actionsGroup));
		m_history.pushAction(m_lastAction);
	}
}

void StageEditor::mouseLeftBtnDownToolPlayers(const PointF& pos,
	ObjectSnap snapping, bool isShiftPressed)
{
	// Snap position to grid
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);

	// Deselect objects, if any are selected
	auto actionDeselect = deselectAllObjectsInternal();

	// Add player
	auto actionAddPlayer = addPlayerInternal(pos);

	if (actionDeselect->getType() == StageEditorAction::ACTION_NONE) {
		// No objects were deselected; just added a player

		m_lastAction = actionAddPlayer;
	} else {
		// Deselected objects and added a player

		m_lastAction = std::make_shared<StageEditorActionMultiple>(
			actionDeselect, actionAddPlayer);
	}

	// Add to actions history
	m_history.pushAction(m_lastAction);
}

void StageEditor::mouseLeftBtnDownToolObstacles(const PointF& pos,
	ObjectSnap snapping, bool isShiftPressed)
{
	// Snap position to grid
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);

	// Deselect objects, if any are selected
	auto actionDeselect = deselectAllObjectsInternal();

	// Add obstacle corner
	auto actionAddCorner = addObstacleCornerInternal(pos);

	if (actionDeselect->getType() == StageEditorAction::ACTION_NONE) {
		// No objects were deselected; just added a player

		m_lastAction = actionAddCorner;
	} else {
		// Deselected objects and added a player

		m_lastAction = std::make_shared<StageEditorActionMultiple>(
			actionDeselect, actionAddCorner);
	}

	// Add to actions history
	m_history.pushAction(m_lastAction);
}

void StageEditor::mouseRightBtnDownToolObstacles()
{
	// Complete obstacle
	m_lastAction = completeObstacleInternal();

	// Add to actions history
	m_history.pushAction(m_lastAction);
}

void StageEditor::mouseLeftBtnDown(const PointF& pos, ObjectSnap snapping,
	bool isShiftPressed)
{
	switch (m_activeTool) {
		case TOOL_SELECT:
			mouseLeftBtnDownToolSelect(pos, snapping, isShiftPressed);
			break;
		case TOOL_PLAYERS:
			mouseLeftBtnDownToolPlayers(pos, snapping, isShiftPressed);
			break;
		case TOOL_OBSTACLES:
			mouseLeftBtnDownToolObstacles(pos, snapping, isShiftPressed);
			break;
	}
}

void StageEditor::mouseRightBtnDown()
{
	if (m_activeTool == TOOL_OBSTACLES) {
		mouseRightBtnDownToolObstacles();
	}
}
