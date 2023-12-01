/**
 * @file ISysSubscriber.hpp
 * @author Tomáš Ludrovan
 * @brief ISysSubscriber interface
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ISYSSUBSCRIBER_HPP
#define ISYSSUBSCRIBER_HPP

#include "types.hpp"

class ISysSubscriber {
public:
	virtual ~ISysSubscriber() {}
	/**
	 * @brief The event loop has started.
	 */
	virtual void startEvent() = 0;
	/**
	 * @brief The user quit the application.
	 * 
	 * @details This is the last event to happen during the run time of the
	 *          application.
	 */
	virtual void quitEvent() = 0;
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
	virtual void frameEvent() = 0;
	// virtual void paintEvent() = 0;  -- TODO - parameter?
};

#endif // ISYSSUBSCRIBER_HPP
