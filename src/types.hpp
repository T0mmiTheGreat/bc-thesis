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
#include <ctime>

/**
 * @brief State of the event loop execution.
 */
enum EventLoopState {
	// Can run event loop
	// Cannot call invalidateRect() and similar methods
	EVENTLOOP_PRERUN,
	// Event loop is running, but start event hasn't been sent yet
	// Can call invalidateRect() and similar methods
	EVENTLOOP_PRESTARTEVENT,
	// Event loop is running, but cannot run another event loop
	// Can call invalidateRect() and similar methods
	EVENTLOOP_RUNNING,
	// Event loop is finished and cannot start a new one
	// Cannot call invalidateRect() and similar methods
	EVENTLOOP_POSTRUN,
};

/**
 * @brief Library-independent key codes.
 * 
 * @details These key codes are recognized by the application. The mapping
 *          from system key codes (e.g. SDL_Keycode) to this enum and vice
 *          versa should be done in the corresponding subscriber/adapter (for
 *          SDL it is `SDLSubscriber`).
 */
typedef enum KeyCode {
	KEY_UNKNOWN,
	KEY_RETURN, // Or ENTER
	KEY_LEFT_ARROW,
	KEY_UP_ARROW,
	KEY_RIGHT_ARROW,
	KEY_DOWN_ARROW,
	// TODO: more keys
	// XXX: also add to SysEvent publishers/subscribers if needed
} KeyCode;

/**
 * @brief Library-independent mouse button codes.
 * 
 * @details These mouse button codes are recognized by the application. The
 *          mapping from system mouse button codes to this enum and vice versa
 *          should be done in the corresponding subscriber/adapter (for
 *          SDL it is `SDLSubscriber`).
 */
typedef enum MouseBtn {
	BTN_UNKNOWN,
	BTN_LEFT,
	BTN_MIDDLE,
	BTN_RIGHT,
} MouseBtn;

enum FontId {
	FONT_OLDENGL_72 = 0,
	COUNT_FONTID
};
const size_t fontIdCount = (size_t)COUNT_FONTID;

/**
 * @brief The axis-aligned rectangle type.
 */
struct Rect {
	/**
	 * @brief X coordinate of the left side of the rectangle.
	 */
	int x;
	/**
	 * @brief Y coordinate of the left side of the rectangle.
	 */
	int y;
	/**
	 * @brief Width of the rectangle.
	 */
	int w;
	/**
	 * @brief Height of the rectangle.
	 */
	int h;

	Rect()
		: Rect(0, 0, 0, 0)
	{}
	Rect(int x, int y, int w, int h)
		: x{x}
		, y{y}
		, w{w}
		, h{h}
	{}
	Rect createEmpty() { return Rect(); }

	/**
	 * @brief Get X coordinate of the right side of the rectangle.
	 */
	int getRight() const { return x + w; }
	/**
	 * @brief Get Y coordinate of the bottom side of the rectangle.
	 */
	int getBottom() const { return y + h; }

	/**
	 * @brief Get the union of two rectangles.
	 * 
	 * @details A union of rectangles in 2D space may result in an octagonal
	 *          shape. In order to make things simple for us, let's asume the
	 *          union "the smallest rectangle that both rectangles fit into".
	 */
	Rect unionRect(const Rect& rhs) const {
		Rect r;
		r.x = (this->x < rhs.x ? this->x : rhs.x);
		r.y = (this->y < rhs.y ? this->y : rhs.y);
		r.w = (this->getRight() < r.getRight() ? this->getRight() : r.getRight()) - r.x;
		r.h = (this->getBottom() < r.getBottom() ? this->getBottom() : r.getBottom()) - r.y;
		return r;
	}

	/**
	 * @brief Returns true if the rectangle has no area.
	 */
	bool isEmpty() const {
		return (this->w <= 0 || this->h <= 0);
	}

	/**
	 * @brief Alias for rectangle union.
	 */
	Rect operator+ (const Rect& rhs) const {
		return unionRect(rhs);
	}

	/**
	 * @brief Unions the rectangle with another.
	 * 
	 * @return The reference to self.
	 */
	Rect& operator+= (const Rect& rhs) {
		return (*this = this->unionRect(rhs));
	}
};

/**
 * @brief Width and height structure.
 */
struct Size2d {
	size_t w;
	size_t h;

	Size2d(size_t w, size_t h)
		: w{w}
		, h{h}
	{}

	Size2d()
		: Size2d(0, 0)
	{}

	Size2d operator+ (const Size2d& rhs) const {
		return Size2d(this->w + rhs.w, this->h + rhs.h);
	}
	Size2d operator- () const {
		return Size2d(-w, -h);
	}
	Size2d operator- (const Size2d& rhs) const {
		return *this + (-rhs);
	}
	Size2d& operator+= (const Size2d& rhs) {
		return (*this = *this + rhs);
	}
	Size2d& operator-= (const Size2d& rhs) {
		return (*this = *this - rhs);
	}
};

/**
 * @brief RGBA color.
 */
struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	/**
	 * @brief Constructs a black color.
	 */
	Color()
		: Color(0, 0, 0, 0xff)
	{}
	/**
	 * @brief Construct a color.
	 * 
	 * @param r Intensity of red channel.
	 * @param g Intensity of green channel.
	 * @param b Intensity of blue channel.
	 * @param a Intensity of alpha channel.
	 */
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r{r}
		, g{g}
		, b{b}
		, a{a}
	{}
};

/**
 * @brief Simple timer implementation.
 * 
 * @details The timer doesn't produce events. Instead, it must be queried when
 *          needed.
 * 
 * @remark The timer may overflow.
 */
class Timer {
private:
	std::clock_t m_interval;
	std::clock_t m_nextTimestamp;

	std::clock_t msToClocks(std::clock_t ms) {
		// ms / 1000 * CPS -> convert ms to s, then convert s to clocks.
		// The operands are swapped here to avoid unnecessary FP conversions
		// but keep precision
		return ms * CLOCKS_PER_SEC / 1000;
	}
public:
	/**
	 * @brief Construct a new Timer object.
	 * 
	 * @param interval Timer interval in milliseconds.
	 */
	Timer(std::clock_t interval) :
		m_interval{msToClocks(interval)}
	{
		reset();
	}
	/**
	 * @brief Starts the timer again.
	 */
	void reset() {
		m_nextTimestamp = std::clock() + m_interval;
	}
	/**
	 * @brief Checks whether the timer interval has elapsed.
	 * 
	 * @details If multiple timer intervals elapse between function calls they
	 *          won't be skipped.
	 */
	bool isLap() {
		std::clock_t now = std::clock();
		bool res = (now >= m_nextTimestamp);
		if (res) {
			m_nextTimestamp += m_interval;
		}
		return res;
	}
};

#endif // TYPES_HPP
