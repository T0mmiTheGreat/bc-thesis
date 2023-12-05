/**
 * @file InGameController.hpp
 * @author Tomáš Ludrovan
 * @brief InGameController class
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INGAMECONTROLLER_HPP
#define INGAMECONTROLLER_HPP

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"

class InGameController : public GeneralControllerBase {
private:
	int spriteX;
	int spriteY;
	int spriteR;

	Timer m_timer;
public:
	InGameController() : GeneralControllerBase(), spriteX{120}, spriteY{140}, spriteR{36}, m_timer(17) {}

	void startedEvent() override;
	void frameEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // INGAMECONTROLLER_HPP
