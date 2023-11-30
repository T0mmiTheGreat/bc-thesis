/**
 * @file ISysEventSubscriber.hpp
 * @author Tomáš Ludrovan
 * @brief ISysEventSubscriber interface
 * @version 0.1
 * @date 2023-11-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ISYSEVENTSUBSCRIBER_HPP
#define ISYSEVENTSUBSCRIBER_HPP

#include "SysEventTypes.hpp"

/**
 * @brief Interface for an object that will subscribe to events published by ISysEventPublisher.
 * 
 * @details The methods should not take too much time to execute. If they do,
 *          they should run asynchronously.
 * 
 *          DO NOT call the methods manually. They should be called from the
 *          ISysEventPublisher type object only.
 */
class ISysEventSubscriber {
public:
	virtual ~ISysEventSubscriber() {}
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
	 */
	virtual void mouseBtnDownEvent(MouseBtn btn) = 0;
	/**
	 * @brief A mouse cursor has moved.
	 * 
	 * @param x The x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	virtual void mouseMoveEvent(int x, int y) = 0;
	/**
	 * @brief The user quit the application.
	 * 
	 * @details This is the last event to happen during the run time of the
	 *          application.
	 */
	virtual void quitEvent() = 0;
};


#endif // ISYSEVENTSUBSCRIBER_HPP
