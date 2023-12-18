/**
 * @file SysProxyFactory.cpp
 * @author Tomáš Ludrovan
 * @brief SysProxyFactory class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sysproxy/SysProxyFactory.hpp"

#include "sysproxy/SDLSysProxy.hpp"
#include "SysProxyFactory.hpp"

std::unique_ptr<ISysProxy> SysProxyFactory::createDefault()
{
	return createSdlSysProxy();
}
std::unique_ptr<ISysProxy> SysProxyFactory::createSdlSysProxy()
{
	return std::make_unique<SDLSysProxy>();
}