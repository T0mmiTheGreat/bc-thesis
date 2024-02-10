/**
 * @file MainMenuController.cpp
 * @author Tomáš Ludrovan
 * @brief MainMenuController class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/MainMenuController.hpp"

#include <format>

#include "types.hpp"
#include "controller/ControllerFactory.hpp"

MainMenuController::MainMenuController(std::shared_ptr<ISysProxy> sysProxy)
	: GeneralControllerBase(sysProxy)
	, m_selectedItem{-1}
{
}

void MainMenuController::createSprites()
{
	Size2d paintareaSize = sysProxy->getPaintAreaSize();

	// Title
	
	m_title = std::make_unique<MainMenuTitleSprite>(sysProxy);
	Size2d titleSize = m_title->getSize();
	m_title->setPos((paintareaSize.w - titleSize.w) / 2, 70);


	// Menu btns

	// Create
	for (auto& btn : m_menuBtns) {
		btn = std::make_unique<MainMenuItemSprite>(sysProxy);
	}

	// Set texts
	m_menuBtns[MENU_PLAY_BTN_IDX]->setText("Play");
	m_menuBtns[MENU_STAGE_EDITOR_BTN_IDX]->setText("Stage editor");
	m_menuBtns[MENU_QUIT_BTN_IDX]->setText("Quit");

	// Position
	int itemY = MENU_TOP;
	for (auto& btn : m_menuBtns) {
		int btnW = btn->getW();
		btn->setPos((paintareaSize.w - btnW) / 2, itemY);
		itemY += btn->getH() + MENU_ITEM_SPACING;
	}
}

int MainMenuController::getMenuItemAt(int x, int y)
{
	// Sprite bounds rectangle
	Rect sprBounds;
	// The position passed as `x` and `y` parameters
	Point pos(x, y);

	for (size_t i = 0; i < m_menuBtns.size(); i++) {
		sprBounds = m_menuBtns[i]->getBounds();
		if (sprBounds.containsPoint(pos)) {
			// Position within a menu item
			return i;
		}
	}

	// No menu item at the given position
	return -1;
}

std::shared_ptr<IControllerChild> MainMenuController::createReplacement()
{
	switch (m_selectedItem) {
		case MENU_PLAY_BTN_IDX:
			return ControllerFactory::createInGameController(sysProxy);
		case MENU_STAGE_EDITOR_BTN_IDX:
			return ControllerFactory::createStageEditorController(sysProxy);
		default:
			return nullptr;
	}
}

void MainMenuController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	createSprites();
}

void MainMenuController::mouseBtnDownEvent(MouseBtn btn, int x, int y)
{
	if (btn != BTN_LEFT) return;

	int menuItemIdx = getMenuItemAt(x, y);
	
	m_selectedItem = menuItemIdx;

	switch (menuItemIdx) {
		case MENU_PLAY_BTN_IDX:
		case MENU_STAGE_EDITOR_BTN_IDX:
			finishedEvent();
			break;
		case MENU_QUIT_BTN_IDX:
			sysProxy->quit();
			break;
	}
}

void MainMenuController::mouseMoveEvent(int x, int y)
{
	Point mousePt(x, y);

	for (auto& btn : m_menuBtns) {
		if (btn->getBounds().containsPoint(mousePt)) {
			// Mouse cursor over menu item - highlight
			btn->setCostume(MainMenuItemSprite::COSTUME_HOVER);
		} else {
			// Don't highlight
			btn->setCostume(MainMenuItemSprite::COSTUME_NORMAL);
		}
	}
}

void MainMenuController::loopEvent()
{
	GeneralControllerBase::loopEvent();
}

void MainMenuController::paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	for (auto& btn : m_menuBtns) {
		btn->repaint(canvas, invalidRect);
	}
	m_title->repaint(canvas, invalidRect);
}
