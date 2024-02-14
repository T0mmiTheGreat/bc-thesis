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
#include "types.hpp"

/**
 * @brief Concrete implementation of ISysProxy for the SDL library.
 */
class SDLSysProxy : public SysProxyBase {
public:
	void runEventLoop() override;
	void invalidateRect() override;
	void invalidateRect(const Rect& rect) override;
	bool isKeyPressed(KeyCode keyCode) override;
	Point getMousePos() override;
	EventLoopState getEventLoopState() override;
	Size2d getTextSize(const std::string& text, FontId font) override;
	Size2d getImageSize(ImageId img) override;
	Size2d getPaintAreaSize() override;
	void setTextInputEnabled(bool value) override;
	void showErrorMessage(const std::string& msg) override;
	void quit() override;
};

#endif // SDLSYSPROXY_HPP
