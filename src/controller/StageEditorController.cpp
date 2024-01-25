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
	// Create icons
	m_icons[MENUICON_NEW_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_NEW);
	m_icons[MENUICON_OPEN_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_OPEN);
	m_icons[MENUICON_SAVE_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_SAVE);
	m_icons[MENUICON_SAVE_AS_IDX] = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_SAVE_AS);

	int iconX = MENUICONS_LEFT_MARGIN;
	// Position icons
	for (auto& icon : m_icons) {
		icon->setPos(iconX, MENUICONS_TOP_MARGIN);
		iconX += MENUICONS_WIDTH + MENUICONS_SPACING;
	}

	// Create menubar line
	m_menuBarLine = std::make_unique<HorizontalLineSprite>(sysProxy);
	// Position
	m_menuBarLine->setPos(0, MENUBAR_HEIGHT);
}

void StageEditorController::startedEvent()
{
	createSprites();
}

void StageEditorController::paintEvent(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	for (auto& icon : m_icons) {
		icon->repaint(canvas, invalidRect);
	}

	m_menuBarLine->repaint(canvas, invalidRect);
}
