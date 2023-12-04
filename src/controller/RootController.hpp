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
#include "controller/ControllerFactory.hpp"

/**
 * @brief Root of the controller tree.
 */
class RootController : public ControllerBase {
private:
	/**
	 * @brief The only child of this controller.
	 * 
	 * @details It may change several times during the run of the controller.
	 *          The initial child is set before the run of the controller.
	 */
	std::unique_ptr<IController> m_childController;
	/**
	 * @brief Flag indicating the end of the controller run.
	 * 
	 * @remark Although there should be only one place where the value is
	 *         written (apart from constructor), it is better to make is atomic
	 *         to emphasize that the variable is accessed from multiple threads.
	 */
	std::atomic_bool m_isQuit;

	/**
	 * @brief Callback for the controller run loop thread.
	 */
	void runChildren();
public:
	RootController() :
		ControllerBase(),
		m_childController{ControllerFactory::createInitialController()},
		m_isQuit{false}
	{}

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
