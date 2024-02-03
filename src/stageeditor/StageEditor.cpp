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

void StageEditor::addPlayerInternal(const PointF& pos)
{
	EditorOID oid = generateEditorOID();

	m_stageState.players[oid] = pos;
	m_lastAction = std::make_shared<StageEditorActionAddPlayer>(pos, oid);
	m_history.pushAction(m_lastAction);
}

void StageEditor::addObstacleCornerInternal(const PointF& pos)
{
	m_obstacleCorners.push_back(pos);
	m_lastAction = std::make_shared<StageEditorActionPlaceObstacleCorner>(pos);
	m_history.pushAction(m_lastAction);
}

void StageEditor::completeObstacleInternal()
{
	// Must be a valid Euclidean polygon
	if (m_obstacleCorners.size() >= 3) {
		EditorOID oid = generateEditorOID();
		PolygonF shape(std::move(m_obstacleCorners));

		m_stageState.obstacles[oid] = std::move(shape);

		m_obstacleCorners = PolygonF::CollectionType();

		m_lastAction = std::make_shared<StageEditorActionCompleteObstacle>(shape, oid);
		m_history.pushAction(m_lastAction);
	}
}

EditorOID StageEditor::getPlayerObjectAt(const PointF& pos)
{
	for (const auto& playerObjPair : m_stageState.players) {
		const EditorOID& oid = playerObjPair.first;
		const PointF& playerPos = playerObjPair.second;

		if (pos.sqrDistance(playerPos) <= sqr(EDITOR_PLAYER_RADIUS)) {
			return oid;
		}
	}

	return EDITOR_OID_NULL;
}

EditorOID StageEditor::getObstacleObjectAt(const PointF& pos)
{
	for (const auto& obstacleObjPair : m_stageState.obstacles) {
		const EditorOID& oid = obstacleObjPair.first;
		const PolygonF& obstacle = obstacleObjPair.second;

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
	, m_lastAction{std::make_shared<StageEditorActionNone>()}
{}

const StageState& StageEditor::getState()
{
	return m_stageState;
}

const std::shared_ptr<StageEditorAction> StageEditor::getLastAction()
{
	return m_lastAction;
}

void StageEditor::activateTool(EditorTool oldTool, EditorTool newTool)
{
	m_lastAction = std::make_shared<StageEditorActionActivateTool>(oldTool, newTool);
	m_history.pushAction(m_lastAction);
}

void StageEditor::addPlayer(double x, double y, ObjectSnap snapping)
{
	addPlayer(PointF(x, y), snapping);
}

void StageEditor::addPlayer(const PointF& pos, ObjectSnap snapping)
{
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);
	addPlayerInternal(posSnap);
}

void StageEditor::addObstacleCorner(double x, double y, ObjectSnap snapping)
{
	addObstacleCorner(PointF(x, y), snapping);
}

void StageEditor::addObstacleCorner(const PointF& pos, ObjectSnap snapping)
{
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);
	addObstacleCornerInternal(posSnap);
}

void StageEditor::completeObstacle()
{
	completeObstacleInternal();
}

void StageEditor::selectObject(double x, double y)
{
	selectObject(PointF(x, y));
}

void StageEditor::selectObject(const PointF& pos)
{
	EditorOID objectId = EDITOR_OID_NULL;

	if ((objectId = getPlayerObjectAt(pos)) != EDITOR_OID_NULL) {
		// Selected player object

		m_selectedPlayers.push_back(objectId);
		m_lastAction = std::make_shared<StageEditorActionSelectPlayerObject>(objectId);
		m_history.pushAction(m_lastAction);
	} else if ((objectId = getObstacleObjectAt(pos)) != EDITOR_OID_NULL) {
		// Selected obstacle object

		m_selectedObstacles.push_back(objectId);
		m_lastAction = std::make_shared<StageEditorActionSelectObstacleObject>(objectId);
		m_history.pushAction(m_lastAction);
	} else {
		// No object selected

		m_lastAction = std::make_shared<StageEditorActionNone>();
	}
}

void StageEditor::deselectAllObjects()
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

		m_lastAction = std::make_shared<StageEditorActionNone>();
	} else {
		// Deselected at least one editor object

		m_lastAction = std::make_shared<StageEditorActionMultiple>(std::move(actionGroup));
		m_history.pushAction(m_lastAction);
	}
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

const PolygonF::CollectionType& StageEditor::getObstacleCorners() const
{
	return m_obstacleCorners;
}
