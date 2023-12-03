/**
 * @file GeneralControllerBase.hpp
 * @author Tomáš Ludrovan
 * @brief GeneralControllerBase class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GENERALCONTROLLERBASE_HPP
#define GENERALCONTROLLERBASE_HPP

#include <memory>
#include <mutex>
#include <condition_variable>

#include "controller/ControllerBase.hpp"
#include "sysproxy/ISysProxy.hpp"
#include "sysproxy/SysProxyFactory.hpp"

class GeneralControllerBase : public ControllerBase {
private:
	void sendStartEventIfEventLoopRunning();
protected:
	bool isControllerFinished;
	std::condition_variable cvIsControllerFinished;
	std::mutex mutexIsControllerFinished;

	std::shared_ptr<ISysProxy> sysProxy;
public:
	GeneralControllerBase():
		isControllerFinished{false},
		sysProxy{SysProxyFactory::createDefault()}
	{}

	virtual std::unique_ptr<IController> runController() override;
	virtual void startEvent() override {}
	virtual void quitEvent() override;
	virtual void keyDownEvent(KeyCode key) override {}
	virtual void mouseBtnDownEvent(MouseBtn btn) override {}
	virtual void mouseMoveEvent(int x, int y) override {}
	virtual void finishedEvent() override;
	virtual void abortEvent() override;
	virtual void frameEvent() override {}
	virtual void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) {}
};

#endif // GENERALCONTROLLERBASE_HPP
