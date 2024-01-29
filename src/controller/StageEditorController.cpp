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
	m_menuBarLine = std::make_unique<HorizontalLineSprite>(sysProxy);

	// Tool icons
	m_toolIcons[TOOLICON_SELECT_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_SELECT_TOOL);
	m_toolIcons[TOOLICON_PLAYER_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_PLAYER_TOOL);
	m_toolIcons[TOOLICON_OBSTACLE_IDX] = std::make_unique<EditorIconSprite>(sysProxy, IMG_ICON_OBSTACLE_TOOL);

	// Toolbar line
	m_toolBarLine = std::make_unique<VerticalLineSprite>(sysProxy);

	// Statusbar text
	m_statusBarText = std::make_unique<StatusbarTextSprite>(sysProxy);
	// Statusbar line
	m_statusBarLine = std::make_unique<HorizontalLineSprite>(sysProxy);

	positionSprites();
}

void StageEditorController::positionSprites()
{
	// Menu icons
	for (int iconIdx = 0; iconIdx < MENUICON_COUNT; iconIdx++) {
		m_menuIcons[iconIdx]->setPos(getMenuIconRect(iconIdx).getTopLeft());
	}
	// Menubar line
	m_menuBarLine->setPos(0, MENUBAR_HEIGHT);

	// Tool icons
	for (int iconIdx = 0; iconIdx < TOOLICON_COUNT; iconIdx++) {
		m_toolIcons[iconIdx]->setPos(getToolIconRect(iconIdx).getTopLeft());
	}
	// Toolbar line
	m_toolBarLine->setPos(TOOLBAR_WIDTH, TOOLBAR_Y);
	m_toolBarLine->setBottomMargin(STATUSBAR_HEIGHT);

	Rect statusbarRect = getStatusbarRect();
	// Statusbar text
	m_statusBarText->setPos(
		statusbarRect.x + STATUSBAR_TEXT_LEFT_MARGIN,
		statusbarRect.y + ((statusbarRect.h - m_statusBarText->getH()) / 2)
	);
	// Statusbar line
	m_statusBarLine->setPos(statusbarRect.getTopLeft());
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

void StageEditorController::mouseMoveMenubar(int x, int y)
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

void StageEditorController::mouseMoveToolbar(int x, int y)
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

void StageEditorController::mouseMoveStatusbar(int x, int y)
{
}

void StageEditorController::mouseMoveWorkspace(int x, int y)
{
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
	if (btn == BTN_LEFT) {
		m_stageEditor.click(x, y, StageEditor::SNAP_NONE);
		updateSprites();
	}
}

void StageEditorController::updateSprites()
{
	const auto lastAction = m_stageEditor.getLastAction();

	switch (lastAction->type) {
		case StageEditorAction::ACTION_ADD_PLAYER:
			addPlayerSprite(lastAction->addPlayer.x, lastAction->addPlayer.y);
			break;
	}
}

void StageEditorController::addPlayerSprite(double x, double y, EditorOID oid)
{
	auto newSprite = std::make_unique<PlayerSprite>(sysProxy);
	newSprite->setPos(static_cast<int>(x) - EDITOR_PLAYER_RADIUS,
		static_cast<int>(y) - EDITOR_PLAYER_RADIUS);
	newSprite->setRadius(EDITOR_PLAYER_RADIUS);
	newSprite->setColor(Color::red());
	m_playerSprites[oid] = std::move(newSprite);
}

void StageEditorController::startedEvent()
{
	createSprites();

	m_stageEditor.setActiveTool(StageEditor::TOOL_PLAYERS);
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

void StageEditorController::paintEvent(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	// Player objects
	for (auto& playerSprite : m_playerSprites) {
		playerSprite.second->repaint(canvas, invalidRect);
	}

	// Menu
	for (auto& icon : m_menuIcons) {
		icon->repaint(canvas, invalidRect);
	}
	m_menuBarLine->repaint(canvas, invalidRect);

	// Toolbar
	for (auto& icon : m_toolIcons) {
		icon->repaint(canvas, invalidRect);
	}
	m_toolBarLine->repaint(canvas, invalidRect);

	// Status bar
	m_statusBarText->repaint(canvas, invalidRect);
	m_statusBarLine->repaint(canvas, invalidRect);
}
