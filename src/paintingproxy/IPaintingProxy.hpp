/**
 * @file IPaintingProxy.hpp
 * @author Tomáš Ludrovan
 * @brief IPaintingProxy interface
 * @version 0.1
 * @date 2024-01-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef IPAINTINGPROXY_HPP
#define IPAINTINGPROXY_HPP

#include <string>

#include "types.hpp"

/**
 * @brief Proxy for communication with the painting manager.
 * 
 * @remark Subset of sys proxy, used by objects responsible for drawing.
 */
class IPaintingProxy {
public:
	virtual ~IPaintingProxy() {}
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
	 * @brief Calculates the size that would be taken by text.
	 * 
	 * @param text Text to measure.
	 * @param font Font to use for the text.
	 */
	virtual Size2d getTextSize(const std::string& text, FontId font) = 0;
	/**
	 * @brief Returns the current size of the screen area in pixels.
	 */
	virtual Size2d getPaintAreaSize() = 0;
};

#endif // IPAINTINGPROXY_HPP
