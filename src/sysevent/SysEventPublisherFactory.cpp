#include "SysEventPublisherFactory.hpp"
/**
 * @file SysEventPublisherFactory.cpp
 * @author Tomáš Ludrovan
 * @brief SysEventPublisherFactory class
 * @version 0.1
 * @date 2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <memory>

#include "SDLSysEventPublisher.hpp"

std::shared_ptr<ISysEventPublisher> SysEventPublisherFactory::createPublisher() const
{
	return std::make_shared<SDLSysEventPublisher>();
}

