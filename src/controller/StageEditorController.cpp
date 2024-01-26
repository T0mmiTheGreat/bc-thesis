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

StageEditorController::StageEditorController(
	std::shared_ptr<ISysProxy> sysProxy)
	: GeneralControllerBase(sysProxy)
{}

void StageEditorController::createSprites()
{
	// Menu icons
	m_menuIcons[MENUICON_NEW_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_NEW);
	m_menuIcons[MENUICON_OPEN_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_OPEN);
	m_menuIcons[MENUICON_SAVE_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_SAVE);
	m_menuIcons[MENUICON_SAVE_AS_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_SAVE_AS);
	m_menuIcons[MENUICON_UNDO_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_UNDO);
	m_menuIcons[MENUICON_REDO_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_REDO);

	// Menubar line
	m_menuBarLine = std::make_unique<HorizontalLineSprite>(sysProxy);

	// Tool icons
	m_toolIcons[TOOLICON_SELECT_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_SELECT_TOOL);
	m_toolIcons[TOOLICON_PLAYER_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_PLAYER_TOOL);
	m_toolIcons[TOOLICON_OBSTACLE_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_OBSTACLE_TOOL);

	// Toolbar line
	m_toolBarLine = std::make_unique<VerticalLineSprite>(sysProxy);

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
}

Rect StageEditorController::getMenubarRect()
{
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	res.h = MENUBAR_HEIGHT;
	return res;
}

Rect StageEditorController::getToolbarRect()
{
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	res.y = TOOLBAR_Y;
	res.w = TOOLBAR_WIDTH;
	res.h -= (MENUBAR_HEIGHT + STATUSBAR_HEIGHT);
	return res;
}

Rect StageEditorController::getStatusbarRect()
{
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	res.y = res.h - STATUSBAR_HEIGHT;
	res.h = STATUSBAR_HEIGHT;
	return res;
}

Rect StageEditorController::getDesktopRect()
{
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	res.x = TOOLBAR_WIDTH;
	res.y = MENUBAR_HEIGHT;
	res.w -= res.x;
	res.h -= (res.y - STATUSBAR_HEIGHT);
	return res;
}

Rect StageEditorController::getMenuIconRect(int iconIdx)
{
	Rect menubarRect = getMenubarRect();
	Rect res(
		menubarRect.x + MENUICONS_LEFT_MARGIN,
		menubarRect.y + MENUICONS_TOP_MARGIN,
		MENUICONS_WIDTH,
		MENUICONS_HEIGHT
	);
	res.x += iconIdx * (MENUICONS_WIDTH + MENUICONS_SPACING);
	return res;
}

Rect StageEditorController::getToolIconRect(int iconIdx)
{
	Rect toolbarRect = getToolbarRect();
	Rect res(
		toolbarRect.x + TOOLICONS_LEFT_MARGIN,
		toolbarRect.y + TOOLICONS_TOP_MARGIN,
		MENUICONS_WIDTH,
		MENUICONS_HEIGHT
	);
	res.x += (iconIdx % TOOLBAR_ITEM_COLUMNS) * (TOOLICONS_WIDTH + TOOLICONS_HORZ_SPACING);
	res.y += (iconIdx / TOOLBAR_ITEM_COLUMNS) * (TOOLICONS_HEIGHT + TOOLICONS_VERT_SPACING);
	return res;
}

void StageEditorController::startedEvent()
{
	createSprites();
}

void StageEditorController::paintEvent(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	for (auto& icon : m_menuIcons) {
		icon->repaint(canvas, invalidRect);
	}

	m_menuBarLine->repaint(canvas, invalidRect);

	for (auto& icon : m_toolIcons) {
		icon->repaint(canvas, invalidRect);
	}

	m_toolBarLine->repaint(canvas, invalidRect);
}
