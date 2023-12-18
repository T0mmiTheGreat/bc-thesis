/**
 * @file SysProxyFactory.hpp
 * @author Tomáš Ludrovan
 * @brief SysProxyFactory class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYSPROXYFACTORY_HPP
#define SYSPROXYFACTORY_HPP

#include <memory>

#include "sysproxy/ISysProxy.hpp"

/**
 * @brief Factory for ISysProxy interface.
 */
class SysProxyFactory {
public:
	static std::unique_ptr<ISysProxy> createDefault();
	static std::unique_ptr<ISysProxy> createSdlSysProxy();
};

#endif // SYSPROXYFACTORY_HPP
