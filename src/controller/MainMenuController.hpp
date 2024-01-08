/**
 * @file MainMenuController.hpp
 * @author Tomáš Ludrovan
 * @brief MainMenuController class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAINMENUCONTROLLER_HPP
#define MAINMENUCONTROLLER_HPP

#include <array>
#include <memory>

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "sprite/MainMenuItemSprite.hpp"
#include "sprite/MainMenuTitleSprite.hpp"

class MainMenuController : public GeneralControllerBase {
private:
	static constexpr int MENU_ITEM_SPACING = 10;
	static constexpr int MENU_TOP = 230;

	static constexpr int MENU_PLAY_BTN_IDX = 0;
	static constexpr int MENU_QUIT_BTN_IDX = MENU_PLAY_BTN_IDX + 1;
	static constexpr int MENU_ITEM_COUNT = MENU_QUIT_BTN_IDX + 1;

	std::array<std::unique_ptr<MainMenuItemSprite>, MENU_ITEM_COUNT> m_menuBtns;
	std::unique_ptr<MainMenuTitleSprite> m_title;
public:
	MainMenuController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void mouseMoveEvent(int x, int y) override;
	void loopEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // MAINMENUCONTROLLER_HPP
