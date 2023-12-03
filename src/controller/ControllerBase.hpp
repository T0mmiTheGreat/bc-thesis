/**
 * @file ControllerBase.hpp
 * @author Tomáš Ludrovan
 * @brief ControllerBase class
 * @version 0.1
 * @date 2023-11-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef CONTROLLERBASE_HPP
#define CONTROLLERBASE_HPP

#include <memory>
#include <mutex>
#include <condition_variable>

#include "controller/IController.hpp"

class ControllerBase : public IController {};

#endif // CONTROLLERBASE_HPP
