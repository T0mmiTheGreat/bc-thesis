/**
 * @file ISysProxy.hpp
 * @author Tomáš Ludrovan
 * @brief ISysProxy interface
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ISYSPROXY_HPP
#define ISYSPROXY_HPP

#include "types.hpp"

class ISysProxy {
public:
	virtual ~ISysProxy() {}
	virtual void invalidateRect(const Rect& rect) = 0;
	virtual bool isKeyPressed(KeyCode keyCode) = 0;
};

#endif // ISYSPROXY_HPP
