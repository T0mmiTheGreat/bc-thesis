/**
 * @file StageEditorController.cpp
 * @author Tomáš Ludrovan
 * @brief StageEditorController class
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "controller/StageEditorController.hpp"

#include "controller/ControllerFactory.hpp"
#include "stageeditor/Common.hpp"

StageEditorController::StageEditorController(
	std::shared_ptr<ISysProxy> sysProxy)
	: GeneralControllerBase(sysProxy)
	, m_viewport(static_cast<Size2dF>(m_stageEditor.getState().getSize()),
		static_cast<Size2dF>(getWorkspaceRect().getSize()))
	, m_brushSprite{nullptr}
{}

void StageEditorController::createSprites()
{
	// Menu icons
	m_menuIcons[MENUICON_NEW_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_NEW);
	m_menuIcons[MENUICON_OPEN_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_OPEN);
	m_menuIcons[MENUICON_SAVE_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_SAVE);
	m_menuIcons[MENUICON_SAVE_AS_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_SAVE_AS);
	m_menuIcons[MENUICON_UNDO_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_UNDO);
	m_menuIcons[MENUICON_REDO_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_REDO);
	m_menuIcons[MENUICON_PROPERTIES_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_COGWHEEL);
	m_menuIcons[MENUICON_BACK_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_BACK);

	// Menubar line
	m_menuBarSprite = std::make_unique<OptionBarSprite>(sysProxy);

	// Tool icons
	m_toolIcons[TOOLICON_SELECT_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_SELECT_TOOL);
	m_toolIcons[TOOLICON_PLAYER_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_PLAYER_TOOL);
	m_toolIcons[TOOLICON_OBSTACLE_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_OBSTACLE_TOOL);
	m_toolIcons[TOOLICON_TRASH_CAN_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_TRASH_CAN);
	m_toolIcons[TOOLICON_RESIZE_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_RESIZE);

	// Toolbar line
	m_toolBarSprite = std::make_unique<OptionBarSprite>(sysProxy);

	// Statusbar text
	m_statusBarText = std::make_unique<StatusbarTextSprite>(sysProxy);
	// Statusbar line
	m_statusBarSprite = std::make_unique<OptionBarSprite>(sysProxy);

	// Grid
	m_gridSprite = std::make_unique<EditorWorkspaceGridSprite>(sysProxy);

	// Open obstacle
	m_obstacleEdges = std::make_unique<ObstacleEdgesSprite>(sysProxy);

	// Brushes
	m_playerBrush = std::make_unique<PlayerBrushSprite>(sysProxy);
	m_obstacleBrush = std::make_unique<ObstacleBrushSprite>(sysProxy);
}

void StageEditorController::initializeSprites()
{
	createSprites();
	positionSprites();

	int activeToolIconIdx = toolToIconIdx(m_stageEditor.getActiveTool());
	toolIconSetSelected(activeToolIconIdx);

	updateSpritesByViewport();
	updateToolBrush();
	updateUndoRedoIcons();
}

void StageEditorController::positionSprites()
{
	// Menu icons
	for (int iconIdx = 0; iconIdx < MENUICON_COUNT; iconIdx++) {
		m_menuIcons[iconIdx]->setPos(getMenuIconRect(iconIdx).getTopLeft());
	}
	// Menubar
	m_menuBarSprite->setPos(0, 0);
	m_menuBarSprite->setBarWidth(0);
	m_menuBarSprite->setBarHeight(MENUBAR_HEIGHT);
	m_menuBarSprite->setFillingColor(MENUBAR_FCOLOR);
	m_menuBarSprite->setStrokingColor(MENUBAR_SCOLOR);
	m_menuBarSprite->setBorders(false, false, false, true);

	// Tool icons
	for (int iconIdx = 0; iconIdx < TOOLICON_COUNT; iconIdx++) {
		m_toolIcons[iconIdx]->setPos(getToolIconRect(iconIdx).getTopLeft());
	}
	// Toolbar
	m_toolBarSprite->setPos(0, TOOLBAR_Y);
	m_toolBarSprite->setBarWidth(TOOLBAR_WIDTH);
	m_toolBarSprite->setBarHeight(-STATUSBAR_HEIGHT);
	m_toolBarSprite->setFillingColor(TOOLBAR_FCOLOR);
	m_toolBarSprite->setStrokingColor(TOOLBAR_SCOLOR);
	m_toolBarSprite->setBorders(false, false, true, false);

	Rect statusbarRect = getStatusbarRect();
	// Statusbar text
	m_statusBarText->setPos(
		statusbarRect.x + STATUSBAR_TEXT_LEFT_MARGIN,
		statusbarRect.y + ((statusbarRect.h - m_statusBarText->getH()) / 2)
	);
	// Statusbar
	m_statusBarSprite->setPos(statusbarRect.getTopLeft());
	m_statusBarSprite->setBarWidth(0);
	m_statusBarSprite->setBarHeight(0);
	m_statusBarSprite->setFillingColor(STATUSBAR_FCOLOR);
	m_statusBarSprite->setStrokingColor(STATUSBAR_SCOLOR);
	m_statusBarSprite->setBorders(false, true, false, false);
}

void StageEditorController::mouseMoveMenubar(int x, int y)
{
	checkMenuIconMouseHover(x, y);
}

void StageEditorController::mouseMoveToolbar(int x, int y)
{
	checkToolIconMouseHover(x, y);
}

void StageEditorController::mouseMoveWorkspace(int x, int y)
{
	checkWorkspaceDoDrag(x, y);
}

void StageEditorController::mouseBtnDownMenubar(MouseBtn btn, int x, int y)
{
	if (btn == BTN_LEFT) {
		checkMenuIconClick(x, y);
	}
}

void StageEditorController::mouseBtnDownToolbar(MouseBtn btn, int x, int y)
{
	if (btn == BTN_LEFT) {
		checkToolIconClick(x, y);
	}
}

void StageEditorController::mouseBtnDownWorkspace(MouseBtn btn, int x, int y)
{
	switch (btn) {
		case BTN_LEFT: {
			PointF pos = getMouseInStageSpace(x, y);
			ObjectSnap snapping = getSnapping();

			const auto lastAction = m_stageEditor.mouseLeftBtnDown(pos, snapping,
				sysProxy->isKeyPressed(KEY_SHIFT),
				getGrabZoneSizeByViewportZoom());
			updateSpritesByAction(lastAction);
		} break;
		case BTN_RIGHT: {
			const std::shared_ptr<StageEditorAction> lastAction = 
				m_stageEditor.mouseRightBtnDown();
			updateSpritesByAction(lastAction);
		} break;
		case BTN_MIDDLE:
			if (!m_viewport.isDrag()) {
				m_viewport.beginDrag(PointF(x, y));
			}
			break;
		case BTN_UNKNOWN: // Warning off
			break;
	}
}

void StageEditorController::mouseWheelWorkspace(int dx, int dy)
{
	(void)dx;

	if (sysProxy->isKeyPressed(KEY_CTRL) && dy != 0) {
		// Zoom
		
		Rect workspaceRect = getWorkspaceRect();
		Point mouse = sysProxy->getMousePos();
		Point mouseRel = mouse.relativeTo(workspaceRect.getTopLeft());
		if (dy > 0) {
			// Zoom in
			m_viewport.zoomIn(static_cast<PointF>(mouseRel),
				dy * StageViewport::DEFAULT_ZOOM_FACTOR);
		} else {
			// Zoom out
			m_viewport.zoomOut(static_cast<PointF>(mouseRel),
				-dy * StageViewport::DEFAULT_ZOOM_FACTOR);
		}
		updateSpritesByViewport();
	}
}

void StageEditorController::checkMenuIconMouseHover(int x, int y)
{
	Point mouse(x, y);
	Rect iconRect;

	for (int iconIdx = 0; iconIdx < MENUICON_COUNT; iconIdx++) {
		iconRect = getMenuIconRect(iconIdx);
		if (iconRect.containsPoint(mouse)) {
			iconHighlightOn(m_menuIcons[iconIdx]);
			break;
		}
	}
}

void StageEditorController::checkToolIconMouseHover(int x, int y)
{
	Point mouse(x, y);
	Rect iconRect;

	for (int iconIdx = 0; iconIdx < TOOLICON_COUNT; iconIdx++) {
		iconRect = getToolIconRect(iconIdx);
		if (iconRect.containsPoint(mouse)) {
			iconHighlightOn(m_toolIcons[iconIdx]);
			break;
		}
	}
}

void StageEditorController::checkWorkspaceDoDrag(int x, int y)
{
	if (m_viewport.isDrag()) {
		m_viewport.doDrag(PointF(x, y));
		updateSpritesByViewport();
	}
}

void StageEditorController::checkMenuIconClick(int x, int y)
{
	Point mouse(x, y);

	for (int iconIdx = 0; iconIdx < MENUICON_COUNT; iconIdx++) {
		if (getMenuIconRect(iconIdx).containsPoint(mouse)) {
			menuIconClick(iconIdx);
			break;
		}
	}
}

void StageEditorController::checkToolIconClick(int x, int y)
{
	Point mouse(x, y);

	for (int iconIdx = 0; iconIdx < TOOLICON_COUNT; iconIdx++) {
		if (getToolIconRect(iconIdx).containsPoint(mouse)) {
			const auto lastAction = m_stageEditor.toolLeftBtnDown(
				iconIdxToTool(iconIdx));
			updateSpritesByAction(lastAction);
			break;
		}
	}
}

void StageEditorController::menuIconClick(int iconIdx)
{
	switch (iconIdx) {
		case MENUICON_NEW_IDX:
			menuIconNewClick();
			break;
		case MENUICON_OPEN_IDX:
			menuIconOpenClick();
			break;
		case MENUICON_SAVE_IDX:
			menuIconSaveClick();
			break;
		case MENUICON_SAVE_AS_IDX:
			menuIconSaveAsClick();
			break;
		case MENUICON_UNDO_IDX:
			menuIconUndoClick();
			break;
		case MENUICON_REDO_IDX:
			menuIconRedoClick();
			break;
		case MENUICON_PROPERTIES_IDX:
			menuIconPropertiesClick();
			break;
		case MENUICON_BACK_IDX:
			menuIconBackClick();
			break;
	}
}

void StageEditorController::menuIconNewClick()
{
	// TODO
}

void StageEditorController::menuIconOpenClick()
{
	// TODO
}

void StageEditorController::menuIconSaveClick()
{
	// TODO
}

void StageEditorController::menuIconSaveAsClick()
{
	// TODO
}

void StageEditorController::menuIconUndoClick()
{
	if (m_stageEditor.canUndo()) {
		auto lastAction = m_stageEditor.undo();
		updateSpritesByAction(lastAction);
		updateUndoRedoIcons();
	}
}

void StageEditorController::menuIconRedoClick()
{
	if (m_stageEditor.canRedo()) {
		auto lastAction = m_stageEditor.redo();
		updateSpritesByAction(lastAction);
		updateUndoRedoIcons();
	}
}

void StageEditorController::menuIconPropertiesClick()
{
	// TODO
}

void StageEditorController::menuIconBackClick()
{
	finishedEvent();
}

void StageEditorController::iconHighlightOff(
	std::unique_ptr<EditorIconSprite>& icon)
{
	if (icon->getCostume() == EditorIconSprite::COSTUME_HOVER) {
		icon->setCostume(EditorIconSprite::COSTUME_NORMAL);
	}
}

void StageEditorController::iconHighlightOn(
	std::unique_ptr<EditorIconSprite>& icon)
{
	if (icon->getCostume() == EditorIconSprite::COSTUME_NORMAL) {
		icon->setCostume(EditorIconSprite::COSTUME_HOVER);
	}
}

void StageEditorController::iconHighlightOffAll()
{
	for (auto& icon : m_menuIcons) {
		iconHighlightOff(icon);
	}
	for (auto& icon : m_toolIcons) {
		iconHighlightOff(icon);
	}
}

void StageEditorController::toolIconSetSelected(int iconIdx)
{
	m_toolIcons[iconIdx]->setCostume(EditorIconSprite::COSTUME_SELECTED);
}

void StageEditorController::toolIconUnsetSelected(int iconIdx)
{
	m_toolIcons[iconIdx]->setCostume(EditorIconSprite::COSTUME_NORMAL);
}

void StageEditorController::menuIconSetEnabled(int iconIdx, bool value)
{
	if (!value) {
		m_menuIcons[iconIdx]->setCostume(EditorIconSprite::COSTUME_DISABLED);
	} else {
		Point mouse = sysProxy->getMousePos();
		m_menuIcons[iconIdx]->setCostume(EditorIconSprite::COSTUME_NORMAL);
		checkMenuIconMouseHover(mouse.x, mouse.y);
	}
}

void StageEditorController::hideBrush()
{
	if (m_brushSprite != nullptr) {
		if (m_brushSprite == m_playerBrush.get()) {
			// Repaint
			m_playerBrush->setPos(0, 0);
		}
		m_brushSprite = nullptr;
	}
}

void StageEditorController::updateSpritesByViewport()
{
	EditorOID oid;

	// Grid
	updateGridSprite();

	// Players
	for (auto& playerSpritePair : m_playerSprites) {
		oid = playerSpritePair.first;
		updatePlayerSprite(oid);
	}

	// Obstacles
	for (auto& obstacleSpritePair : m_obstacleSprites) {
		oid = obstacleSpritePair.first;
		updateObstacleSprite(oid);
	}

	// Open obstacle
	updateObstacleEdgesSprite();

	// Brush
	updateToolBrush();
}

void StageEditorController::updateUndoRedoIcons()
{
	menuIconSetEnabled(MENUICON_UNDO_IDX, m_stageEditor.canUndo());
	menuIconSetEnabled(MENUICON_REDO_IDX, m_stageEditor.canRedo());
}

void StageEditorController::updateSpritesByAction(
	const std::shared_ptr<StageEditorAction> action)
{
	switch (action->getType()) {
		case StageEditorAction::ACTION_NONE:
			break;
		case StageEditorAction::ACTION_MULTIPLE:
			updateSpritesByActionMultiple(action);
			break;
		case StageEditorAction::ACTION_ADD_PLAYER_OBJECT:
			updateSpritesByActionAddPlayerObject(action);
			break;
		case StageEditorAction::ACTION_ADD_OBSTACLE_OBJECT:
			updateSpritesByActionAddObstacleObject(action);
			break;
		case StageEditorAction::ACTION_PLACE_OBSTACLE_CORNER:
			updateSpritesByActionPlaceObstacleCorner(action);
			break;
		case StageEditorAction::ACTION_UNPLACE_OBSTACLE_CORNER:
			updateSpritesByActionUnplaceObstacleCorner(action);
			break;
		case StageEditorAction::ACTION_ACTIVATE_TOOL:
			updateSpritesByActionActivateTool(action);
			break;
		case StageEditorAction::ACTION_SELECT_PLAYER_OBJECT:
			updateSpritesByActionSelectPlayerObject(action);
			break;
		case StageEditorAction::ACTION_SELECT_OBSTACLE_OBJECT:
			updateSpritesByActionSelectObstacleObject(action);
			break;
		case StageEditorAction::ACTION_DESELECT_PLAYER_OBJECT:
			updateSpritesByActionDeselectPlayerObject(action);
			break;
		case StageEditorAction::ACTION_DESELECT_OBSTACLE_OBJECT:
			updateSpritesByActionDeselectObstacleObject(action);
			break;
		case StageEditorAction::ACTION_BEGIN_DRAG_SELECTED:
			updateSpritesByActionBeginDragSelected(action);
			break;
		case StageEditorAction::ACTION_MOVE_PLAYER_OBJECT:
			updateSpritesByActionMovePlayerObject(action);
			break;
		case StageEditorAction::ACTION_MOVE_OBSTACLE_OBJECT:
			updateSpritesByActionMoveObstacleObject(action);
			break;
		case StageEditorAction::ACTION_DELETE_PLAYER_OBJECT:
			updateSpritesByActionDeletePlayerObject(action);
			break;
		case StageEditorAction::ACTION_DELETE_OBSTACLE_OBJECT:
			updateSpritesByActionDeleteObstacleObject(action);
			break;
		case StageEditorAction::ACTION_BEGIN_DRAG_STAGE_CORNER:
			updateSpritesByActionBeginDragStageCorner(action);
			break;
		case StageEditorAction::ACTION_RESIZE_STAGE:
			updateSpritesByActionResizeStage(action);
			break;
	}

	updateUndoRedoIcons();
}

void StageEditorController::updateSpritesByActionMultiple(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionMultiple>(action);
	
	for (const auto& a : actionCast->getActions()) {
		updateSpritesByAction(a);
	}
}

void StageEditorController::updateSpritesByActionAddPlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionAddPlayerObject>(action);
	addPlayerSprite(actionCast->getPlayerObject().getOid());
}

void StageEditorController::updateSpritesByActionAddObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionAddObstacleObject>(action);
	addObstacleSprite(actionCast->getObstacleObject().getOid());
}

void StageEditorController::updateSpritesByActionPlaceObstacleCorner(
	const std::shared_ptr<StageEditorAction> action)
{
	(void)action;
	updateObstacleEdgesSprite();
	updateToolBrush();
}

void StageEditorController::updateSpritesByActionUnplaceObstacleCorner(
	const std::shared_ptr<StageEditorAction> action)
{
	(void)action;
	updateObstacleEdgesSprite();
	updateToolBrush();
}

void StageEditorController::updateSpritesByActionActivateTool(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionActivateTool>(action);
	toolIconUnsetSelected(toolToIconIdx(actionCast->getPrevTool()));
	toolIconSetSelected(toolToIconIdx(actionCast->getNewTool()));
	updateToolBrush();
}

void StageEditorController::updateSpritesByActionSelectPlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionSelectPlayerObject>(action);
	m_playerSprites[actionCast->getOid()]->setCostume(
		PlayerSprite::COSTUME_HIGHLIGHTED);
}

void StageEditorController::updateSpritesByActionSelectObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionSelectObstacleObject>(action);
	m_obstacleSprites[actionCast->getOid()]->setCostume(
		ObstacleSprite::COSTUME_HIGHLIGHTED);
}

void StageEditorController::updateSpritesByActionDeselectPlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeselectPlayerObject>(action);
	m_playerSprites[actionCast->getOid()]->setCostume(
		PlayerSprite::COSTUME_NORMAL);
}

void StageEditorController::updateSpritesByActionDeselectObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeselectObstacleObject>(action);
	m_obstacleSprites[actionCast->getOid()]->setCostume(
		ObstacleSprite::COSTUME_NORMAL);
}

void StageEditorController::updateSpritesByActionBeginDragSelected(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionBeginDragSelected>(action);
	
	for (EditorOID oid : actionCast->getPlayerOids()) {
		m_draggedPlayerSprites[oid] = std::move(m_playerSprites[oid]);
		m_playerSprites.erase(oid);
		m_draggedPlayerSprites[oid]->setColor(Color::ghost());
		m_draggedPlayerSprites[oid]->setCostume(PlayerSprite::COSTUME_NORMAL);
	}
	
	for (EditorOID oid : actionCast->getObstacleOids()) {
		m_draggedObstacleSprites[oid] = std::move(m_obstacleSprites[oid]);
		m_obstacleSprites.erase(oid);
		m_draggedObstacleSprites[oid]->setColor(Color::ghost());
		m_draggedObstacleSprites[oid]->setCostume(ObstacleSprite::COSTUME_NORMAL);
	}
}

void StageEditorController::updateSpritesByActionMovePlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionMovePlayerObject>(action);

	EditorOID oid = actionCast->getOid();

	m_playerSprites[oid] = std::move(m_draggedPlayerSprites[oid]);
	m_draggedPlayerSprites.erase(oid);
	m_playerSprites[oid]->setColor(Color::playerDefault());
	m_playerSprites[oid]->setCostume(PlayerSprite::COSTUME_HIGHLIGHTED);

	updatePlayerSprite(oid);
}

void StageEditorController::updateSpritesByActionMoveObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionMoveObstacleObject>(action);

	EditorOID oid = actionCast->getOid();

	m_obstacleSprites[oid] = std::move(m_draggedObstacleSprites[oid]);
	m_draggedObstacleSprites.erase(oid);
	m_obstacleSprites[oid]->setColor(Color::obstacle());
	m_obstacleSprites[oid]->setCostume(ObstacleSprite::COSTUME_HIGHLIGHTED);

	updateObstacleSprite(oid);
}

void StageEditorController::updateSpritesByActionDeletePlayerObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeletePlayerObject>(action);
	
	EditorOID oid = actionCast->getObject().getOid();

	m_playerSprites.erase(oid);
}

void StageEditorController::updateSpritesByActionDeleteObstacleObject(
	const std::shared_ptr<StageEditorAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<StageEditorActionDeleteObstacleObject>(action);
	
	EditorOID oid = actionCast->getObject().getOid();

	m_obstacleSprites.erase(oid);
}

void StageEditorController::updateSpritesByActionBeginDragStageCorner(
	const std::shared_ptr<StageEditorAction> action)
{
	(void)action;
	
	updateToolBrush();
}

void StageEditorController::updateSpritesByActionResizeStage(
	const std::shared_ptr<StageEditorAction> action)
{
	(void)action;

	updateGridSprite();
}

void StageEditorController::updateGridSprite()
{
	// Get the transformation matrix
	Matrix3x3 tm = getStageToScreenMatrix();

	bool isValid;
	// Get the grid bounds in stage space
	Rect gridRect(0, 0, getPredictedStageSize(isValid));
	// Project to screen space
	gridRect.transform(tm);

	// Calculate the size of spaces between horizontal/vertical lines.
	unsigned solidsFrequency;
	ObjectSnap snapping = getSnappingByViewportZoom(solidsFrequency);
	double lineSpacing = m_viewport.getZoom() * snapping;

	// Modify sprite
	m_gridSprite->setCostume(isValid
		? EditorWorkspaceGridSprite::COSTUME_NORMAL
		: EditorWorkspaceGridSprite::COSTUME_BAD);
	m_gridSprite->setPos(gridRect.x, gridRect.y);
	m_gridSprite->setSize(gridRect.getSize());
	m_gridSprite->setXSpacing(lineSpacing);
	m_gridSprite->setYSpacing(lineSpacing);
	m_gridSprite->setSolidsFrequency(solidsFrequency);
}

void StageEditorController::updatePlayerSprite(EditorOID oid)
{
	// Get the sprite
	auto& sprite = m_playerSprites[oid];

	int radius = getPlayerSpriteRadius();

	// Get the transformation matrix
	Matrix3x3 tm = getStageToScreenMatrix();
	// Update the matrix -- sprite position is not at the center, it's at its
	// top left corner
	tm *= Matrix3x3::createTranslation(-radius, -radius);

	// Get the player position in stage space
	PointF playerPos = m_stageEditor.getState().players.at(oid).pos;
	// Project to screen space
	playerPos.transform(tm);

	sprite->setPos(static_cast<Point>(playerPos));
	sprite->setRadius(radius);
}

void StageEditorController::updateObstacleEdgesSprite()
{
	// Get the open obstacle's corners
	auto corners = m_stageEditor.getObstacleCorners();

	// Get the transformation matrix
	Matrix3x3 tm = getStageToScreenMatrix();

	for (auto& corner : corners) {
		corner.transform(tm);
	}
	m_obstacleEdges->setCorners(corners);
}

void StageEditorController::updateObstacleSprite(EditorOID oid)
{
	// Get the sprite
	auto& sprite = m_obstacleSprites[oid];

	// Get the transformation matrix
	Matrix3x3 tm = getStageToScreenMatrix();

	// Get the obstacle in stage space
	PolygonF obstacleObject = m_stageEditor.getState().obstacles.at(oid).shape;
	// Project to screen space
	obstacleObject.transform(tm);

	// Modify sprite
	sprite->setShape(obstacleObject);
}

void StageEditorController::updateDraggedSprites()
{
	for (const auto& pair : m_draggedPlayerSprites) {
		updateDraggedPlayerSprite(pair.first);
	}
	for (const auto& pair : m_draggedObstacleSprites) {
		updateDraggedObstacleSprite(pair.first);
	}
}

void StageEditorController::updateDraggedPlayerSprite(EditorOID oid)
{
	// I am not able to find an abstraction good enough to prevent copy-pasting
	// between this method and `updateDraggedObstacleSprite()`. The
	// `updateToolBrush*()` methods seem to be fairly similar too...

	// Get the mouse position in stage space
	PointF pos = getMouseInStageSpace();
	// Get the mouse snapping
	ObjectSnap snapping = getSnapping();

	bool isSuccess;
	// Predict
	auto predictedAction = m_stageEditor.predictEndDragPlayerObject(oid, pos, snapping, isSuccess);

	if (predictedAction->getType() == StageEditorAction::ACTION_MOVE_PLAYER_OBJECT) {
		// Downcast
		auto predictedActionCast =
			std::dynamic_pointer_cast<StageEditorActionMovePlayerObject>(
				predictedAction);
		
		EditorOID oid = predictedActionCast->getOid();
		double dx = predictedActionCast->getDx();
		double dy = predictedActionCast->getDy();

		// Calculate the new position in stage space
		PointF newPos = m_stageEditor.getState().players.at(oid).pos;
		newPos.x += dx;
		newPos.y += dy;

		Matrix3x3 tmScreen = getStageToScreenMatrix();
		// Project the new position to screen space
		PointF newPosScreen = newPos.getTransformed(tmScreen);

		// Modify sprite
		const auto& draggedPlayer = m_draggedPlayerSprites.at(oid);
		draggedPlayer->setCenterPos(static_cast<Point>(newPosScreen));
		draggedPlayer->setRadius(getPlayerSpriteRadius());
		draggedPlayer->setColor(isSuccess ? Color::ghost() : Color::badGhost());
	}
}

void StageEditorController::updateDraggedObstacleSprite(EditorOID oid)
{
	// I am not able to find an abstraction good enough to prevent copy-pasting
	// between this method and `updateDraggedPlayerSprite()`. The
	// `updateToolBrush*()` methods seem to be fairly similar too...

	// Get the mouse position in stage space
	PointF pos = getMouseInStageSpace();
	// Get the mouse snapping
	ObjectSnap snapping = getSnapping();

	bool isSuccess;
	// Predict
	auto predictedAction = m_stageEditor.predictEndDragObstacleObject(oid, pos, snapping, isSuccess);

	if (predictedAction->getType() == StageEditorAction::ACTION_MOVE_OBSTACLE_OBJECT) {
		// Downcast
		auto predictedActionCast =
			std::dynamic_pointer_cast<StageEditorActionMoveObstacleObject>(
				predictedAction);
		
		EditorOID oid = predictedActionCast->getOid();
		double dx = predictedActionCast->getDx();
		double dy = predictedActionCast->getDy();

		// Calculate the new shape in stage space
		PolygonF newShape = m_stageEditor.getState().obstacles.at(oid).shape;
		for (auto& corner : newShape.corners) {
			corner.x += dx;
			corner.y += dy;
		}

		Matrix3x3 tmScreen = getStageToScreenMatrix();
		// Project the new shape to screen space
		PolygonF newShapeScreen = newShape.getTransformed(tmScreen);

		// Modify sprite
		const auto& draggedObstacle = m_draggedObstacleSprites.at(oid);
		draggedObstacle->setShape(std::move(newShapeScreen));
		draggedObstacle->setColor(isSuccess ? Color::ghost() : Color::badGhost());
	}
}

void StageEditorController::updateToolBrush()
{
	switch (m_stageEditor.getActiveTool()) {
		case TOOL_SELECT:
			updateToolBrushSelect();
			break;
		case TOOL_PLAYERS:
			updateToolBrushPlayers();
			break;
		case TOOL_OBSTACLES:
			updateToolBrushObstacles();
			break;
		case TOOL_DELETE:
			updateToolBrushDelete();
			break;
		case TOOL_RESIZE_STAGE:
			updateToolBrushResizeStage();
			break;
	}
}

void StageEditorController::updateToolBrushSelect()
{
	hideBrush();

	updateDraggedSprites();
}

void StageEditorController::updateToolBrushPlayers()
{
	Point mousePos = sysProxy->getMousePos();

	if (!getWorkspaceRect().containsPoint(mousePos)) {
		// Don't draw anything if the mouse does not move over workspace
		hideBrush();
	} else {
		// Set the brush sprite
		m_brushSprite = m_playerBrush.get();
		
		// Get the mouse position in stage space
		PointF pos = getMouseInStageSpace();
		// Get the mouse snapping
		ObjectSnap snapping = getSnapping();

		bool isSuccess;
		// Predict
		auto predictedAction = m_stageEditor.predictAddPlayer(pos, snapping,
			isSuccess);
		assert(predictedAction->getType() == StageEditorAction::ACTION_ADD_PLAYER_OBJECT);

		// Downcast
		auto predictedActionCast =
			std::dynamic_pointer_cast<StageEditorActionAddPlayerObject>(
				predictedAction);

		Matrix3x3 tmScreen = getStageToScreenMatrix();
		// Project to screen space
		PointF playerPosScreen = predictedActionCast->getPlayerObject().pos;
		playerPosScreen.transform(tmScreen);
		
		// Modify sprite
		m_playerBrush->setCenterPos(static_cast<Point>(playerPosScreen));
		m_playerBrush->setRadius(getPlayerSpriteRadius());
		m_playerBrush->setCostume(isSuccess ? PlayerBrushSprite::COSTUME_NORMAL
			: PlayerBrushSprite::COSTUME_BAD);
	}
}

void StageEditorController::updateToolBrushObstacles()
{
	Point mousePos = sysProxy->getMousePos();
		
	// Get the mouse position in stage space
	PointF pos = getMouseInStageSpace();
	// Get the mouse snapping
	ObjectSnap snapping = getSnapping();

	bool isSuccess;
	// Predict
	auto predictedAction = m_stageEditor.predictPlaceObstacleCorner(pos,
		snapping, isSuccess);
	assert(predictedAction->getType() == StageEditorAction::ACTION_PLACE_OBSTACLE_CORNER);
	
	// Downcast
	auto predictedActionCast =
		std::dynamic_pointer_cast<StageEditorActionPlaceObstacleCorner>(
			predictedAction);

	Matrix3x3 tmScreen = getStageToScreenMatrix();
	// Project to screen space
	PointF cornerPosScreen = predictedActionCast->getPos();
	cornerPosScreen.transform(tmScreen);

	if (m_stageEditor.getObstacleCorners().size() == 0) {
		if (!getWorkspaceRect().containsPoint(mousePos)) {
			// Don't draw anything if the mouse does not move over workspace

			hideBrush();
		} else {
			// Draw just a dot

			m_brushSprite = m_obstacleBrush.get();

			m_obstacleBrush->setCostume(ObstacleBrushSprite::COSTUME_DOT);
			m_obstacleBrush->setP0(static_cast<Point>(cornerPosScreen));
		}
	} else {
		// Draw a dashed edge

		m_brushSprite = m_obstacleBrush.get();

		Matrix3x3 tm = getStageToScreenMatrix();
		PointF p0 = m_stageEditor.getObstacleCorners().back();
		p0.transform(tm);

		m_obstacleBrush->setP0(static_cast<Point>(p0));
		m_obstacleBrush->setP1(static_cast<Point>(cornerPosScreen));
		m_obstacleBrush->setCostume(isSuccess
			? ObstacleBrushSprite::COSTUME_NORMAL
			: ObstacleBrushSprite::COSTUME_BAD);
	}
}

void StageEditorController::updateToolBrushDelete()
{
	hideBrush();
}

void StageEditorController::updateToolBrushResizeStage()
{
	hideBrush();

	updateGridSprite();
}

void StageEditorController::addPlayerSprite(EditorOID oid)
{
	auto newSprite = std::make_unique<PlayerSprite>(sysProxy);
	newSprite->setColor(Color::red());
	m_playerSprites[oid] = std::move(newSprite);
	updatePlayerSprite(oid);
}

void StageEditorController::addObstacleSprite(EditorOID oid)
{
	auto newSprite = std::make_unique<ObstacleSprite>(sysProxy);
	newSprite->setColor(Color::obstacle());
	m_obstacleSprites[oid] = std::move(newSprite);
	updateObstacleSprite(oid);
}

Matrix3x3 StageEditorController::getScreenToStageMatrix()
{
	Rect workspaceRect = getWorkspaceRect();

	// Relative to workspace
	Matrix3x3 res = Matrix3x3::createTranslation(-workspaceRect.x,
		-workspaceRect.y);
	// Project to stage space
	res *= m_viewport.getProjectionToStageMatrix();

	return res;
}

Matrix3x3 StageEditorController::getStageToScreenMatrix()
{
	Rect workspaceRect = getWorkspaceRect();

	// Project to workspace
	Matrix3x3 res = m_viewport.getProjectionToScreenMatrix();
	// Relative to screen
	res *= Matrix3x3::createTranslation(workspaceRect.x, workspaceRect.y);

	return res;
}

Rect StageEditorController::getMenubarRect()
{
	// Full width
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	// Fixed height
	res.h = MENUBAR_HEIGHT;
	return res;
}

Rect StageEditorController::getToolbarRect()
{
	// Full height
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	// Height limited by the menu bar
	res.y = TOOLBAR_Y;
	// Fixed width
	res.w = TOOLBAR_WIDTH;
	// Height limited by the status bar
	res.h -= (MENUBAR_HEIGHT + STATUSBAR_HEIGHT);
	return res;
}

Rect StageEditorController::getStatusbarRect()
{
	// Full width
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	// Fixed height (at the bottom of the screen)
	res.y = res.h - STATUSBAR_HEIGHT;
	res.h = STATUSBAR_HEIGHT;
	return res;
}

Rect StageEditorController::getWorkspaceRect()
{
	// Full size
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	// Width limited by the tool bar
	res.x = TOOLBAR_WIDTH;
	// Height limited by the menu bar
	res.y = MENUBAR_HEIGHT;
	// Width must not go beyond the screen bounds
	res.w -= res.x;
	// Height limited by menu bar and status bar
	res.h -= (res.y + STATUSBAR_HEIGHT);
	return res;
}

Rect StageEditorController::getMenuIconRect(int iconIdx)
{
	if (iconIdx != MENUICON_BACK_IDX) {
		// Get the menu bar position
		Rect menubarRect = getMenubarRect();
		Rect res(
			menubarRect.x + MENUICONS_LEFT_MARGIN, // Add left margin
			menubarRect.y + MENUICONS_TOP_MARGIN,  // Add top margin
			MENUICONS_WIDTH, // Set width
			MENUICONS_HEIGHT // Set height
		);
		// Skip preceding icons
		res.x += iconIdx * (MENUICONS_WIDTH + MENUICONS_SPACING);
		return res;
	} else {
		Rect menubarRect = getMenubarRect();
		Rect res(
			menubarRect.getRight() - MENUICONS_RIGHT_MARGIN - MENUICONS_WIDTH,
			menubarRect.y + MENUICONS_TOP_MARGIN,
			MENUICONS_WIDTH,
			MENUICONS_HEIGHT
		);
		return res;
	}
}

Rect StageEditorController::getToolIconRect(int iconIdx)
{
	// Get the tool bar position
	Rect toolbarRect = getToolbarRect();
	Rect res(
		toolbarRect.x + TOOLICONS_LEFT_MARGIN, // Add left margin
		toolbarRect.y + TOOLICONS_TOP_MARGIN,  // Add top margin
		MENUICONS_WIDTH, // Set width
		MENUICONS_HEIGHT // Set height
	);
	// Skip icons on the left
	res.x += (iconIdx % TOOLBAR_ITEM_COLUMNS) * (TOOLICONS_WIDTH + TOOLICONS_HORZ_SPACING);
	// Skip rows
	res.y += (iconIdx / TOOLBAR_ITEM_COLUMNS) * (TOOLICONS_HEIGHT + TOOLICONS_VERT_SPACING);
	return res;
}

int StageEditorController::toolToIconIdx(EditorTool tool)
{
	switch (tool) {
		case TOOL_SELECT: return TOOLICON_SELECT_IDX;
		case TOOL_PLAYERS: return TOOLICON_PLAYER_IDX;
		case TOOL_OBSTACLES: return TOOLICON_OBSTACLE_IDX;
		case TOOL_DELETE: return TOOLICON_TRASH_CAN_IDX;
		case TOOL_RESIZE_STAGE: return TOOLICON_RESIZE_IDX;
		// Don't place the `default` label, or else the compiler won't produce
		// a warning when there are some enum values missing
	}

	// Default
	return -1;
}

EditorTool StageEditorController::iconIdxToTool(int iconIdx)
{
	assert(0 <= iconIdx && iconIdx < TOOLICON_COUNT);

	switch (iconIdx) {
		case TOOLICON_SELECT_IDX: return TOOL_SELECT;
		case TOOLICON_PLAYER_IDX: return TOOL_PLAYERS;
		case TOOLICON_OBSTACLE_IDX: return TOOL_OBSTACLES;
		case TOOLICON_TRASH_CAN_IDX: return TOOL_DELETE;
		case TOOLICON_RESIZE_IDX: return TOOL_RESIZE_STAGE;
		// Inaccessible, but need to get rid of the warning
		default: return EditorTool(-1);
	}
}

ObjectSnap StageEditorController::getSnappingByViewportZoom()
{
	// /dev/null (we don't need the `solidsFrequency` value)
	unsigned devnull;
	return getSnappingByViewportZoom(devnull);
}

ObjectSnap StageEditorController::getSnappingByViewportZoom(unsigned& solidsFrequency)
{
	// FIXME: This algorithm surely needs some improvements...

	static constexpr double MIN_SPACING = 12.0;

	StageViewport::ZoomType zoom = m_viewport.getZoom();

	// Between two dashed lines there have to be at least 12 pixels.
	// line spacing = zoom * snapping
	// line spacing >= 12
	// zoom * snapping >= 12
	// snapping >= 12/zoom
	// zoom >= 12/snapping
	//
	// The snapping is one of
	// {x: x = s * 10^n; s.memberOf({1, 2, 5}); n.memberOf(N_0)}
	// That is e.g., 1, 2, 5, 10, 20, 50, ...

	double snap1 = 1, snap2 = 2, snap5 = 5;
	while (true) {
		if (zoom >= MIN_SPACING/snap1) {
			solidsFrequency = 10;
			return static_cast<ObjectSnap>(snap1);
		} else if (zoom >= MIN_SPACING/snap2) {
			solidsFrequency = 5;
			return static_cast<ObjectSnap>(snap2);
		} else if (zoom >= MIN_SPACING/snap5) {
			solidsFrequency = 2;
			return static_cast<ObjectSnap>(snap5);
		} else {
			snap1 *= 10.0;
			snap2 *= 10.0;
			snap5 *= 10.0;
		}
	}
}

ObjectSnap StageEditorController::getSnapping()
{
	return (sysProxy->isKeyPressed(KEY_ALT) ? OBJECT_SNAP_NONE
		: getSnappingByViewportZoom());
}

double StageEditorController::getGrabZoneSizeByViewportZoom()
{
	return (1.0 / m_viewport.getZoom()) * 10.0;
}

int StageEditorController::getPlayerSpriteRadius()
{
	return static_cast<int>(EDITOR_PLAYER_RADIUS * m_viewport.getZoom());
}

PointF StageEditorController::getMouseInStageSpace()
{
	return getMouseInStageSpace(sysProxy->getMousePos());
}

PointF StageEditorController::getMouseInStageSpace(const Point& mouse)
{
	Rect workspaceRect = getWorkspaceRect();
	PointF mouseViewport(
		mouse.x - workspaceRect.x,
		mouse.y - workspaceRect.y
	);
	
	PointF res = m_viewport.screenToStage(mouseViewport);
	return res;
}

PointF StageEditorController::getMouseInStageSpace(int x, int y)
{
	return getMouseInStageSpace(Point(x, y));
}

Size2d StageEditorController::getPredictedStageSize(bool& isValid)
{
	Size2d res = m_stageEditor.getState().getSize();
	isValid = true;

	if (m_stageEditor.isDraggingStageCorner()) {
		PointF pos = getMouseInStageSpace();
		ObjectSnap snapping = getSnapping();

		auto predictedAction = m_stageEditor.predictEndDragStageCorner(pos,
			snapping, isValid);
		if (predictedAction->getType() == StageEditorAction::ACTION_RESIZE_STAGE) {
			auto predictedActionCast =
				std::dynamic_pointer_cast<StageEditorActionResizeStage>(
					predictedAction);
			res.w += predictedActionCast->getResizeX();
			res.h += predictedActionCast->getResizeY();
		}
	}

	return res;
}

std::shared_ptr<IControllerChild> StageEditorController::createReplacement()
{
	return ControllerFactory::createMainMenuController(sysProxy);
}

void StageEditorController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	initializeSprites();
}

void StageEditorController::mouseBtnDownEvent(MouseBtn btn, int x, int y)
{
	Rect menubarRect = getMenubarRect();
	Rect toolbarRect = getToolbarRect();
	Rect statusbarRect = getStatusbarRect();
	Rect workspaceRect = getWorkspaceRect();
	Point mouse(x, y);

	if (menubarRect.containsPoint(mouse)) {
		// Within menubar
		mouseBtnDownMenubar(btn, x, y);
	} else if (toolbarRect.containsPoint(mouse)) {
		// Within toolbar
		mouseBtnDownToolbar(btn, x, y);
	} else if (statusbarRect.containsPoint(mouse)) {
		// Within status bar
		// Nothing
	} else if (workspaceRect.containsPoint(mouse)) {
		// Within workspace
		mouseBtnDownWorkspace(btn, x, y);
	}
}

void StageEditorController::mouseBtnUpEvent(MouseBtn btn, int x, int y)
{
	switch (btn) {
		case BTN_LEFT: {
			PointF pos = getMouseInStageSpace(x, y);
			ObjectSnap snapping = getSnapping();
			const auto lastAction = m_stageEditor.mouseLeftBtnUp(pos, snapping);
			updateSpritesByAction(lastAction);
		} break;
		case BTN_MIDDLE:
			m_viewport.endDrag();
			break;
		case BTN_RIGHT:
		case BTN_UNKNOWN:
			break;
	}
}

void StageEditorController::mouseMoveEvent(int x, int y)
{
	Rect menubarRect = getMenubarRect();
	Rect toolbarRect = getToolbarRect();
	Rect statusbarRect = getStatusbarRect();
	Rect workspaceRect = getWorkspaceRect();
	Point mouse(x, y);

	iconHighlightOffAll();

	if (menubarRect.containsPoint(mouse)) {
		// Within menubar
		mouseMoveMenubar(x, y);
	} else if (toolbarRect.containsPoint(mouse)) {
		// Within toolbar
		mouseMoveToolbar(x, y);
	} else if (statusbarRect.containsPoint(mouse)) {
		// Within status bar
		// Nothing
	} else if (workspaceRect.containsPoint(mouse)) {
		// Within workspace
		mouseMoveWorkspace(x, y);
	}

	updateToolBrush();
}

void StageEditorController::mouseWheelEvent(int dx, int dy)
{
	Rect workspaceRect = getWorkspaceRect();
	Point mouse = sysProxy->getMousePos();

	if (workspaceRect.containsPoint(mouse)) {
		mouseWheelWorkspace(dx, dy);
	}
}

void StageEditorController::paintEvent(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	// Grid
	m_gridSprite->repaint(canvas, invalidRect);

	// Open obstacle
	m_obstacleEdges->repaint(canvas, invalidRect);

	// Obstacles
	for (auto& obstacleSprite : m_obstacleSprites) {
		obstacleSprite.second->repaint(canvas, invalidRect);
	}

	// Players
	for (auto& playerSprite : m_playerSprites) {
		playerSprite.second->repaint(canvas, invalidRect);
	}

	// Dragged obstacles
	for (auto& obstacleSprite : m_draggedObstacleSprites) {
		obstacleSprite.second->repaint(canvas, invalidRect);
	}

	// Dragged players
	for (auto& playerSprite : m_draggedPlayerSprites) {
		playerSprite.second->repaint(canvas, invalidRect);
	}

	// Brush
	if (m_brushSprite != nullptr) {
		m_brushSprite->repaint(canvas, invalidRect);
	}

	// Menu
	m_menuBarSprite->repaint(canvas, invalidRect);
	for (auto& icon : m_menuIcons) {
		icon->repaint(canvas, invalidRect);
	}

	// Toolbar
	m_toolBarSprite->repaint(canvas, invalidRect);
	for (auto& icon : m_toolIcons) {
		icon->repaint(canvas, invalidRect);
	}

	// Status bar
	m_statusBarSprite->repaint(canvas, invalidRect);
	m_statusBarText->repaint(canvas, invalidRect);
}
