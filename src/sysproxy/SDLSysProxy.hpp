/**
 * @file SDLSysProxy.hpp
 * @author Tomáš Ludrovan
 * @brief SDLSysProxy class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SDLSYSPROXY_HPP
#define SDLSYSPROXY_HPP

#include "sysproxy/SysProxyBase.hpp"

class SDLSysProxy : public SysProxyBase {
public:
	void runEventLoop() override;
	void invalidateRect() override;
	void invalidateRect(const Rect& rect) override;
	bool isKeyPressed(KeyCode keyCode) override;
};

#endif // SDLSYSPROXY_HPP
