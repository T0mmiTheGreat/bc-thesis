/**
 * @file InitialController.hpp
 * @author Tomáš Ludrovan
 * @brief InitialController class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INITIALCONTROLLER_HPP
#define INITIALCONTROLLER_HPP

#include "controller/GeneralControllerBase.hpp"

class InitialController : public GeneralControllerBase {
protected:
	std::shared_ptr<IControllerChild> createReplacement();

	void onLoop() override;
public:
	InitialController(std::shared_ptr<ISysProxy> sysProxy) : GeneralControllerBase(sysProxy) {}
};

#endif // INITIALCONTROLLER_HPP
