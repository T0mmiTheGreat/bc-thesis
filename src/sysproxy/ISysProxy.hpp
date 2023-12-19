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

#include <string>

#include "types.hpp"

/**
 * @brief Proxy for communication with the system manager (e.g. SDLManager).
 */
class ISysProxy {
public:
	virtual ~ISysProxy() {}
	/**
	 * @brief Runs the event loop synchronously.
	 * 
	 * @details Most methods may be called only if this method is running.
	 */
	virtual void runEventLoop() = 0;
	/**
	 * @brief Requests the whole client area to be repainted.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual void invalidateRect() = 0;
	/**
	 * @brief Requests repainting of a portion of client area.
	 * 
	 * @param rect The portion of client area to repaint.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual void invalidateRect(const Rect& rect) = 0;
	/**
	 * @brief Returns true if the key represented by `keyCode` is pressed.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual bool isKeyPressed(KeyCode keyCode) = 0;
	/**
	 * @brief Returns the current event loop state.
	 */
	virtual EventLoopState getEventLoopState() = 0;
	virtual Size2d getTextSize(const std::string& text, FontId font) = 0;
	virtual Size2d getPaintAreaSize() = 0;
};

#endif // ISYSPROXY_HPP
