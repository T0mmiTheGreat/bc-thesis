/**
 * @file GeneralControllerBase.hpp
 * @author Tomáš Ludrovan
 * @brief GeneralControllerBase class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GENERALCONTROLLERBASE_HPP
#define GENERALCONTROLLERBASE_HPP

#include <memory>

#include "types.hpp"
#include "canvas/ICanvas.hpp"
#include "controller/IControllerChild.hpp"
#include "sysproxy/ISysProxy.hpp"

class GeneralControllerBase : public IControllerChild {
protected:
	SwapCallback swapCallback;
	std::unique_ptr<ISysProxy> sysProxy;

	virtual std::unique_ptr<IControllerChild> createReplacement();
public:
	GeneralControllerBase();
	virtual void startedEvent() override;
	virtual void finishedEvent() override;
	virtual void stoppedEvent() override;
	virtual void abortedEvent() override;
	virtual void keyDownEvent(KeyCode key) override;
	virtual void mouseBtnDownEvent(MouseBtn btn) override;
	virtual void mouseMoveEvent(int x, int y) override;
	virtual void frameEvent() override;
	virtual void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
	virtual void setSwapCallback(SwapCallback f) override;
};

#endif // GENERALCONTROLLERBASE_HPP
