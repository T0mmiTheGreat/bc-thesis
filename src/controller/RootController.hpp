/**
 * @file RootController.hpp
 * @author Tomáš Ludrovan
 * @brief RootController class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ROOTCONTROLLER_HPP
#define ROOTCONTROLLER_HPP

#include <memory>

#include "types.hpp"
#include "canvas/ICanvas.hpp"
#include "controller/ControllerBase.hpp"
#include "controller/IRootController.hpp"
#include "syssubscriber/ISysSubscriber.hpp"

class RootController : public ControllerBase, public IRootController {
private:
	std::unique_ptr<IControllerChild> m_childController;
public:
	RootController();
	void startEvent() override;
	void quitEvent() override;
	void keyDownEvent(KeyCode key) override;
	void mouseBtnDownEvent(MouseBtn btn) override;
	void mouseMoveEvent(int x, int y) override;
	void frameEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
	void run() override;

	void setChildController(std::unique_ptr<IControllerChild> value);
};

#endif // ROOTCONTROLLER_HPP
