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
#include "stageserializer/IStageSerializer.hpp"

class MainMenuController : public GeneralControllerBase {
private:
	enum ExitResult {
		RES_STAGE_SELECT,
		RES_PLAY,
		RES_EDITOR,
	};
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
	std::unique_ptr<MainMenuTitleSprite> m_title;

	ExitResult m_exitResult;

	// Used when selecting a stage for playing (passed as parameters to
	// StageSelectController constructor).

	IStageSerializer::IdType m_selectedStage;
	bool m_isSelectedStageValid;

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
	std::shared_ptr<IControllerChild> createReplacement() override;
	
	void onStarted() override;
	void onResumed() override;
	void onMouseBtnDown(MouseBtn btn, int x, int y) override;
	void onMouseMove(int x, int y) override;
	void onLoop() override;
	void onPaint(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	MainMenuController(std::shared_ptr<ISysProxy> sysProxy);
};

#endif // MAINMENUCONTROLLER_HPP
