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

#include "types.hpp"
#include "controller/ControllerFactory.hpp"
#include "playerinput/PlayerInputFactory.hpp"
#include "stageserializer/StageSerializerFactory.hpp"

MainMenuController::MainMenuController(std::shared_ptr<ISysProxy> sysProxy)
	: GeneralControllerBase(sysProxy)
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

const std::shared_ptr<IStageSerializer>
MainMenuController::getStageForGame() const
{
	assert(m_isSelectedStageValid);

	auto res = StageSerializerFactory::createDefault();
	res->load(m_selectedStage);
	return res;
}

const std::vector<std::shared_ptr<IPlayerInput>>
MainMenuController::getPlayersForGame() const
{
	// FIXME
	std::vector<std::shared_ptr<IPlayerInput>> res;
	res.push_back(PlayerInputFactory::createKeyboardPlayerInputWSAD(sysProxy));
	res.push_back(PlayerInputFactory::createKeyboardPlayerInputIKJL(sysProxy));
	res.push_back(PlayerInputFactory::createKeyboardPlayerInputArrows(sysProxy));
	return res;
}

std::shared_ptr<IControllerChild> MainMenuController::createReplacement()
{
	switch (m_exitResult) {
		case RES_STAGE_SELECT:
			return ControllerFactory::createStageSelectController(sysProxy,
				m_selectedStage, m_isSelectedStageValid);
		case RES_PLAY:
			return ControllerFactory::createInGameController(sysProxy,
				getStageForGame(), getPlayersForGame());
		case RES_EDITOR:
			return ControllerFactory::createStageEditorController(sysProxy);
	}

	// Default
	return nullptr;
}

void MainMenuController::onStarted()
{
	GeneralControllerBase::onStarted();
	createSprites();
}

void MainMenuController::onResumed()
{
	GeneralControllerBase::onResumed();
	if (m_exitResult == RES_STAGE_SELECT) {
		if (m_isSelectedStageValid) {
			m_exitResult = RES_PLAY;
			finishedEvent();
		}
	}
}

void MainMenuController::onMouseBtnDown(MouseBtn btn, int x, int y)
{
	if (btn != BTN_LEFT) return;

	int menuItemIdx = getMenuItemAt(x, y);

	switch (menuItemIdx) {
		case MENU_PLAY_BTN_IDX:
			m_exitResult = RES_STAGE_SELECT;
			pausedEvent();
			break;
		case MENU_STAGE_EDITOR_BTN_IDX:
			m_exitResult = RES_EDITOR;
			finishedEvent();
			break;
		case MENU_QUIT_BTN_IDX:
			sysProxy->quit();
			break;
	}
}

void MainMenuController::onMouseMove(int x, int y)
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

void MainMenuController::onLoop()
{
	GeneralControllerBase::onLoop();
}

void MainMenuController::onPaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	GeneralControllerBase::onPaint(canvas, invalidRect);
	for (auto& btn : m_menuBtns) {
		btn->repaint(canvas, invalidRect);
	}
	m_title->repaint(canvas, invalidRect);
}
