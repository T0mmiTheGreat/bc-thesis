/**
 * @file InitialController.hpp
 * @author Tomáš Ludrovan
 * @brief InitialController class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INITIALCONTROLLER_HPP
#define INITIALCONTROLLER_HPP

#include <memory>

#include "controller/GeneralControllerBase.hpp"

class InitialController : public GeneralControllerBase {
private:
	int spriteX;
	int spriteY;
	int spriteR;
public:
	InitialController() : GeneralControllerBase(), spriteX{120}, spriteY{140}, spriteR{36} {}

	std::unique_ptr<IController> runController() override;

	void keyDownEvent(KeyCode key) override;
	void startEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // INITIALCONTROLLER_HPP
