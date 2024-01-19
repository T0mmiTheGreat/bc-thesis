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
	// Space between menu items
	static constexpr int MENU_ITEM_SPACING = 10;
	// Y position of the first menu item
	static constexpr int MENU_TOP = 230;

	// Indices of the buttons in the buttons array (and the length of the array)
	static constexpr int MENU_PLAY_BTN_IDX = 0;
	static constexpr int MENU_STAGE_EDITOR_BTN_IDX = MENU_PLAY_BTN_IDX + 1;
	static constexpr int MENU_QUIT_BTN_IDX = MENU_STAGE_EDITOR_BTN_IDX + 1;
	static constexpr int MENU_ITEM_COUNT = MENU_QUIT_BTN_IDX + 1;

	// Sprites
	std::array<std::unique_ptr<MainMenuItemSprite>, MENU_ITEM_COUNT> m_menuBtns;
	// The menu item clicked or -1 if none was clicked
	int m_selectedItem;
	std::unique_ptr<MainMenuTitleSprite> m_title;

	void createSprites();
	/**
	 * @brief Returns the index of menu item at the given position.
	 * 
	 * @param x 
	 * @param y 
	 * @return Index to `m_menuBtns` or -1 if no item is at the given position.
	 */
	int getMenuItemAt(int x, int y);
protected:
	std::unique_ptr<IControllerChild> createReplacement() override;
public:
	MainMenuController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void mouseBtnDownEvent(MouseBtn btn, int x, int y) override;
	void mouseMoveEvent(int x, int y) override;
	void loopEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // MAINMENUCONTROLLER_HPP
