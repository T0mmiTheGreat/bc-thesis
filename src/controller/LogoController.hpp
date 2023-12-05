/**
 * @file LogoController.hpp
 * @author Tomáš Ludrovan
 * @brief LogoController class
 * @version 0.1
 * @date 2023-12-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LOGOCONTROLLER_HPP
#define LOGOCONTROLLER_HPP

#include "types.hpp"

#include "controller/GeneralControllerBase.hpp"

class LogoController : public GeneralControllerBase {
private:
	Timer m_timer;
	uint8_t m_opacity;
	int m_frameNumber;
public:
	LogoController() :
		GeneralControllerBase(),
		m_timer(17)
	{}

	virtual void startedEvent() override;
	virtual void frameEvent() override;
	virtual void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // LOGOCONTROLLER_HPP
