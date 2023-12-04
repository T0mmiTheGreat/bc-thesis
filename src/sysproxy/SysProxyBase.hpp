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

/**
 * @brief Base class for sys proxies.
 * 
 * @details In future, there might be some definitions common to all proxies,
 *          so it is better for all proxies to inherit from this one.
 */
class SysProxyBase : public ISysProxy {};

#endif // SYSPROXYBASE_HPP
