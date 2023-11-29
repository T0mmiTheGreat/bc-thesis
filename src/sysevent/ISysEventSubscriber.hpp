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

class ISysEventSubscriber {
public:
	virtual ~ISysEventSubscriber() {}
	virtual void keyDownEvent(KeyCode key) = 0;
	virtual void mouseBtnDownEvent(MouseBtn btn) = 0;
	virtual void mouseMoveEvent(int x, int y) = 0;
	virtual void quitEvent() = 0;
};


#endif // ISYSEVENTSUBSCRIBER_HPP
