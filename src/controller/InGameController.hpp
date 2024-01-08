/**
 * @file InGameController.hpp
 * @author Tomáš Ludrovan
 * @brief InGameController class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INGAMECONTROLLER_HPP
#define INGAMECONTROLLER_HPP

#include <ctime>

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "core/ICore.hpp"

class InGameController : public GeneralControllerBase {
private:
	std::unique_ptr<ICore> m_core;
	Timer m_tickTimer;
public:
	InGameController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void loopEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // INGAMECONTROLLER_HPP