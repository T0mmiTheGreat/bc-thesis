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

#include <cctype>
#include <vector>

#include "functions.hpp"
#include "math/Math.hpp"
#include "stageeditor/Common.hpp"
#include "stageserializer/StageSerializerFactory.hpp"

void StageEditor::getSnappedPoint(const PointF& p, ObjectSnap snapping,
	PointF& pSnap)
{
	if (snapping != OBJECT_SNAP_NONE) {
		pSnap.x = roundToMultiple(p.x, static_cast<PointF::ValueType>(snapping));
		pSnap.y = roundToMultiple(p.y, static_cast<PointF::ValueType>(snapping));
	} else {
		pSnap = p;
	}
}

void StageEditor::getSnappedOffset(const PointF& p0, const PointF& p1,
	ObjectSnap snapping, double& offsetX, double& offsetY)
{
	PointF offsetPoint = p1 - p0;
	PointF offsetPointSnapped;
	getSnappedPoint(offsetPoint, snapping, offsetPointSnapped);
	offsetX = offsetPointSnapped.x;
	offsetY = offsetPointSnapped.y;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionNone()
{
	return std::make_shared<StageEditorActionNone>();
}

std::shared_ptr<StageEditorAction> StageEditor::createActionAddPlayerObject(
	const PointF& pos)
{
	EditorOID oid = generateEditorOID();
	StageEditorPlayerObject playerTmp(oid, pos);
	auto res = std::make_shared<StageEditorActionAddPlayerObject>(playerTmp);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionPlaceObstacleCorner(
	const PointF& pos)
{
	auto res = std::make_shared<StageEditorActionPlaceObstacleCorner>(pos);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionAddObstacleObject()
{
	return createActionAddObstacleObject(m_obstacleCorners);
}

std::shared_ptr<StageEditorAction> StageEditor::createActionAddObstacleObject(
	const PolygonF& shape)
{
	EditorOID oid = generateEditorOID();
	StageEditorObstacleObject tmpObstacle(oid, shape);

	auto res = std::make_shared<StageEditorActionAddObstacleObject>(tmpObstacle);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionAbortObstacle()
{
	if (m_obstacleCorners.empty()) {
		return createActionNone();
	} else {
		std::vector<std::shared_ptr<StageEditorAction>> actionsGroup;
		std::shared_ptr<StageEditorAction> newAction;

		// Delete the corners
		for (auto i = m_obstacleCorners.rbegin(); i != m_obstacleCorners.rend(); ++i) {
			newAction = std::make_shared<StageEditorActionUnplaceObstacleCorner>(*i);
			actionsGroup.push_back(newAction);
		}

		auto res = getMergedActions(actionsGroup);
		return res;
	}
}

std::shared_ptr<StageEditorAction> StageEditor::createActionActivateTool(
	EditorTool newTool)
{
	auto res = std::make_shared<StageEditorActionActivateTool>(m_activeTool,
		newTool);
	return res;
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

std::shared_ptr<StageEditorAction> StageEditor::createActionDeselectPlayerObject(
	EditorOID oid)
{
	auto res = std::make_shared<StageEditorActionDeselectPlayerObject>(oid);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeselectObstacleObject(
	EditorOID oid)
{
	auto res = std::make_shared<StageEditorActionDeselectObstacleObject>(oid);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeselectPlayerIfSelected(
	EditorOID oid)
{
	if (m_selectedPlayers.contains(oid)) {
		return createActionDeselectPlayerObject(oid);
	} else {
		return createActionNone();
	}
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeselectObstacleIfSelected(
	EditorOID oid)
{
	if (m_selectedObstacles.contains(oid)) {
		return createActionDeselectObstacleObject(oid);
	} else {
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
		newAction = createActionDeselectPlayerObject(oid);
		actionGroup.push_back(newAction);
	}

	// Deselection of obstacles
	for (auto oid : m_selectedObstacles) {
		newAction = createActionDeselectObstacleObject(oid);
		actionGroup.push_back(newAction);
	}

	// Merge
	auto res = getMergedActions(actionGroup);

	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionBeginDragSelected(
	const PointF& where)
{
	auto res = std::make_shared<StageEditorActionBeginDragSelected>(where,
		m_selectedPlayers, m_selectedObstacles);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionMovePlayerObject(
	EditorOID oid, double dx, double dy)
{
	auto res = std::make_shared<StageEditorActionMovePlayerObject>(oid, dx, dy);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionMoveObstacleObject(
	EditorOID oid, double dx, double dy)
{
	auto res = std::make_shared<StageEditorActionMoveObstacleObject>(oid, dx,
		dy);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionMoveSelectedObjects(
	double dx, double dy)
{
	// The actions performed (all the moves)
	std::vector<std::shared_ptr<StageEditorAction>> actionGroup;
	std::shared_ptr<StageEditorAction> newAction;

	// Move players
	for (EditorOID oid : m_selectedPlayers) {
		newAction = createActionMovePlayerObject(oid, dx, dy);
		actionGroup.push_back(newAction);
	}

	// Move obstacles
	for (EditorOID oid : m_selectedObstacles) {
		newAction = createActionMoveObstacleObject(oid, dx, dy);
		actionGroup.push_back(newAction);
	}

	// Merge
	auto res = getMergedActions(actionGroup);

	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeletePlayerObject(
	EditorOID oid)
{
	auto res = std::make_shared<StageEditorActionDeletePlayerObject>(
		m_stageState.players.at(oid));
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeleteObstacleObject(
	EditorOID oid)
{
	auto res = std::make_shared<StageEditorActionDeleteObstacleObject>(
		m_stageState.obstacles.at(oid));
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeleteSelectedObjects()
{
	// The actions performed (all the deletions)
	std::vector<std::shared_ptr<StageEditorAction>> actionGroup;
	std::shared_ptr<StageEditorAction> newAction;

	// Delete players
	for (EditorOID oid : m_selectedPlayers) {
		newAction = createActionDeletePlayerObject(oid);
		actionGroup.push_back(newAction);
	}

	// Delete obstacles
	for (EditorOID oid : m_selectedObstacles) {
		newAction = createActionDeleteObstacleObject(oid);
		actionGroup.push_back(newAction);
	}

	// Merge
	auto res = getMergedActions(actionGroup);

	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionBeginDragStageCorner()
{
	auto res = std::make_shared<StageEditorActionBeginDragStageCorner>();
	return res;
}

std::shared_ptr<StageEditorAction> 
StageEditor::createActionResizeStage(int resizeX, int resizeY)
{
	auto res = std::make_shared<StageEditorActionResizeStage>(resizeX, resizeY);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionSetStageTitle(
	const std::string& newName)
{
	auto res = std::make_shared<StageEditorActionSetStageTitle>(
		m_stageState.title, newName);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeleteAllPlayers()
{
	// The actions performed (all the deletions)
	std::vector<std::shared_ptr<StageEditorAction>> actionGroup;
	std::shared_ptr<StageEditorAction> newAction;

	// Delete
	for (const auto& playerPair : m_stageState.players) {
		const auto& oid = playerPair.first;
		newAction = createActionDeletePlayerObject(oid);
		actionGroup.push_back(newAction);
	}

	// Merge
	auto res = getMergedActions(actionGroup);

	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeleteAllObstacles()
{
	// The actions performed (all the deletions)
	std::vector<std::shared_ptr<StageEditorAction>> actionGroup;
	std::shared_ptr<StageEditorAction> newAction;

	// Delete
	for (const auto& obstaclePair : m_stageState.obstacles) {
		const auto& oid = obstaclePair.first;
		newAction = createActionDeleteObstacleObject(oid);
		actionGroup.push_back(newAction);
	}

	// Merge
	auto res = getMergedActions(actionGroup);

	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeleteAllObjects()
{
	// Delete players
	auto actionDeletePlayers = createActionDeleteAllPlayers();
	// Delete obstacles
	auto actionDeleteObstacles = createActionDeleteAllObstacles();

	// Merge
	auto res = getMergedActions(actionDeletePlayers, actionDeleteObstacles);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionClearStage()
{
	// XXX: Kinda bad, likely to cause problems...

	// Stop obstacle creation
	auto actionAbortObstacle = createActionAbortObstacle();
	// Stop all dragging
	auto actionEndDragSelected = createActionMoveSelectedObjects(0, 0);
	auto actionEndDragStageCorner = createActionResizeStage(0, 0);
	// Deselect all objects
	auto actionDeselect = createActionDeselectAll();
	// Delete all objects
	auto actionDelete = createActionDeleteAllObjects();

	// Merge
	auto res = getMergedActions(actionAbortObstacle, actionEndDragSelected,
		actionEndDragStageCorner, actionDeselect, actionDelete);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionResetStage()
{
	// Reset title
	auto actionSetTitle = createActionSetStageTitle(
		StageState::STAGE_TITLE_DEFAULT);
	// Reset size
	int resizeX = StageState::STAGE_WIDTH_INITIAL - m_stageState.width;
	int resizeY = StageState::STAGE_HEIGHT_INITIAL - m_stageState.height;
	auto actionResize = createActionResizeStage(resizeX, resizeY);

	// Merge
	auto res = getMergedActions(actionSetTitle, actionResize);
	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::createActionDeserializeStage(
	const std::shared_ptr<IStageSerializer>& data)
{
	return deserializeStage(data);
}

std::shared_ptr<StageEditorAction> StageEditor::createActionAssignStageId(
	const IStageSerializer::IdType& id)
{
	if (!m_stageState.isIdAssociated()) {
		return std::make_shared<StageEditorActionAssignStageId>(id);
	} else {
		return createActionNone();
	}
}

std::shared_ptr<StageEditorAction> StageEditor::createActionRemoveStageId()
{
	if (m_stageState.isIdAssociated()) {
		return std::make_shared<StageEditorActionRemoveStageId>(
			m_stageState.stageId);
	} else {
		return createActionNone();
	}
}

std::shared_ptr<StageEditorAction> StageEditor::createActionChangeStageId(
	const IStageSerializer::IdType& id)
{
	auto actionRemove = createActionRemoveStageId();
	auto actionAssign = createActionAssignStageId(id);
	auto res = getMergedActions(actionRemove, actionAssign);
	return res;
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
		case StageEditorAction::ACTION_ADD_PLAYER_OBJECT:
			doActionAddPlayerObject(action);
			break;
		case StageEditorAction::ACTION_ADD_OBSTACLE_OBJECT:
			doActionAddObstacleObject(action);
			break;
		case StageEditorAction::ACTION_PLACE_OBSTACLE_CORNER:
			doActionPlaceObstacleCorner(action);
			break;
		case StageEditorAction::ACTION_UNPLACE_OBSTACLE_CORNER:
			doActionUnplaceObstacleCorner(action);
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
		case StageEditorAction::ACTION_BEGIN_DRAG_SELECTED:
			doActionBeginDragSelected(action);
			break;
		case StageEditorAction::ACTION_MOVE_PLAYER_OBJECT:
			doActionMovePlayerObject(action);
			break;
		case StageEditorAction::ACTION_MOVE_OBSTACLE_OBJECT:
			doActionMoveObstacleObject(action);
			break;
		case StageEditorAction::ACTION_DELETE_PLAYER_OBJECT:
			doActionDeletePlayerObject(action);
			break;
		case StageEditorAction::ACTION_DELETE_OBSTACLE_OBJECT:
			doActionDeleteObstacleObject(action);
			break;
		case StageEditorAction::ACTION_BEGIN_DRAG_STAGE_CORNER:
			doActionBeginDragStageCorner(action);
			break;
		case StageEditorAction::ACTION_RESIZE_STAGE:
			doActionResizeStage(action);
			break;
		case StageEditorAction::ACTION_SET_STAGE_TITLE:
			doActionSetStageTitle(action);
			break;
		case StageEditorAction::ACTION_ASSIGN_STAGE_ID:
			doActionAssignStageId(action);
			break;
		case StageEditorAction::ACTION_REMOVE_STAGE_ID:
			doActionRemoveStageId(action);
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

void StageEditor::doActionAddPlayerObject(const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionAddPlayerObject>(action);
	
	EditorOID oid = actionCast->getPlayerObject().getOid();

	m_stageState.players.emplace(oid, actionCast->getPlayerObject());
}

void StageEditor::doActionAddObstacleObject(const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionAddObstacleObject>(action);
	
	EditorOID oid = actionCast->getObstacleObject().getOid();
	
	m_stageState.obstacles.emplace(oid, actionCast->getObstacleObject());
}

void StageEditor::doActionPlaceObstacleCorner(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionPlaceObstacleCorner>(action);
	
	const PointF& pos = actionCast->getPos();

	m_obstacleCorners.push_back(pos);
}

void StageEditor::doActionUnplaceObstacleCorner(
	const std::shared_ptr<StageEditorAction> action)
{
	(void)action;

	m_obstacleCorners.pop_back();
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

void StageEditor::doActionBeginDragSelected(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionBeginDragSelected>(action);
	
	m_dragStartSelected = actionCast->getWhere();
	m_isDraggingSelected = true;
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
	player.moveBy(dx, dy);
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
	obstacle.moveBy(dx, dy);
}

void StageEditor::doActionDeletePlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeletePlayerObject>(action);
	
	EditorOID oid = actionCast->getObject().getOid();

	m_stageState.players.erase(oid);
}

void StageEditor::doActionDeleteObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeleteObstacleObject>(action);
	
	EditorOID oid = actionCast->getObject().getOid();

	m_stageState.obstacles.erase(oid);
}

void StageEditor::doActionBeginDragStageCorner(
	const std::shared_ptr<StageEditorAction> action)
{
	(void)action;

	m_isDraggingStage = true;
}

void StageEditor::doActionResizeStage(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionResizeStage>(action);
	
	int resizeX = actionCast->getResizeX();
	int resizeY = actionCast->getResizeY();

	m_stageState.width += resizeX;
	m_stageState.height += resizeY;

	m_isDraggingStage = false;
}

void StageEditor::doActionSetStageTitle(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionSetStageTitle>(action);
	
	m_stageState.title = actionCast->getNewName();
}

void StageEditor::doActionAssignStageId(
	const std::shared_ptr<StageEditorAction> action)
{
	auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionAssignStageId>(action);
	
	m_stageState.assignId(actionCast->getId());
}

void StageEditor::doActionRemoveStageId(
	const std::shared_ptr<StageEditorAction> action)
{
	(void)action;

	m_stageState.removeId();
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

	if (!canCreatePlayer(posSnap)) {
		return createActionNone();
	} else {
		// Deselect objects, if any are selected
		auto actionDeselect = createActionDeselectAll();

		// Add player
		auto actionAddPlayer = createActionAddPlayerObject(posSnap);

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
}

const std::shared_ptr<StageEditorAction> StageEditor::addObstacleCorner(
	const PointF& pos, ObjectSnap snapping)
{
	// Snap position to grid
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);

	if (!canPlaceCorner(posSnap)) {
		return createActionNone();
	} else {
		// Deselect objects, if any are selected
		auto actionDeselect = createActionDeselectAll();

		// Place obstacle corner
		auto actionPlaceCorner = createActionPlaceObstacleCorner(posSnap);

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
}

const std::shared_ptr<StageEditorAction> StageEditor::completeObstacle()
{
	// Complete obstacle
	std::shared_ptr<StageEditorAction> res;
	if (!canCompleteObstacle()) {
		res = createActionNone();
	} else {
		// Remove the corners
		auto actionAbortObstacle = createActionAbortObstacle();
		// Create obstacle from the corners
		auto actionAddObstacle = createActionAddObstacleObject();
		// Merge
		res = getMergedActions(actionAbortObstacle, actionAddObstacle);
	}
	
	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::activateTool(
	EditorTool tool)
{
	std::shared_ptr<StageEditorAction> res;

	if (m_activeTool == tool) {
		// Do nothing

		res = createActionNone();
	} else {
		// Abort obstacle
		auto actionAbortObstacle = createActionAbortObstacle();

		// Activate tool
		auto actionActivateTool = createActionActivateTool(tool);

		res = getMergedActions(actionAbortObstacle, actionActivateTool);
	}

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
	std::shared_ptr<StageEditorAction> res;

	if (!m_isDraggingSelected) {
		res = createActionBeginDragSelected(pos);
		doAction(res);
	} else {
		res = createActionNone();
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::endDragSelected(
	const PointF& pos, ObjectSnap snapping)
{
	if (m_isDraggingSelected) {
		PointF offsetPoint = pos.relativeTo(m_dragStartSelected);
		PointF offsetPointSnap;
		getSnappedPoint(offsetPoint, snapping, offsetPointSnap);

		std::shared_ptr<StageEditorAction> res;
		if (canMoveSelectedObjects(offsetPointSnap.x, offsetPointSnap.y)){
			res = createActionMoveSelectedObjects(offsetPointSnap.x,
				offsetPointSnap.y);
		} else {
			res = createActionMoveSelectedObjects(0.0, 0.0);
		}

		if (res->getType() != StageEditorAction::ACTION_NONE) {
			// Perform
			doAction(res);

			// Add to actions history
			m_history.pushAction(res);
		}

		m_isDraggingSelected = false;

		return res;
	} else {
		return createActionNone();
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::deleteSelected()
{
	// First deselect them
	auto actionDeselect = createActionDeselectAll();

	// Then delete them
	auto actionDelete = createActionDeleteSelectedObjects();

	// Merge
	auto res = getMergedActions(actionDeselect, actionDelete);

	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::deleteObject(
	const PointF& pos)
{
	EditorOID oid;

	std::shared_ptr<StageEditorAction> actionDeselect, actionDelete, res;

	if ((oid = getPlayerObjectAt(pos)) != EDITOR_OID_NULL) {
		// Deleted player object

		// It could be selected...
		actionDeselect = createActionDeselectPlayerIfSelected(oid);
		// Delete
		actionDelete = createActionDeletePlayerObject(oid);
		// Merge
		res = getMergedActions(actionDeselect, actionDelete);
	} else if ((oid = getObstacleObjectAt(pos)) != EDITOR_OID_NULL) {
		// Deleted obstacle object

		// It could be selected...
		actionDeselect = createActionDeselectObstacleIfSelected(oid);
		// Delete
		actionDelete = createActionDeleteObstacleObject(oid);
		// Merge
		res = getMergedActions(actionDeselect, actionDelete);
	} else {
		// Didn't delete anything

		res = createActionNone();
	}

	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::beginDragStageCorner()
{
	if (!m_isDraggingStage) {
		auto res = createActionBeginDragStageCorner();
		doAction(res);
		return res;
	} else {
		return createActionNone();
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::endDragStageCorner(
	const PointF& pos, ObjectSnap snapping)
{
	if (m_isDraggingStage) {
		bool isSuccess;
		auto res = predictEndDragStageCorner(pos, snapping, isSuccess);

		if (!isSuccess) {
			res = createActionResizeStage(0, 0);
		}

		if (res->getType() != StageEditorAction::ACTION_NONE) {
			// Perform
			doAction(res);

			// Add to actions history
			m_history.pushAction(res);
		}

		return res;
	} else {
		return createActionNone();
	}
}

void StageEditor::checkStageResizeLimits(int& resizeX, int& resizeY)
{
	int newW = m_stageState.width + resizeX;
	int newH = m_stageState.height + resizeY;

	if (newW < m_stageState.STAGE_WIDTH_MIN) {
		resizeX += (m_stageState.STAGE_WIDTH_MIN - newW);
	}
	if (newH < m_stageState.STAGE_HEIGHT_MIN) {
		resizeY += (m_stageState.STAGE_HEIGHT_MIN - newH);
	}
}

std::shared_ptr<IStageSerializer> StageEditor::serializeStage()
{
	auto res = StageSerializerFactory::createDefault();
	// Title
	res->setTitle(m_stageState.title);
	// Width
	res->setWidth(m_stageState.width);
	// Height
	res->setHeight(m_stageState.height);

	// Players -- `m_stageState.players` as a vector
	std::vector<IStageSerializer::PlayerType> players_;
	players_.reserve(m_stageState.players.size());
	for (const auto& playerPair : m_stageState.players) {
		const auto& player = playerPair.second;
		players_.push_back(player.pos);
	}
	res->setPlayers(players_);

	// Obstacles -- `m_stageState.obstacles` as a vector of triangles
	std::vector<IStageSerializer::ObstacleType> obstacles_;
	std::vector<TriangleF> pogTriang;
	for (const auto& obstaclePair : m_stageState.obstacles) {
		// Get the obstacle
		const auto& obstacle = obstaclePair.second;
		// Triangulate
		obstacle.shape.triangulate(pogTriang);
		vectorAppendRange(obstacles_, pogTriang);
	}
	res->setObstacles(obstacles_);

	// Position rules -- just a sequence of numbers:
	// `0 -> players_.size() - 1`
	// FIXME: Editor should allow making custom position rules

	// The list
	std::vector<IStageSerializer::PositionRuleType> positionRules_;
	// A single rule
	IStageSerializer::PositionRuleType positionRule_;
	positionRule_.reserve(players_.size());
	// Create the rule
	for (size_t i = 0; i < players_.size(); i++) positionRule_.push_back(i);
	// Insert to the rules list
	positionRules_.push_back(std::move(positionRule_));
	res->setPositionRules(positionRules_);

	return res;
}

std::shared_ptr<StageEditorAction> StageEditor::deserializeStage(
	const std::shared_ptr<IStageSerializer> data)
{
	std::shared_ptr<StageEditorAction> newAction;

	// Set title
	auto actionSetTitle = createActionSetStageTitle(data->getTitle());
	// Set size
	int resizeX = data->getWidth() - m_stageState.width;
	int resizeY = data->getHeight() - m_stageState.height;
	auto actionResize = createActionResizeStage(resizeX, resizeY);
	// Set players
	std::vector<std::shared_ptr<StageEditorAction>> actionsGroupAddPlayer;
	for (const auto& pos : data->getPlayers()) {
		newAction = createActionAddPlayerObject(pos);
		actionsGroupAddPlayer.push_back(newAction);
	}
	auto actionAddPlayers = getMergedActions(actionsGroupAddPlayer);
	// Set obstacles
	std::vector<std::shared_ptr<StageEditorAction>> actionsGroupAddObstacle;
	for (const auto& shape : data->getObstacles()) {
		newAction = createActionAddObstacleObject(shape);
		actionsGroupAddObstacle.push_back(newAction);
	}
	auto actionAddObstacles = getMergedActions(actionsGroupAddObstacle);

	// Merge
	auto res = getMergedActions(actionSetTitle, actionResize, actionAddPlayers,
		actionAddObstacles);
	
	return res;
}

StageEditor::StageEditor()
	: m_stageState()
	, m_activeTool{TOOL_SELECT}
	, m_isDraggingSelected{false}
	, m_isDraggingStage{false}
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

const std::unordered_set<EditorOID>& StageEditor::getSelectedPlayers() const
{
	return m_selectedPlayers;
}

const std::unordered_set<EditorOID>& StageEditor::getSelectedObstacles() const
{
	return m_selectedObstacles;
}

bool StageEditor::isDraggingStageCorner() const
{
	return m_isDraggingStage;
}

void StageEditor::getStageFileName(std::string& fn)
{
	bool devnull;
	getStageFileName(fn, devnull);
}

void StageEditor::getStageFileName(std::string& fn, bool& isAutoGenerated)
{
	getStageFileName(m_stageState.title, fn, isAutoGenerated);
}

void StageEditor::getStageFileName(const std::string& stageTitle,
	std::string& fn, bool& isAutoGenerated)
{
	auto serializer = StageSerializerFactory::createDefault();

	if (m_stageState.isIdAssociated()) {
		// Already has a file -- return it

		auto id = m_stageState.stageId;
		fn = serializer->getStageCanonicalId(id);
		isAutoGenerated = false;
	} else {
		// Doesn't have a file yet -- auto-generate

		auto id = serializer->generateIdByTitle(stageTitle);
		fn = serializer->getStageCanonicalId(id);
		isAutoGenerated = true;
	}
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

const std::shared_ptr<StageEditorAction> StageEditor::undo()
{
	if (!canUndo()) {
		return createActionNone();
	} else {
		auto undoneAction = m_history.undoAction();
		auto undoneActionInv = undoneAction->createInverse();

		doAction(undoneActionInv);
		return undoneActionInv;
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::redo()
{
	if (!canRedo()) {
		return createActionNone();
	} else {
		auto redoneAction = m_history.redoAction();

		doAction(redoneAction);
		return redoneAction;
	}
}

bool StageEditor::canUndo()
{
	return m_history.canUndo();
}

bool StageEditor::canRedo()
{
	return m_history.canRedo();
}

const std::shared_ptr<StageEditorAction> StageEditor::newStage()
{
	// Clear
	auto actionClear = createActionClearStage();
	// Reset
	auto actionReset = createActionResetStage();
	// Remove ID
	auto actionRemoveId = createActionRemoveStageId();

	// Merge
	auto res = getMergedActions(actionClear, actionReset, actionRemoveId);
	
	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

void StageEditor::save()
{
	if (!m_stageState.isIdAssociated()) {
		// Get stage ID
		auto serializer = StageSerializerFactory::createDefault();
		auto id = serializer->generateIdByTitle(m_stageState.title);

		// Assign ID to stage
		auto actionAssignId = createActionAssignStageId(id);
		if (actionAssignId->getType() != StageEditorAction::ACTION_NONE) {
			doAction(actionAssignId);
			m_history.pushAction(actionAssignId);
		}
	}

	// Serialize
	auto serializer = serializeStage();

	// Save
	serializer->save(m_stageState.stageId);
}

const std::shared_ptr<StageEditorAction> StageEditor::load(
	const IStageSerializer::IdType& id)
{
	// Clear
	auto actionClear = createActionClearStage();
	// Load
	auto data = StageSerializerFactory::createDefault();
	data->load(id);
	auto actionLoad = createActionDeserializeStage(data);

	// Merge
	auto res = getMergedActions(actionClear, actionLoad);
	
	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	// Assign ID to stage
	m_stageState.assignId(id);

	return res;
}

const std::shared_ptr<StageEditorAction>
StageEditor::mouseLeftBtnDownToolSelect(const PointF& pos, ObjectSnap snapping,
	bool isShiftPressed, double grabZoneSize)
{
	(void)snapping;
	(void)grabZoneSize;

	if (isSelectedObjectAt(pos)) {
		return beginDragSelected(pos);
	} else {
		return selectObject(pos, isShiftPressed);
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDownToolPlayers(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed,
	double grabZoneSize)
{
	(void)isShiftPressed;
	(void)grabZoneSize;

	return addPlayer(pos, snapping);
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDownToolObstacles(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed,
	double grabZoneSize)
{
	(void)isShiftPressed;
	(void)grabZoneSize;

	return addObstacleCorner(pos, snapping);
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDownToolDelete(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed,
	double grabZoneSize)
{
	(void)snapping;
	(void)isShiftPressed;
	(void)grabZoneSize;
	
	if (isSelectedObjectAt(pos)) {
		return deleteSelected();
	} else {
		return deleteObject(pos);
	}
}

const std::shared_ptr<StageEditorAction>
StageEditor::mouseLeftBtnDownToolResizeStage(const PointF& pos,
	ObjectSnap snapping, bool isShiftPressed, double grabZoneSize)
{
	(void)snapping;
	(void)isShiftPressed;
	
	PointF stageCorner(m_stageState.width, m_stageState.height);

	if (pos.sqrDistance(stageCorner) <= sqr(grabZoneSize)) {
		return beginDragStageCorner();
	} else {
		return createActionNone();
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnUpToolSelect(
	const PointF& pos, ObjectSnap snapping)
{
	return endDragSelected(pos, snapping);
}

const std::shared_ptr<StageEditorAction>
StageEditor::mouseLeftBtnUpToolResizeStage(const PointF& pos,
	ObjectSnap snapping)
{
	return endDragStageCorner(pos, snapping);
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseRightBtnDownToolObstacles()
{
	return completeObstacle();
}

bool StageEditor::canPlacePlayer(const StageEditorPlayerObject& player)
{
	std::unordered_set<EditorOID> dontIgnore;
	return canPlacePlayer(player, dontIgnore, dontIgnore);
}

bool StageEditor::canPlacePlayer(const StageEditorPlayerObject& player,
		const std::unordered_set<EditorOID>& ignoredPlayers,
	const std::unordered_set<EditorOID>& ignoredObstacles)
{
	// Check that the whole player is within the stage bounds

	// Get the stage bounds
	RectF stageBounds(0.0, 0.0, static_cast<Size2dF>(m_stageState.getSize()));
	// Decrement by the player diameter
	RectF stageBoundsDeflated = stageBounds.getInflated(-player.getRadius() * 2);
	if (!stageBoundsDeflated.containsPoint(player.pos)) {
		return false;
	}

	// Check collisions with other players
	for (const auto& playerPair : m_stageState.players) {
		const EditorOID& oid = playerPair.first;
		const auto& object = playerPair.second;

		if (ignoredPlayers.contains(oid)) continue;

		if (object.collidesWithPlayer(player)) {
			return false;
		}
	}

	// Check collisions with obstacles
	for (const auto& obstaclePair : m_stageState.obstacles) {
		const EditorOID& oid = obstaclePair.first;
		const auto& object = obstaclePair.second;

		if (ignoredObstacles.contains(oid)) continue;

		if (object.collidesWithPlayer(player)) {
			return false;
		}
	}

	// No collisions
	return true;	
}

bool StageEditor::canPlaceObstacle(const StageEditorObstacleObject& obstacle)
{
	std::unordered_set<EditorOID> dontIgnore;
	return canPlaceObstacle(obstacle, dontIgnore);
}

bool StageEditor::canPlaceObstacle(const StageEditorObstacleObject& obstacle,
	const std::unordered_set<EditorOID>& ignoredPlayers)
{
	// Check collisions with players
	for (const auto& playerPair : m_stageState.players) {
		EditorOID oid = playerPair.first;
		const auto& object = playerPair.second;

		if (ignoredPlayers.contains(oid)) continue;

		if (obstacle.collidesWithPlayer(object)) {
			return false;
		}
	}

	// No collisions
	return true;
}

bool StageEditor::canCreatePlayer(const PointF& pos)
{
	StageEditorPlayerObject tmpPlayer(EDITOR_OID_NULL, pos);

	return canPlacePlayer(tmpPlayer);
}

bool StageEditor::canConstructEdge(const PointF& pos, bool isClosing)
{
	if (isClosing && m_obstacleCorners.size() < 3) {
		// Cannot close an obstacle if its shape is not a valid Euclidean
		// polygon

		return false;
	} else if (!isClosing && m_obstacleCorners.empty()) {
		// The first corner can always be placed

		return true;
	} else {
		// The edge end points
		PointF edgeP0 = m_obstacleCorners.back();
		PointF edgeP1 = (isClosing ? m_obstacleCorners.front() : pos);

		if (edgeP0 == edgeP1) {
			// Cannot construct edge of zero length

			return false;
		}

		// For convenience we will treat it as a degenerate obstacle
		StageEditorObstacleObject tmpObstacle(EDITOR_OID_NULL,
			PolygonF(edgeP0, edgeP1));
		
		// Check collisions with already placed edges
		for (size_t cornerIdx = 0; cornerIdx < m_obstacleCorners.size() - 1; cornerIdx++) {
			const PointF& placedEdgeP0 = m_obstacleCorners[cornerIdx];
			const PointF& placedEdgeP1 = m_obstacleCorners[cornerIdx + 1];

			if ((isClosing && cornerIdx == 0) // Closing the obstacle + this is the first edge
			    || cornerIdx == m_obstacleCorners.size() - 2) // This is the last edge
			{
				if (areVectorsAntiparallel(edgeP1.x - edgeP0.x, edgeP1.y - edgeP0.y,
					placedEdgeP1.x - placedEdgeP0.x, placedEdgeP1.y - placedEdgeP0.y))
				{
					// The problem is this:
					//
					// placedEdgeP0  edgeP1  placedEdgeP1 = edgeP0
					//      +-----------+=================+

					return false;
				}
			} else {
				if (isLineSegmentsCross(edgeP0.x, edgeP0.y, edgeP1.x, edgeP1.y,
					placedEdgeP0.x, placedEdgeP0.y, placedEdgeP1.x, placedEdgeP1.y))
				{
					// Crossing with already placed edge

					return false;
				}
			}
		}

		// Finally, check collisions with players
		return canPlaceObstacle(tmpObstacle);
	}
}

bool StageEditor::canPlaceCorner(const PointF& pos)
{
	return canConstructEdge(pos, false);
}

bool StageEditor::canCompleteObstacle()
{
	if (!canConstructEdge(m_obstacleCorners.front(), true)) {
		return false;
	} else {
		StageEditorObstacleObject tmpObstacle(EDITOR_OID_NULL,
			PolygonF(m_obstacleCorners));
		
		return canPlaceObstacle(tmpObstacle);
	}
}

bool StageEditor::canMoveSelectedPlayer(EditorOID oid, double dx, double dy)
{
	StageEditorPlayerObject tmpPlayer(m_stageState.players.at(oid));
	tmpPlayer.moveBy(dx, dy);

	return canPlacePlayer(tmpPlayer, m_selectedPlayers, m_selectedObstacles);
}

bool StageEditor::canMoveSelectedObstacle(EditorOID oid, double dx, double dy)
{
	StageEditorObstacleObject tmpObstacle(m_stageState.obstacles.at(oid));
	tmpObstacle.moveBy(dx, dy);

	return canPlaceObstacle(tmpObstacle, m_selectedPlayers);
}

bool StageEditor::canMoveSelectedObjects(double dx, double dy)
{
	// Check can move selected players
	for (EditorOID oid : m_selectedPlayers) {
		if (!canMoveSelectedPlayer(oid, dx, dy)) {
			return false;
		}
	}
	
	// Check can move selected obstacles
	for (EditorOID oid : m_selectedObstacles) {
		if (!canMoveSelectedObstacle(oid, dx, dy)) {
			return false;
		}
	}

	// Can move all objects
	return true;
}

bool StageEditor::canResizeStage(int dx, int dy)
{
	RectF newBounds(0.0, 0.0, m_stageState.width + dx,
		m_stageState.height + dy);
	RectF newBoundsDeflated = newBounds.getInflated(-EDITOR_PLAYER_RADIUS * 2);
	
	// All players must be within the new bounds
	for (const auto& playerPair : m_stageState.players) {
		const auto& player = playerPair.second;

		if (!newBoundsDeflated.containsPoint(player.pos)) {
			return false;
		}
	}

	return true;
}

const std::shared_ptr<StageEditorAction> StageEditor::toolLeftBtnDown(
	EditorTool tool)
{
	return activateTool(tool);
}

const std::shared_ptr<StageEditorAction> StageEditor::setStageTitle(
	const std::string& newName)
{
	auto res = createActionSetStageTitle(newName);

	if (res->getType() != StageEditorAction::ACTION_NONE) {
		// Perform
		doAction(res);

		// Add to actions history
		m_history.pushAction(res);
	}

	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnDown(
	const PointF& pos, ObjectSnap snapping, bool isShiftPressed,
	double grabZoneSize)
{
	switch (m_activeTool) {
		case TOOL_SELECT:
			return mouseLeftBtnDownToolSelect(pos, snapping, isShiftPressed,
				grabZoneSize);
			break;
		case TOOL_PLAYERS:
			return mouseLeftBtnDownToolPlayers(pos, snapping, isShiftPressed,
				grabZoneSize);
			break;
		case TOOL_OBSTACLES:
			return mouseLeftBtnDownToolObstacles(pos, snapping, isShiftPressed,
				grabZoneSize);
			break;
		case TOOL_DELETE:
			return mouseLeftBtnDownToolDelete(pos, snapping, isShiftPressed,
				grabZoneSize);
			break;
		case TOOL_RESIZE_STAGE:
			return mouseLeftBtnDownToolResizeStage(pos, snapping,
				isShiftPressed, grabZoneSize);
			break;
	}

	// Default
	return createActionNone();
}

const std::shared_ptr<StageEditorAction> StageEditor::mouseLeftBtnUp(
	const PointF& pos, ObjectSnap snapping)
{
	switch (m_activeTool) {
		case TOOL_SELECT:
			return mouseLeftBtnUpToolSelect(pos, snapping);
		case TOOL_RESIZE_STAGE:
			return mouseLeftBtnUpToolResizeStage(pos, snapping);
		default:
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

const std::shared_ptr<StageEditorAction> StageEditor::predictAddPlayer(
	const PointF& pos, ObjectSnap snapping, bool& isSuccess)
{
	PointF posSnapped;
	getSnappedPoint(pos, snapping, posSnapped);

	auto res = createActionAddPlayerObject(posSnapped);
	isSuccess = canCreatePlayer(posSnapped);
	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::predictPlaceObstacleCorner(
	const PointF& pos, ObjectSnap snapping, bool& isSuccess)
{
	PointF posSnapped;
	getSnappedPoint(pos, snapping, posSnapped);

	auto res = createActionPlaceObstacleCorner(posSnapped);
	isSuccess = canPlaceCorner(posSnapped);
	return res;
}

const std::shared_ptr<StageEditorAction> StageEditor::predictEndDragPlayerObject(
	EditorOID oid, const PointF& pos, ObjectSnap snapping, bool& isSuccess)
{
	if (!m_selectedPlayers.contains(oid) || !m_isDraggingSelected) {
		isSuccess = true;
		return createActionNone();
	} else {
		double dx, dy;
		getSnappedOffset(m_dragStartSelected, pos, snapping, dx, dy);

		auto res = createActionMovePlayerObject(oid, dx, dy);
		isSuccess = canMoveSelectedPlayer(oid, dx, dy);
		return res;
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::predictEndDragObstacleObject(
	EditorOID oid, const PointF& pos, ObjectSnap snapping, bool& isSuccess)
{
	if (!m_selectedObstacles.contains(oid) || !m_isDraggingSelected) {
		isSuccess = true;
		return createActionNone();
	} else {
		double dx, dy;
		getSnappedOffset(m_dragStartSelected, pos, snapping, dx, dy);

		auto res = createActionMoveObstacleObject(oid, dx, dy);
		isSuccess = canMoveSelectedObstacle(oid, dx, dy);
		return res;
	}
}

const std::shared_ptr<StageEditorAction> StageEditor::predictEndDragStageCorner(
	const PointF& pos, ObjectSnap snapping, bool& isSuccess)
{
	if (!m_isDraggingStage) {
		isSuccess = true;
		return createActionNone();
	} else {
		PointF stageBottomRight(m_stageState.width, m_stageState.height);
		double dx, dy;
		getSnappedOffset(stageBottomRight, pos, snapping, dx, dy);

		int resizeX = static_cast<int>(dx);
		int resizeY = static_cast<int>(dy);
		checkStageResizeLimits(resizeX, resizeY);

		auto res = createActionResizeStage(resizeX, resizeY);
		isSuccess = canResizeStage(resizeX, resizeY);
		return res;
	}
}
