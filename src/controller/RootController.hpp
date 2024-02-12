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
#include <vector>

#include "types.hpp"
#include "canvas/ICanvas.hpp"
#include "controller/ControllerBase.hpp"
#include "controller/IControllerChild.hpp"
#include "controller/IRootController.hpp"
#include "sysproxy/ISysProxy.hpp"
#include "syssubscriber/ISysSubscriber.hpp"

class RootController 
	: public ControllerBase
	, public IRootController
	, public IControllerChild::IParent
{
private:
	std::shared_ptr<IControllerChild> m_childController;
	std::vector<std::shared_ptr<IControllerChild>> m_pausedControllers;
	std::shared_ptr<ISysProxy> m_sysProxy;
public:
	RootController(std::shared_ptr<ISysProxy> sysProxy);
	void startEvent() override;
	void quitEvent() override;
	void keyDownEvent(KeyCode key) override;
	void textInputEvent(const char* text) override;
	void mouseBtnDownEvent(MouseBtn btn, int x, int y) override;
	void mouseBtnUpEvent(MouseBtn btn, int x, int y) override;
	void mouseMoveEvent(int x, int y) override;
	void mouseWheelEvent(int dx, int dy) override;
	void loopEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
	void run() override;
	void replaceController(std::shared_ptr<IControllerChild> replacement)
		override;
	void pauseController(std::shared_ptr<IControllerChild> replacement)
		override;

	void setChildController(std::shared_ptr<IControllerChild> value);
};

#endif // ROOTCONTROLLER_HPP
