/**
 * @file SysProxyBase.hpp
 * @author Tomáš Ludrovan
 * @brief SysProxyBase class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYSPROXYBASE_HPP
#define SYSPROXYBASE_HPP

#include <memory>

#include "sysproxy/ISysProxy.hpp"

class SysProxyBase : public ISysProxy {
public:
	static std::shared_ptr<ISysProxy> createDefault();
};

#endif // SYSPROXYBASE_HPP
