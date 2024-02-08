/**
 * @file LogoController.hpp
 * @author Tomáš Ludrovan
 * @brief LogoController class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LOGOCONTROLLER_HPP
#define LOGOCONTROLLER_HPP

#include <memory>

#include "types.hpp"

#include "controller/GeneralControllerBase.hpp"
#include "sprite/LogoSprite.hpp"

/**
 * @brief Controller that displays logo.
 * 
 */
class LogoController : public GeneralControllerBase {
private:
	std::unique_ptr<LogoSprite> m_sprite;
protected:
	std::unique_ptr<IControllerChild> createReplacement() override;
public:
	LogoController(std::shared_ptr<ISysProxy> sysProxy) : GeneralControllerBase(sysProxy) {}
	virtual void startedEvent() override;
	virtual void loopEvent() override;
	virtual void paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override;
};

#endif // LOGOCONTROLLER_HPP
