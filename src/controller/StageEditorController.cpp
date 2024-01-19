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
	auto iconNew = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_NEW);
	auto iconOpen = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_OPEN);
	auto iconSave = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_SAVE);
	auto iconSaveAs = std::make_unique<ImageSprite>(sysProxy, IMG_ICON_SAVE_AS);

	iconNew->setPos(10, 10);
	iconOpen->setPos(50, 10);
	iconSave->setPos(90, 10);
	iconSaveAs->setPos(130, 10);

	m_icons[ICON_NEW_IDX] = std::move(iconNew);
	m_icons[ICON_OPEN_IDX] = std::move(iconOpen);
	m_icons[ICON_SAVE_IDX] = std::move(iconSave);
	m_icons[ICON_SAVE_AS_IDX] = std::move(iconSaveAs);
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
}
