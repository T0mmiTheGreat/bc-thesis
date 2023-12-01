/**
 * @file SysTypes.hpp
 * @author Tomáš Ludrovan
 * @brief Types used by ISysSubscriber
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYSTYPES_HPP
#define SYSTYPES_HPP

typedef enum KeyCode {
	KEY_UNKNOWN,
	KEY_RETURN,
	KEY_LEFT_ARROW,
	KEY_UP_ARROW,
	KEY_RIGHT_ARROW,
	KEY_DOWN_ARROW,
	// TODO: more keys
	// XXX: also add to SysEvent publishers/subscribers if needed
} KeyCode;

typedef enum MouseBtn {
	BTN_UNKNOWN,
	BTN_LEFT,
	BTN_MIDDLE,
	BTN_RIGHT,
} MouseBtn;

#endif // SYSTYPES_HPP
