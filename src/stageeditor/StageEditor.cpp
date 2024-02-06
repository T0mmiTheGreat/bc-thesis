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

#include <vector>

#include "math/Math.hpp"
#include "stageeditor/Common.hpp"

void StageEditor::getSnappedPoint(const PointF& p, ObjectSnap snapping,
	PointF& pSnap)
{
	pSnap.x = roundToMultiple(p.x, static_cast<PointF::ValueType>(snapping));
	pSnap.y = roundToMultiple(p.y, static_cast<PointF::ValueType>(snapping));
}

std::shared_ptr<StageEditorAction> StageEditor::createActionNone()
{
	return std::make_shared<StageEditorActionNone>();
}

std::shared_ptr<StageEditorAction> StageEditor::createActionAddPlayer(
	const PointF& pos)
{
	EditorOID oid = generateEditorOID();
	auto res = std::make_shared<StageEditorActionAddPlayer>(pos, oid);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionPlaceObstacleCorner(
	const PointF& pos)
{
	auto res = std::make_shared<StageEditorActionPlaceObstacleCorner>(pos);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionCompleteObstacle()
{
	// Must be a valid Euclidean polygon
	if (m_obstacleCorners.size() < 3) {
		return std::make_shared<StageEditorActionNone>();
	} else {
		EditorOID oid = generateEditorOID();
		PolygonF shape = m_obstacleCorners;

		auto res = std::make_shared<StageEditorActionCompleteObstacle>(shape, oid);
		return res;
	}
}

std::shared_ptr<StageEditorAction> StageEditor::createActionSelectObject(
	const PointF& pos)
{
	EditorOID objectId = EDITOR_OID_NULL;

	if ((objectId = getPlayerObjectAt(pos)) != EDITOR_OID_NULL) {
		// Selected player object

		auto res = std::make_shared<StageEditorActionSelectPlayerObject>(objectId);
		return res;
	} else if ((objectId = getObstacleObjectAt(pos)) != EDITOR_OID_NULL) {
		// Selected obstacle object

		auto res = std::make_shared<StageEditorActionSelectObstacleObject>(objectId);
		return res;
	} else {
		// No object selected

		return createActionNone();
	}
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeselectAll()
{
	// The actions performed (all the deselections)
	std::vector<std::shared_ptr<StageEditorAction>> actionGroup;
	std::shared_ptr<StageEditorAction> newAction;

	// Deselection of players
	for (auto oid : m_selectedPlayers) {
		newAction = std::make_shared<StageEditorActionDeselectPlayerObject>(
			oid);
		actionGroup.push_back(newAction);
	}

	// Deselection of obstacles
	for (auto oid : m_selectedObstacles) {
		newAction = std::make_shared<StageEditorActionDeselectObstacleObject>(
			oid);
		actionGroup.push_back(newAction);
	}

	// Merge
	auto res = getMergedActions(actionGroup);

	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionMoveSelectedObjects(
	double dx, double dy)
{
	if (dx == 0 && dy == 0) {
		return createActionNone();
	} else {
		// The actions performed (all the moves)
		std::vector<std::shared_ptr<StageEditorAction>> actionGroup;
		std::shared_ptr<StageEditorAction> newAction;

		// Move players
		for (EditorOID oid : m_selectedPlayers) {
			newAction = std::make_shared<StageEditorActionMovePlayerObject>(
				oid, dx, dy);
			actionGroup.push_back(newAction);
		}

		// Move obstacles
		for (EditorOID oid : m_selectedObstacles) {
			newAction = std::make_shared<StageEditorActionMoveObstacleObject>(
				oid, dx, dy);
			actionGroup.push_back(newAction);
		}

		// Merge
		auto res = getMergedActions(actionGroup);

		return res;
	}
}

template <StageEditorActionDerived... Args>
std::shared_ptr<StageEditorAction> StageEditor::getMergedActions(
	std::shared_ptr<Args>&... actions)
{
	std::vector<std::shared_ptr<StageEditorAction>> v;
	v.reserve(sizeof...(actions));
	(v.push_back(actions), ...);
	return getMergedActions(v);
}

std::shared_ptr<StageEditorAction> StageEditor::getMergedActions(
	std::vector<std::shared_ptr<StageEditorAction>> actions)
{
	std::vector<std::shared_ptr<StageEditorAction>> actionsGroup;

	for (auto& a : actions) {
		if (a->getType() != StageEditorAction::ACTION_NONE) {
			actionsGroup.push_back(a);
		}
	}

	if (actionsGroup.empty()) {
		return createActionNone();
	} else if (actionsGroup.size() == 1) {
		return actionsGroup[0];
	} else {
		auto res = std::make_shared<StageEditorActionMultiple>(
			std::move(actionsGroup));
		return res;
	}
}

void StageEditor::doAction(const std::shared_ptr<StageEditorAction> action)
{
	switch (action->getType()) {
		case StageEditorAction::ACTION_NONE:
			break;
		case StageEditorAction::ACTION_MULTIPLE:
			doActionMultiple(action);
			break;
		case StageEditorAction::ACTION_ADD_PLAYER:
			doActionAddPlayer(action);
			break;
		case StageEditorAction::ACTION_PLACE_OBSTACLE_CORNER:
			doActionPlaceObstacleCorner(action);
			break;
		case StageEditorAction::ACTION_COMPLETE_OBSTACLE:
			doActionCompleteObstacle(action);
			break;
		case StageEditorAction::ACTION_ACTIVATE_TOOL:
			doActionActivateTool(action);
			break;
		case StageEditorAction::ACTION_SELECT_PLAYER_OBJECT:
			doActionSelectPlayerObject(action);
			break;
		case StageEditorAction::ACTION_SELECT_OBSTACLE_OBJECT:
			doActionSelectObstacleObject(action);
			break;
		case StageEditorAction::ACTION_DESELECT_PLAYER_OBJECT:
			doActionDeselectPlayerObject(action);
			break;
		case StageEditorAction::ACTION_DESELECT_OBSTACLE_OBJECT:
			doActionDeselectObstacleObject(action);
			break;
		case StageEditorAction::ACTION_MOVE_PLAYER_OBJECT:
			doActionMovePlayerObject(action);
			break;
		case StageEditorAction::ACTION_MOVE_OBSTACLE_OBJECT:
			doActionMoveObstacleObject(action);
			break;
	}
}

void StageEditor::doActionMultiple(const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionMultiple>(action);

	for (const auto& a : actionCast->getActions()) {
		doAction(a);
	}
}

void StageEditor::doActionAddPlayer(const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionAddPlayer>(action);
	
	EditorOID oid = actionCast->getOid();
	const PointF& pos = actionCast->getPos();

	m_stageState.players.emplace(oid, StageEditorPlayerObject(oid, pos));
}

void StageEditor::doActionPlaceObstacleCorner(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionPlaceObstacleCorner>(action);
	
	const PointF& pos = actionCast->getPos();

	m_obstacleCorners.push_back(pos);
}

void StageEditor::doActionCompleteObstacle(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionCompleteObstacle>(action);
	
	EditorOID oid = actionCast->getOid();
	const PolygonF& shape = actionCast->getShape();

	m_stageState.obstacles.emplace(oid, StageEditorObstacleObject(oid, shape));

	m_obstacleCorners.clear();
}

void StageEditor::doActionActivateTool(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionActivateTool>(action);

	EditorTool newTool = actionCast->getNewTool();

	m_activeTool = newTool;
}

void StageEditor::doActionSelectPlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionSelectPlayerObject>(action);
	
	EditorOID oid = actionCast->getOid();

	m_selectedPlayers.insert(oid);
}

void StageEditor::doActionSelectObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionSelectObstacleObject>(action);
	
	EditorOID oid = actionCast->getOid();

	m_selectedObstacles.insert(oid);
}

void StageEditor::doActionDeselectPlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeselectPlayerObject>(action);
	
	EditorOID oid = actionCast->getOid();

	m_selectedPlayers.erase(oid);
}

void StageEditor::doActionDeselectObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeselectObstacleObject>(action);
	
	EditorOID oid = actionCast->getOid();

	m_selectedObstacles.erase(oid);
}

void StageEditor::doActionMovePlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionMovePlayerObject>(action);
	
	EditorOID oid = actionCast->getOid();
	double dx = actionCast->getDx();
	double dy = actionCast->getDy();

	auto& player = m_stageState.players.at(oid);

	player.pos.x += dx;
	player.pos.y += dy;
}

void StageEditor::doActionMoveObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionMoveObstacleObject>(action);
	
	EditorOID oid = actionCast->getOid();
	double dx = actionCast->getDx();
	double dy = actionCast->getDy();

	auto& obstacle = m_stageState.obstacles.at(oid);

	for (auto& corner : obstacle.shape.corners) {
		corner.x += dx;
		corner.y += dy;
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

const std::shared_ptr<StageEditorAction> StageEditor::addPlayer(
	const PointF& pos, ObjectSnap snapping)
{
	// Snap position to grid
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);

	// Deselect objects, if any are selected
	auto actionDeselect = createActionDeselectAll();

	// Add player
	auto actionAddPlayer = createActionAddPlayer(pos);

	// Merge
	auto res = getMergedActions(actionDeselect, actionAddPlayer);

	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::addObstacleCorner(
	const PointF& pos, ObjectSnap snapping)
{
	// Snap position to grid
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);

	// Deselect objects, if any are selected
	auto actionDeselect = createActionDeselectAll();

	// Place obstacle corner
	auto actionPlaceCorner = createActionPlaceObstacleCorner(pos);

	// Merge
	auto res = getMergedActions(actionDeselect, actionPlaceCorner);

	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::completeObstacle()
{
	// Complete obstacle
	auto res = createActionCompleteObstacle();
	
	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::selectObject(
	const PointF& pos, bool keepCurrent)
{
	std::shared_ptr<StageEditorAction> actionDeselect;

	if (!keepCurrent) {
		// Deselect objects
		
		actionDeselect = createActionDeselectAll();
	} else {
		// Else keep the selected objects (do nothing)

		actionDeselect = createActionNone();
	}

	// Select object
	auto actionSelect = createActionSelectObject(pos);
	
	// Merge
	auto res = getMergedActions(actionDeselect, actionSelect);

	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::beginDragSelected(
	const PointF& pos)
{
	if (!m_isDragging) {
		m_dragStart = pos;
		m_isDragging = true;
	}
	return createActionNone();
}

const std::shared_ptr<StageEditorAction> StageEditor::endDragSelected(
	const PointF& pos, ObjectSnap snapping)
{
	if (m_isDragging) {
		PointF offsetPoint = pos.relativeTo(m_dragStart);
		PointF offsetPointSnap;
		getSnappedPoint(offsetPoint, snapping, offsetPointSnap);

		auto res = createActionMoveSelectedObjects(offsetPoint.x, offsetPoint.y);

		if (res->getType() != StageEditorAction::ACTION_NONE) {
			// Perform
			doAction(res);

			// Add to actions history
			m_history.pushAction(res);
		}

		m_isDragging = false;

		return res;
	} else {
		return createActionNone();
	}
}

StageEditor::StageEditor()
	: m_stageState{
		.width = STAGE_WIDTH_INITIAL,
		.height = STAGE_HEIGHT_INITIAL,
		.obstacles = std::unordered_map<EditorOID, StageEditorObstacleObject>(),
		.players = std::unordered_map<EditorOID, StageEditorPlayerObject>(),
	}
	, m_activeTool{TOOL_SELECT}
	, m_isDragging{false}
{}

const StageState& StageEditor::getState() const
{
	return m_stageState;
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
	return (getSelectedPlayerAt(pos) != EDITOR_OID_NULL)
		|| (getSelectedObstacleAt(pos) != EDITOR_OID_NULL);
}

const std::shared_ptr<StageEditorAction> StageEditor::activateTool(
	EditorTool newTool)
{
	auto res = std::make_shared<StageEditorActionActivateTool>(m_activeTool, newTool);
	
	m_activeTool = newTool;
	
	m_history.pushAction(res);

	return res;
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

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDownToolSelect(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed)
{
	(void)snapping;

	if (isSelectedObjectAt(pos)) {
		return beginDragSelected(pos);
	} else {
		return selectObject(pos, isShiftPressed);
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDownToolPlayers(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed)
{
	(void)isShiftPressed;

	return addPlayer(pos, snapping);
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDownToolObstacles(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed)
{
	(void)isShiftPressed;

	return addObstacleCorner(pos, snapping);
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDownToolDelete(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed)
{
	(void)pos;
	(void)snapping;
	(void)isShiftPressed;
	return createActionNone();
	// TODO
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnUpToolSelect(
	const PointF& pos, ObjectSnap snapping)
{
	return endDragSelected(pos, snapping);
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseRightBtnDownToolObstacles()
{
	return completeObstacle();
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDown(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed)
{
	switch (m_activeTool) {
		case TOOL_SELECT:
			return mouseLeftBtnDownToolSelect(pos, snapping, isShiftPressed);
			break;
		case TOOL_PLAYERS:
			return mouseLeftBtnDownToolPlayers(pos, snapping, isShiftPressed);
			break;
		case TOOL_OBSTACLES:
			return mouseLeftBtnDownToolObstacles(pos, snapping, isShiftPressed);
			break;
		case TOOL_DELETE:
			return mouseLeftBtnDownToolDelete(pos, snapping, isShiftPressed);
			break;
	}

	// Default
	return createActionNone();
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnUp(
	const PointF& pos, ObjectSnap snapping)
{
	if (m_activeTool == TOOL_SELECT) {
		return mouseLeftBtnUpToolSelect(pos, snapping);
	} else {
		return createActionNone();
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseRightBtnDown()
{
	if (m_activeTool == TOOL_OBSTACLES) {
		return mouseRightBtnDownToolObstacles();
	} else {
		return createActionNone();
	}
}
