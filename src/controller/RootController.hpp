/**
 * @file RootController.hpp
 * @author Tomáš Ludrovan
 * @brief RootController class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ROOTCONTROLLER_HPP
#define ROOTCONTROLLER_HPP

#include <memory>
#include <atomic>

#include "controller/ControllerBase.hpp"
#include "controller/IController.hpp"

class RootController : public ControllerBase {
private:
	std::unique_ptr<IController> m_childController;
	std::atomic_bool m_isQuit;

	void runChildren();
public:
	RootController() : ControllerBase(), m_isQuit{false} {}

	std::unique_ptr<IController> runController() override;
	void startEvent() override;
	void quitEvent() override;
	void keyDownEvent(KeyCode key) override;
	void mouseBtnDownEvent(MouseBtn btn) override;
	void mouseMoveEvent(int x, int y) override;
	void finishedEvent() override;
	void abortEvent() override;
	void frameEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
};

#endif // ROOTCONTROLLER_HPP
