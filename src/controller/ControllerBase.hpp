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

/**
 * @brief Base class for all controllers.
 * 
 * @details In future, there might be some definitions common to all controllers,
 *          so it is better for all controllers to inherit from this one.
 */
class ControllerBase : public IController {};

#endif // CONTROLLERBASE_HPP
