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
	std::unique_ptr<IControllerChild> createReplacement();
public:
	void frameEvent() override;
};

#endif // INITIALCONTROLLER_HPP
