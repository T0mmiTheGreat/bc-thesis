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

#include "stageeditor/Common.hpp"

StageEditorController::StageEditorController(
	std::shared_ptr<ISysProxy> sysProxy)
	: GeneralControllerBase(sysProxy)
	, m_viewport(static_cast<Size2dF>(m_stageEditor.getState().getSize()),
		static_cast<Size2dF>(getWorkspaceRect().getSize()))
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

	// Menubar line
	m_menuBarSprite = std::make_unique<OptionBarSprite>(sysProxy);

	// Tool icons
	m_toolIcons[TOOLICON_SELECT_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_SELECT_TOOL);
	m_toolIcons[TOOLICON_PLAYER_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_PLAYER_TOOL);
	m_toolIcons[TOOLICON_OBSTACLE_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_OBSTACLE_TOOL);

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

	positionSprites();
	updateSpritesByViewport();
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

Rect StageEditorController::getMenuIconRect(int iconIdx)
{
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
	// Note: currently there is only one row (3 columns, 3 icons). If this
	// changes in the future, this note should be deleted
	res.y += (iconIdx / TOOLBAR_ITEM_COLUMNS) * (TOOLICONS_HEIGHT + TOOLICONS_VERT_SPACING);
	return res;
}

void StageEditorController::mouseMoveMenubar(int x, int y)
{
	checkMenuIconMouseHover(x, y);
}

void StageEditorController::mouseMoveToolbar(int x, int y)
{
	checkToolIconMouseHover(x, y);
}

void StageEditorController::mouseMoveStatusbar(int x, int y)
{
}

void StageEditorController::mouseMoveWorkspace(int x, int y)
{
	checkWorkspaceDoDrag(x, y);
}

void StageEditorController::mouseBtnDownMenubar(MouseBtn btn, int x, int y)
{
}

void StageEditorController::mouseBtnDownToolbar(MouseBtn btn, int x, int y)
{
}

void StageEditorController::mouseBtnDownStatusbar(MouseBtn btn, int x, int y)
{
}

void StageEditorController::mouseBtnDownWorkspace(MouseBtn btn, int x, int y)
{
	switch (m_activeTool) {
		case TOOL_SELECT:
			mouseBtnDownWorkspaceToolSelect(btn, x, y);
			break;
		case TOOL_PLAYERS:
			mouseBtnDownWorkspaceToolPlayers(btn, x, y);
			break;
		case TOOL_OBSTACLES:
			mouseBtnDownWorkspaceToolObstacles(btn, x, y);
			break;
	}

	if (btn == BTN_MIDDLE) {
		if (!m_viewport.isDrag()) {
			m_viewport.beginDrag(PointF(x, y));
		}
	}
}

void StageEditorController::mouseBtnDownWorkspaceToolSelect(MouseBtn btn,
	int x, int y)
{
}

void StageEditorController::mouseBtnDownWorkspaceToolPlayers(MouseBtn btn,
	int x, int y)
{
	if (btn == BTN_LEFT) {
		addPlayerObject(x, y);
	}
}

void StageEditorController::mouseBtnDownWorkspaceToolObstacles(MouseBtn btn,
	int x, int y)
{
	if (btn == BTN_LEFT) {
		addObstacleCorner(x, y);
	} else if (btn == BTN_RIGHT) {
		completeObstacleObject();
	}
}

void StageEditorController::mouseWheelWorkspace(int dx, int dy)
{
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

void StageEditorController::addPlayerObject(int x, int y)
{
	// Get the transformation matrix
	Matrix3x3 tm = getScreenToStageMatrix();

	// Player position in screen space
	PointF playerPos(x, y);
	// Project to stage space
	playerPos.transform(tm);
	
	m_stageEditor.addPlayer(playerPos, StageEditor::SNAP_NONE);
	
	updateSpritesByBackend();
}

void StageEditorController::addObstacleCorner(int x, int y)
{
	// Get the transformation matrix
	Matrix3x3 tm = getScreenToStageMatrix();

	// Corner position in screen space
	PointF cornerPos(x, y);
	// Project to stage space
	cornerPos.transform(tm);

	m_stageEditor.addObstacleCorner(cornerPos, StageEditor::SNAP_NONE);

	updateSpritesByBackend();
}

void StageEditorController::completeObstacleObject()
{
	// Complete in backend
	m_stageEditor.completeObstacle();

	// Get rid of the open obstacle (it is now closed)
	m_obstacleEdges->clearCorners();

	updateSpritesByBackend();
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

void StageEditorController::updateSpritesByBackend()
{
	const auto lastAction = m_stageEditor.getLastAction();

	switch (lastAction->getType()) {

		case StageEditorAction::ACTION_ADD_PLAYER: {
			const auto actionAddPlayer =
				std::dynamic_pointer_cast<StageEditorActionAddPlayer>(
					lastAction);
			addPlayerSprite(actionAddPlayer->getOid());
		} break;

		case StageEditorAction::ACTION_PLACE_OBSTACLE_CORNER: {
			updateObstacleEdgesSprite();
		} break;

		case StageEditorAction::ACTION_COMPLETE_OBSTACLE: {
			const auto actionCompleteObstacle =
				std::dynamic_pointer_cast<StageEditorActionCompleteObstacle>(
					lastAction);
			addObstacleSprite(actionCompleteObstacle->getOid());
		} break;

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
}

void StageEditorController::updateGridSprite()
{
	// Get the transformation matrix
	Matrix3x3 tm = getStageToScreenMatrix();

	// Get the grid bounds in stage space
	Rect gridRect(0, 0, m_stageEditor.getState().getSize());
	// Project to screen space
	gridRect.transform(tm);

	// Calculate the size of spaces between horizontal/vertical lines.
	double lineSpacing = m_viewport.getZoom() * 100.0; // FIXME

	// Modify sprite
	m_gridSprite->setPos(gridRect.x, gridRect.y);
	m_gridSprite->setSize(gridRect.getSize());
	m_gridSprite->setXSpacing(lineSpacing);
	m_gridSprite->setYSpacing(lineSpacing);
}

void StageEditorController::updatePlayerSprite(EditorOID oid)
{
	// Get the sprite
	auto& sprite = m_playerSprites[oid];

	// Get the player sprite radius
	double radius = EDITOR_PLAYER_RADIUS * m_viewport.getZoom();

	// Get the transformation matrix
	Matrix3x3 tm = getStageToScreenMatrix();
	// Update the matrix -- sprite position is not at the center, it's at its
	// top left corner
	tm *= Matrix3x3::createTranslation(-radius, -radius);

	// Get the player position in stage space
	PointF playerPos = m_stageEditor.getState().players.at(oid);
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
	PolygonF obstacleObject = m_stageEditor.getState().obstacles.at(oid);
	// Project to screen space
	obstacleObject.transform(tm);

	// Modify sprite
	sprite->setShape(obstacleObject);
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
	// Height limited by the status bar
	res.h -= (res.y - STATUSBAR_HEIGHT);
	return res;
}

void StageEditorController::startedEvent()
{
	createSprites();

	m_activeTool = TOOL_OBSTACLES;
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
		mouseBtnDownStatusbar(btn, x, y);
	} else if (workspaceRect.containsPoint(mouse)) {
		// Within workspace
		mouseBtnDownWorkspace(btn, x, y);
	}
}

void StageEditorController::mouseBtnUpEvent(MouseBtn btn, int x, int y)
{
	if (btn == BTN_MIDDLE) {
		m_viewport.endDrag();
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
		mouseMoveStatusbar(x, y);
	} else if (workspaceRect.containsPoint(mouse)) {
		// Within workspace
		mouseMoveWorkspace(x, y);
	}
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
	Rect& invalidRect)
{
	// Grid
	m_gridSprite->repaint(canvas, invalidRect);

	// Open obstacle
	m_obstacleEdges->repaint(canvas, invalidRect);

	// Obstacles
	for (auto& obstacleSprite : m_obstacleSprites) {
		obstacleSprite.second->repaint(canvas, invalidRect);
	}

	// Player objects
	for (auto& playerSprite : m_playerSprites) {
		playerSprite.second->repaint(canvas, invalidRect);
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
