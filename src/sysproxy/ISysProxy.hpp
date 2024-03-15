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
#include "paintingproxy/IPaintingProxy.hpp"
#include "playerinput/ISysProxyPlayerInput.hpp"

/**
 * @brief Proxy for communication with the system manager (e.g. SDLManager).
 */
class ISysProxy
	: public IPaintingProxy
	, public ISysProxyPlayerInput
{
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
	 * @brief Returns true if either left or right Ctrl is pressed.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual bool isCtrlPressed() = 0;
	/**
	 * @brief Returns true if either left or right Shift is pressed.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual bool isShiftPressed() = 0;
	/**
	 * @brief Returns true if either left or right Alt is pressed.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual bool isAltPressed() = 0;
	/**
	 * @brief Returns the current mouse position.
	 * 
	 * @remark May only be called if the event loop is running.
	 */
	virtual Point getMousePos() = 0;
	/**
	 * @brief Returns the current event loop state.
	 */
	virtual EventLoopState getEventLoopState() = 0;
	/**
	 * @brief Calculates the size that would be taken by text.
	 * 
	 * @param text Text to measure.
	 * @param font Font to use for the text.
	 */
	virtual Size2d getTextSize(const std::string& text, FontId font) = 0;
	/**
	 * @brief Returns the size of an image.
	 */
	virtual Size2d getImageSize(ImageId img) = 0;
	/**
	 * @brief Returns the current size of the screen area in pixels.
	 */
	virtual Size2d getPaintAreaSize() = 0;
	/**
	 * @brief Enables or disables text input events.
	 * 
	 * @details The text input events won't be sent by default, because they
	 *          may slow down the application. Pass `true` to enable sending
	 *          them. After you are done receiving the events, disable them
	 *          by passing `false` to this method.
	 */
	virtual void setTextInputEnabled(bool value) = 0;
	/**
	 * @brief Shows a dialog box reporting an error.
	 */
	virtual void showErrorMessage(const std::string& msg) = 0;
	/**
	 * @brief Requests the program to stop.
	 * 
	 * @details This method should stop the event loop just like pressing the
	 *          'X' button. The implementations may not necessarily "simulate"
	 *          the 'X' button click exactly but should try to (the caller is
	 *          unlikely to notice any difference anyway). Quit event is the
	 *          consequence of calling this method, although there may be some
	 *          events in the queue before the quit event.
	 */
	virtual void quit() = 0;
};

#endif // ISYSPROXY_HPP
