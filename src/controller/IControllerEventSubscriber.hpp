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
 */
class IControllerEventSubscriber {
public:
	virtual ~IControllerEventSubscriber() {}
	/**
	 * @brief The controller should start.
	 * 
	 * @details The first event that the controller receives.
	 */
	virtual void startedEvent() = 0;
	/**
	 * @brief The controller should end.
	 * 
	 * @details The last event that the controller receives. Should be sent
	 *          only by the controller itself and not its parent.
	 */
	virtual void finishedEvent() = 0;
	/**
	 * @brief The controller should stop.
	 * 
	 * @details In most cases identical to finishedEvent(). Should only be sent
	 *          by its parent and not the controller itself.
	 */
	virtual void stoppedEvent() = 0;
	/**
	 * @brief The controller should stop.
	 * 
	 * @details In most cases identical to finishedEvent(). May be sent both
	 *          by parent and the controller itself. Puts more emphasis on the
	 *          "exceptional termination".
	 */
	virtual void abortedEvent() = 0;
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
	 * @brief A mouse cursor moved.
	 * 
	 * @param x The x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseMoveEvent(int x, int y) = 0;
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
	 * @remark The recipent may choose to repaint area larger than invalidRect,
	 *         but they must modify the invalidRect value to mach the area they
	 *         painted.
	 */
	virtual void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) = 0;
};

#endif // ICONTROLLEREVENTSUBSCRIBER_HPP
