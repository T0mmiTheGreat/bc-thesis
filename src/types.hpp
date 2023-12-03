/**
 * @file types.hpp
 * @author Tomáš Ludrovan
 * @brief General-purpose types
 * @version 0.1
 * @date 2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>

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

struct Rect {
	int x;
	int y;
	int w;
	int h;

	int getRight() const { return x + w; }
	int getBottom() const { return y + h; }

	Rect unionRect(const Rect& rhs) const {
		Rect r;
		r.x = (this->x < rhs.x ? this->x : rhs.x);
		r.y = (this->y < rhs.y ? this->y : rhs.y);
		r.w = (this->getRight() < r.getRight() ? this->getRight() : r.getRight()) - r.x;
		r.h = (this->getBottom() < r.getBottom() ? this->getBottom() : r.getBottom()) - r.y;
		return r;
	}

	bool isEmpty() const {
		return (this->w <= 0 || this->h <= 0);
	}

	Rect operator+ (const Rect& rhs) const {
		return unionRect(rhs);
	}

	Rect& operator+= (const Rect& rhs) {
		return (*this = this->unionRect(rhs));
	}
};

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Color() : Color(0, 0, 0, 0) {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
		r{r},
		g{g},
		b{b},
		a{a}
	{}
};


#endif // TYPES_HPP
