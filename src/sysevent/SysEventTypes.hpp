/**
 * @file SysEventTypes.hpp
 * @author Tomáš Ludrovan
 * @brief Types used by SysEvent classes
 * @version 0.1
 * @date 2023-11-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SYSEVENTTYPES_HPP
#define SYSEVENTTYPES_HPP

typedef enum KeyCode {
	KEY_UNKNOWN,
	KEY_RETURN,
	KEY_LEFT_ARROW,
	KEY_UP_ARROW,
	KEY_RIGHT_ARROW,
	KEY_DOWN_ARROW,
	// TODO: more keys
	// XXX: also add to SysEvent publishers if needed
} KeyCode;

typedef enum MouseBtn {
	BTN_UNKNOWN,
	BTN_LEFT,
	BTN_MIDDLE,
	BTN_RIGHT,
} MouseBtn;

#endif // SYSEVENTTYPES_HPP
