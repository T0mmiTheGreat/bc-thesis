/**
 * @file IControllerEventSubscriber.hpp
 * @author Tomáš Ludrovan
 * @brief IControllerEventSubscriber interface
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ICONTROLLEREVENTSUBSCRIBER_HPP
#define ICONTROLLEREVENTSUBSCRIBER_HPP

#include <memory>

#include "types.hpp"
#include "canvas/ICanvas.hpp"

/**
 * @brief Controller that receives events from another controller.
 * 
 * @details Many "begin" and "end" events are provided here, but none of them
 *          should be sent directly. There are specialized functions which take
 *          care of it. I.e., don't call `startedEvent()`, call `start()`
 *          instead.
 */
class IControllerEventSubscriber {
public:
	virtual ~IControllerEventSubscriber() {}

	/**
	 * @brief The controller has started.
	 * 
	 * @details The first event that the controller receives. Should only be
	 *          called by its parent.
	 */
	virtual void startedEvent() = 0;
	/**
	 * @brief The controller has finished.
	 * 
	 * @details The last event that the controller receives. Should only be
	 *          called by the controller itself.
	 */
	virtual void finishedEvent() = 0;
	/**
	 * @brief The controller has stopped.
	 * 
	 * @details The last event that the controller receives. Should only be
	 *          called by its parent.
	 */
	virtual void stoppedEvent() = 0;
	/**
	 * @brief The controller has stopped.
	 * 
	 * @details The last event that the controller receives. Puts more emphasis
	 *          on the "exceptional termination". May be called both by its
	 *          parent and the controller itself.
	 */
	virtual void abortedEvent() = 0;
	/**
	 * @brief The controller has stopped, but should not reset its state as it
	 *        will be resumed again.
	 * 
	 * @details Should only be called by the controller itself. Should call
	 *          The next event it receives should be either `resumedEvent()`
	 *          or any of the "end" events.
	 */
	virtual void pausedEvent() = 0;
	/**
	 * @brief The controller was awoken after `pausedEvent()`.
	 * 
	 * @details Should only be called by its parent.
	 */
	virtual void resumedEvent() = 0;

	/**
	 * @brief A key was pressed.
	 * 
	 * @details Only keys on the keyboard are observed. For mouse buttons use
	 *          mouseBtnDownEvent().
	 * 
	 * @param key The code of the key.
	 */
	virtual void keyDownEvent(KeyCode key) = 0;
	/**
	 * @brief A character key was pressed.
	 * 
	 * @param text The null-terminated input text in UTF-8 encoding.
	 * 
	 * @remark The parameter type is compatible with `SDL_TextInputEvent`.
	 *         For more details, refer to:
	 *         https://wiki.libsdl.org/SDL2/SDL_TextInputEvent
	 * 
	 * @example ```
	 *          std::string s = "Input character: ";
	 *          s += text;
	 *          ```
	 */
	virtual void textInputEvent(const char* text) = 0;
	/**
	 * @brief A mouse button was pressed.
	 * 
	 * @param btn The code of the button.
	 * @param x The mouse x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The mouse y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseBtnDownEvent(MouseBtn btn, int x, int y) = 0;
	/**
	 * @brief A mouse button was released.
	 * 
	 * @param btn The code of the button.
	 * @param x The mouse x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The mouse y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseBtnUpEvent(MouseBtn btn, int x, int y) = 0;
	/**
	 * @brief A mouse cursor moved.
	 * 
	 * @param x The x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseMoveEvent(int x, int y) = 0;
	/**
	 * @brief A mouse wheel has been scrolled.
	 * 
	 * @param dx The amount scrolled horizontally, positive to the right and
	 *           negative to the left.
	 * @param dy The amount scrolled vertically, positive away from the user
	 *           and negative towards the user.
	 */
	virtual void mouseWheelEvent(int dx, int dy) = 0;
	/**
	 * @brief Event that happens every event loop iteration.
	 */
	virtual void loopEvent() = 0;
	/**
	 * @brief Request to paint.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The recipent may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	virtual void paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) = 0;
};

#endif // ICONTROLLEREVENTSUBSCRIBER_HPP
