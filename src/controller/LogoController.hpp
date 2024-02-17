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
	std::shared_ptr<IControllerChild> createReplacement() override;

	virtual void onStarted();
	virtual void onLoop();
	virtual void onPaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect);
public:
	LogoController(std::shared_ptr<ISysProxy> sysProxy) : GeneralControllerBase(sysProxy) {}
};

#endif // LOGOCONTROLLER_HPP
