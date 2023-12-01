/**
 * @file SysProxyBase.cpp
 * @author Tomáš Ludrovan
 * @brief SysProxyBase class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sysproxy/SysProxyBase.hpp"

#include <memory>

#include "sysproxy/SDLSysProxy.hpp"

std::shared_ptr<ISysProxy> SysProxyBase::createDefault()
{
	return std::make_shared<SDLSysProxy>();
}

