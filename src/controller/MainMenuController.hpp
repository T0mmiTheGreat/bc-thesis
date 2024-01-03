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

#include <memory>

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "sprite/MainMenuSprite.hpp"
#include "sprite/MainMenuTitleSprite.hpp"

class MainMenuController : public GeneralControllerBase {
private:
	std::unique_ptr<MainMenuSprite> m_mainMenu;
	std::unique_ptr<MainMenuTitleSprite> m_title;
public:
	void startedEvent() override;
	void mouseMoveEvent(int x, int y) override;
	void loopEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // MAINMENUCONTROLLER_HPP
