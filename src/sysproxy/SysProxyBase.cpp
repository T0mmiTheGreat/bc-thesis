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

bool SysProxyBase::isCtrlPressed()
{
	return (isKeyPressed(KEY_LCTRL) || isKeyPressed(KEY_RCTRL));
}

bool SysProxyBase::isShiftPressed()
{
	return (isKeyPressed(KEY_LSHIFT) || isKeyPressed(KEY_RSHIFT));
}

bool SysProxyBase::isAltPressed()
{
	return (isKeyPressed(KEY_LALT) || isKeyPressed(KEY_RALT));
}
