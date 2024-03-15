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
 */
class SysProxyBase : public ISysProxy {
public:
	/**
	 * @brief Returns true if either left or right Ctrl is pressed.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual bool isCtrlPressed() override;
	/**
	 * @brief Returns true if either left or right Shift is pressed.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual bool isShiftPressed() override;
	/**
	 * @brief Returns true if either left or right Alt is pressed.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual bool isAltPressed() override;
};

#endif // SYSPROXYBASE_HPP
