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

#include <array>
#include <cassert>
#include <cstdint>
#include <ctime>
#include <vector>

#include "math/Math.hpp"

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
	KEY_BACKSPACE,
	KEY_CTRL, // If Ctrl is not present, looks for Cmd
	KEY_SHIFT,
	KEY_ALT, // If Alt is not present, looks for Opt/Option
	KEY_LEFT_ARROW,
	KEY_UP_ARROW,
	KEY_RIGHT_ARROW,
	KEY_DOWN_ARROW,
	KEY_A,
	KEY_W,
	KEY_D,
	KEY_S,
	KEY_H,
	KEY_U,
	KEY_K,
	KEY_J,
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

/**
 * @brief Fonts used by the program.
 * 
 * @note The naming is "FONT_<font_file_name>_<font_size>"
 */
enum FontId {
	FONT_OLDENGL_72 = 0, // Old English Text MT
	FONT_SEGOEPR_66,     // Segoe Print
	FONT_BRLNSTR_20,     // Berlin Sans FB
	FONT_TAHOMA_16,      // Tahoma
	FONT_TAHOMA_20,      // Tahoma
	// XXX: Add new fonts to SDL manager too (constructor)

	COUNT_FONTID // Number of fonts used by the program
};
// Number of fonts used by the program
constexpr size_t fontIdCount = static_cast<size_t>(COUNT_FONTID);

/**
 * @brief Images used by the program.
 */
enum ImageId {
	IMG_ICON_NEW = 0,
	IMG_ICON_OPEN,
	IMG_ICON_SAVE,
	IMG_ICON_SAVE_AS,
	IMG_ICON_UNDO,
	IMG_ICON_REDO,
	IMG_ICON_COGWHEEL,
	IMG_ICON_BACK,
	IMG_ICON_SELECT_TOOL,
	IMG_ICON_PLAYER_TOOL,
	IMG_ICON_OBSTACLE_TOOL,
	IMG_ICON_TRASH_CAN,
	IMG_ICON_RESIZE,
	// XXX: Add new images to SDL manager too (constructor)

	COUNT_IMAGEID // Number of images used by the program
};
// Number of images used by the program
constexpr size_t imageIdCount = static_cast<size_t>(COUNT_IMAGEID);

/**
 * @brief Converts milliseconds to clocks.
 * 
 * @tparam T Scalar type.
 */
template <typename T>
inline constexpr T msToClocks(T ms);

/**
 * @brief A matrix with 3 rows and 3 columns suitable for transformations.
 * 
 * @note In this program the transformations use row vectors. When combining
 *       transformations (multiplying matrices) the transformations are as if
 *       performed from left to right.
 */
struct Matrix3x3 {
	typedef double ValueType;

	// matrix[rowIdx][colIdx]
	std::array<std::array<ValueType, 3>, 3> matrix;

	constexpr Matrix3x3(ValueType a11, ValueType a12, ValueType a13, ValueType a21,
		ValueType a22, ValueType a23, ValueType a31, ValueType a32,
		ValueType a33)
		: matrix{
			{a11, a12, a13,
			a21, a22, a23,
			a31, a32, a33}
		}
	{}

	constexpr Matrix3x3()
		: Matrix3x3(
			0.0, 0.0, 0.0,
			0.0, 0.0, 0.0,
			0.0, 0.0, 0.0
		)
	{}

	/**
	 * @brief Creates zero matrix. 
	 */
	static constexpr Matrix3x3 createZero() {
		return Matrix3x3();
	}

	/**
	 * @brief Creates identity matrix.
	 */
	static constexpr Matrix3x3 createIdentity() {
		return Matrix3x3(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		);
	}

	/**
	 * @brief Creates translation matrix.
	 * 
	 * @param tx Translation in the X axis.
	 * @param ty Translation in the Y axis.
	 */
	static constexpr Matrix3x3 createTranslation(double tx, double ty) {
		return Matrix3x3(
			 1,  0,  0,
			 0,  1,  0,
			tx, ty,  1
		);
	}

	/**
	 * @brief Creates scale matrix.
	 * 
	 * @param sx Scale in the X axis.
	 * @param sy Scale in the Y axis.
	 */
	static constexpr Matrix3x3 createScale(double sx, double sy) {
		return Matrix3x3(
			sx,  0,  0,
			 0, sy,  0,
			 0,  0,  1
		);
	}

	/**
	 * @brief Returns the value at the given position.
	 */
	constexpr const ValueType& get(size_t row, size_t col) const {
		return matrix[row][col];
	}
	/**
	 * @brief Returns the value at the given position.
	 */
	constexpr ValueType& get(size_t row, size_t col) {
		return matrix[row][col];
	}

	/**
	 * @brief Transforms the provided homogeneous coordinates.
	 * 
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @param h The homogeneous element.
	 * @param xOut Transformed X coordinate.
	 * @param yOut Transformed Y coordinate.
	 * @param hOut Transformed homogeneous element.
	 */
	constexpr void transform(ValueType x, ValueType y, ValueType h,
		ValueType& xOut, ValueType& yOut, ValueType& hOut) const
	{
		xOut = x * matrix[0][0];
		yOut = x * matrix[0][1];
		hOut = x * matrix[0][2];
		xOut += y * matrix[1][0];
		yOut += y * matrix[1][1];
		hOut += y * matrix[1][2];
		xOut += h * matrix[2][0];
		yOut += h * matrix[2][1];
		hOut += h * matrix[2][2];
	}

	/**
	 * @brief Transforms the provided Cartesian coordinates.
	 * 
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @param xOut Transformed X coordinate.
	 * @param yOut Transformed Y coordinate.
	 */
	constexpr void transform(ValueType x, ValueType y, ValueType& xOut,
		ValueType& yOut) const
	{
		ValueType hOut;

		transform(x, y, 1.0, xOut, yOut, hOut);

		xOut /= hOut;
		yOut /= hOut;
	}

	constexpr const std::array<ValueType, 3>& operator[] (size_t row) const {
		return matrix[row];
	}
	constexpr std::array<ValueType, 3>& operator[] (size_t row) {
		return matrix[row];
	}

	/**
	 * @brief Multiplies two matrices.
	 * 
	 * @note Note that matrix multiplication is NOT commutative: A*B != B*A
	 *       in most cases.
	 */
	constexpr Matrix3x3 operator* (const Matrix3x3& rhs) const {
		Matrix3x3 res = createZero();
		// This algorithm is optimized for cache misses, although I doubt that
		// at this little scale there would be any. Copied from:
		// https://www.fit.vutbr.cz/~peringer/UNOFFICIAL/IPS/IPS.pdf
		// if anyone cares...
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				for (int j = 0; j < 3; j++) {
					res.matrix[i][j] += this->matrix[i][k] * rhs.matrix[k][j];
				}
			}
		}
		return res;
	}

	constexpr Matrix3x3& operator*= (const Matrix3x3& rhs) {
		return (*this = *this * rhs);
	}
};

/**
 * @brief Structure representing X and Y coordinate.
 * 
 * @tparam T Scalar type.
 */
template <typename T>
struct PointGeneric {
	typedef T ValueType;

	/**
	 * @brief Hasher for PointGeneric type.
	 */
	struct Hash {
		size_t operator() (const PointGeneric<T>& key) const {
			size_t h1 = std::hash<T>{}(key.x);
			size_t h2 = std::hash<T>{}(key.y);
			return h1 ^ h2;
		}
	};

	T x;
	T y;

	constexpr PointGeneric(T x, T y)
		: x{x}
		, y{y}
	{}
	constexpr PointGeneric()
		: PointGeneric(0, 0)
	{}

	/**
	 * @brief Creates [0,0] point.
	 */
	static constexpr PointGeneric<T> zero() {
		return PointGeneric<T>();
	}

	/**
	 * @brief Calculates the point position relative to position of `rel`.
	 * 
	 * @remark The result is basically vector: `rel` -> `this`.
	 */
	constexpr PointGeneric<T> relativeTo(const PointGeneric<T>& rel) const {
		return PointGeneric<T>(x - rel.x, y - rel.y);
	}

	constexpr void transform(const Matrix3x3& tm) {
		PointGeneric<Matrix3x3::ValueType> resF;
		tm.transform(x, y, resF.x, resF.y);

		*this = static_cast<PointGeneric<T>>(resF);
	}

	/**
	 * @brief Returns the point transformed using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 */
	constexpr PointGeneric<T> getTransformed(const Matrix3x3& tm) const {
		PointGeneric<T> res(*this);
		res.transform(tm);
		return res;
	}

	constexpr void offset(T x, T y) {
		this->x += x;
		this->y += y;
	}

	constexpr void offset(const PointGeneric<T>& offsetPoint) {
		offset(offsetPoint.x, offsetPoint.y);
	}

	/**
	 * @brief Returns the squared distance between two points.
	 * 
	 * @remark The returned value is the square of the distance. The caller may
	 *         apply square root on the returned value to get the actual
	 *         distance, but they may not need to; if, for example, they need
	 *         to check if the distance is less than some value, a more
	 *         efficient solution would be to square the value instead of
	 *         doing costly sqrt operation.
	 */
	constexpr T sqrDistance(const PointGeneric<T>& rhs) const {
		return sqr(this->x - rhs.x) + sqr(this->y - rhs.y);
	}

	constexpr bool operator== (const PointGeneric<T>& rhs) const {
		return (this->x == rhs.x && this->y == rhs.y);
	}

	constexpr PointGeneric<T> operator+ (const PointGeneric<T>& rhs) const {
		PointGeneric<T> res = *this;
		res.offset(rhs);
		return res;
	}

	constexpr PointGeneric<T> operator- (const PointGeneric<T>& rhs) const {
		PointGeneric<T> res = this->relativeTo(rhs);
		return res;
	}

	/**
	 * @brief Casts point to another point type.
	 * 
	 * @tparam TT Scalar type. Value type of the other point.
	 */
	template <typename TT>
	explicit constexpr operator PointGeneric<TT>() const {
		return PointGeneric<TT>(
			static_cast<TT>(x),
			static_cast<TT>(y)
		);
	}
};

/**
 * @brief Structure representing X and Y coordinate (integral).
 */
typedef PointGeneric<int> Point;
/**
 * @brief Structure representing X and Y coordinate (floating point).
 */
typedef PointGeneric<double> PointF;

/**
 * @brief Width and height structure.
 * 
 * @tparam T Scalar type.
 */
template <typename T>
struct Size2dGeneric {
	typedef T ValueType;

	T w;
	T h;

	constexpr Size2dGeneric(T w, T h)
		: w{w}
		, h{h}
	{}

	constexpr Size2dGeneric()
		: Size2dGeneric(0, 0)
	{}

	/**
	 * @brief Adds up two Size2d objects.
	 * 
	 * @return A new Size2dGeneric which's width is the sum of the widths of
	 *         the two operands (same for height).
	 */
	constexpr Size2dGeneric<T> operator+ (const Size2dGeneric<T>& rhs) const {
		return Size2dGeneric<T>(this->w + rhs.w, this->h + rhs.h);
	}
	/**
	 * @brief Negates a Size2dGeneric object.
	 * 
	 * @return A new Size2dGeneric which's each dimension is negation of that
	 *         dimension in the operand.
	 */
	constexpr Size2dGeneric<T> operator- () const {
		return Size2dGeneric<T>(-w, -h);
	}
	/**
	 * @brief Subtracts two Size2dGeneric objects.
	 * 
	 * @return A new Size2dGeneric which's width is the difference of the widths
	 *         of the two operands (same for height).
	 */
	constexpr Size2dGeneric<T> operator- (const Size2dGeneric<T>& rhs) const {
		return *this + (-rhs);
	}
	/**
	 * @brief Increments a Size2dGeneric object.
	 * 
	 * @return Reference to self which's width has been incremented by the width
	 *         of the operand (same for height).
	 */
	constexpr Size2dGeneric<T>& operator+= (const Size2dGeneric<T>& rhs) {
		return (*this = *this + rhs);
	}
	/**
	 * @brief Decrements a Size2dGeneric object.
	 * 
	 * @return Reference to self which's width has been decremented by the width
	 *         of the operand (same for height).
	 */
	constexpr Size2dGeneric<T>& operator-= (const Size2dGeneric<T>& rhs) {
		return (*this = *this - rhs);
	}
	/**
	 * @brief Compares two sizes for equality.
	 */
	constexpr bool operator== (const Size2dGeneric<T>& rhs) const {
		return (this->w == rhs.w)
			&& (this->h == rhs.h);
	}

	/**
	 * @brief Casts size2d to another size2d type.
	 * 
	 * @tparam TT Scalar type. Value type of the other size2d.
	 */
	template <typename TT>
	explicit constexpr operator Size2dGeneric<TT>() const {
		return Size2dGeneric<TT>(
			static_cast<TT>(w),
			static_cast<TT>(h)
		);
	}
};

/**
 * @brief Width and height structure (integral).
 */
typedef Size2dGeneric<int> Size2d;
/**
 * @brief Width and height structure (floating point).
 */
typedef Size2dGeneric<double> Size2dF;

/**
 * @brief The axis-aligned rectangle type.
 * 
 * @tparam T Scalar type.
 */
template <typename T>
struct RectGeneric {
	typedef T ValueType;

	/**
	 * @brief X coordinate of the left side of the rectangle.
	 */
	T x;
	/**
	 * @brief Y coordinate of the left side of the rectangle.
	 */
	T y;
	/**
	 * @brief Width of the rectangle.
	 */
	T w;
	/**
	 * @brief Height of the rectangle.
	 */
	T h;

	constexpr RectGeneric()
		: RectGeneric(0, 0, 0, 0)
	{}
	constexpr RectGeneric(T x, T y, T w, T h)
		: x{x}
		, y{y}
		, w{w}
		, h{h}
	{}
	constexpr RectGeneric(T x, T y, const Size2dGeneric<T>& size)
		: RectGeneric(x, y, size.w, size.h)
	{}
	constexpr RectGeneric(const PointGeneric<T>& topLeft, const Size2dGeneric<T>& size)
		: RectGeneric(topLeft.x, topLeft.y, size.w, size.h)
	{}
	constexpr RectGeneric(const PointGeneric<T>& topLeft, const PointGeneric<T>& bottomRight)
		: RectGeneric(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y)
	{}
	static constexpr RectGeneric<T> createEmpty() { return RectGeneric<T>(); }

	/**
	 * @brief Get X coordinate of the right side of the rectangle.
	 */
	constexpr T getRight() const { return x + w; }
	/**
	 * @brief Get Y coordinate of the bottom side of the rectangle.
	 */
	constexpr T getBottom() const { return y + h; }
	/**
	 * @brief Get the structure containing the width and height of the
	 *        rectangle.
	 */
	constexpr Size2dGeneric<T> getSize() const { return Size2dGeneric<T>(w, h); }
	/**
	 * @brief Get the structure containing the X and Y coordinate of the
	 *        rectangle.
	 */
	constexpr PointGeneric<T> getTopLeft() const { return PointGeneric<T>(x, y); }
	/**
	 * @brief Get the structure containing the X and Y coordinate of the
	 *        bottom right corner of the rectangle.
	 */
	constexpr PointGeneric<T> getBottomRight() const {
		return PointGeneric<T>(getRight(), getBottom());
	}
	/**
	 * @brief Get the structure containing the X and Y coordinate of the
	 *        point in the center of the rectangle.
	 */
	constexpr PointGeneric<T> getCenterPoint() const {
		return PointGeneric<T>(x + w/2, y + h/2);
	}

	constexpr void setPos(T x, T y) {
		this->x = x;
		this->y = y;
	}
	constexpr void setPos(const PointGeneric<T>& pos) {
		setPos(pos.x, pos.y);
	}

	/**
	 * @brief Unite rectangle with another.
	 * 
	 * @details A union of rectangles in 2D space may result in an octagonal
	 *          shape. In order to make things simple for us, let's asume the
	 *          union "the smallest rectangle that both rectangles fit into".
	 */
	constexpr void unionRect(const RectGeneric<T>& rhs) {
		this->x = (this->x < rhs.x ? this->x : rhs.x);
		this->y = (this->y < rhs.y ? this->y : rhs.y);
		this->w = (this->getRight() > rhs.getRight() ? this->getRight() : rhs.getRight()) - x;
		this->h = (this->getBottom() > rhs.getBottom() ? this->getBottom() : rhs.getBottom()) - y;
	}

	/**
	 * @brief Get the union of two rectangles.
	 * 
	 * @details A union of rectangles in 2D space may result in an octagonal
	 *          shape. In order to make things simple for us, let's asume the
	 *          union "the smallest rectangle that both rectangles fit into".
	 */
	constexpr RectGeneric<T> getunionRect(const RectGeneric<T>& rhs) const {
		RectGeneric<T> res(*this);
		res.unionRect(rhs);
		return res;
	}

	/**
	 * @brief Returns true if a point is within the bounds of the rectangle.
	 * 
	 * @remark Inclusive, i.e., returns true even if it lies on the rectangle
	 *         bound.
	 */
	constexpr bool containsPoint(const PointGeneric<T>& pt) const {
		return (x <= pt.x && pt.x <= getRight()) && (y <= pt.y && pt.y <= getBottom());
	}

	/**
	 * @brief Returns true if the rectangle has no area.
	 */
	constexpr bool isEmpty() const {
		return (this->w <= 0 || this->h <= 0);
	}

	/**
	 * @brief Centers the rectangle within the area of `rhs`.
	 */
	constexpr void centerAt(const RectGeneric<T>& rhs) {
		x = rhs.x + ((rhs.w - this->w) / 2);
		y = rhs.y + ((rhs.h - this->h) / 2);
	}
	/**
	 * @brief Returns a copy of this rectangle centered within `rhs`.
	 */
	constexpr RectGeneric<T> getCenteredAt(const RectGeneric<T>& rhs) const {
		RectGeneric<T> res(*this);
		res.centerAt(rhs);
		return res;
	}

	constexpr void offset(T x, T y) {
		this->x += x;
		this->y += y;
	}
	constexpr void offset(const PointGeneric<T>& offsetPoint) {
		offset(offsetPoint.x, offsetPoint.y);
	}

	/**
	 * @brief Transforms rectangle using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 * 
	 * @remark Note that the result might not be as expected, because the
	 *         rectangle must remain axis-aligned. If the transformation
	 *         involves rotation, shearing, or other transformations that
	 *         affect the angle of the rectangle sides, consider converting
	 *         the rectangle to polygon type.
	 */
	constexpr void transform(const Matrix3x3& tm) {
		PointGeneric<T> topLeft = getTopLeft();
		PointGeneric<T> bottomRight = getBottomRight();
		
		topLeft.transform(tm);
		bottomRight.transform(tm);

		x = topLeft.x;
		y = topLeft.y;
		w = bottomRight.x - topLeft.x;
		h = bottomRight.y - topLeft.y;
	}
	/**
	 * @brief Transforms rectangle using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 * 
	 * @remark Note that the result might not be as expected, because the
	 *         rectangle must remain axis-aligned. If the transformation
	 *         involves rotation, shearing, or other transformations that
	 *         affect the angle of the rectangle sides, consider converting
	 *         the rectangle to polygon type.
	 */
	constexpr RectGeneric<T> getTransformed(const Matrix3x3& tm) const {
		RectGeneric<T> res(*this);
		res.transform(tm);
		return res;
	}

	/**
	 * @brief Changes the rectangle size from its center.
	 * 
	 * @details The returned rectangle will have its width and height changed
	 *          by `dx` and `dy` respectively, and the X and Y changed by
	 *          `-dx/2` and `-dy/2` respectively.
	 * 
	 * @param dx Increment in X axis.
	 * @param dy Increment in Y axis.
	 */
	constexpr RectGeneric<T> getInflated(T dx, T dy) const {
		RectGeneric<T> res(
			x - (dx/2),
			y - (dy/2),
			w + dx,
			h + dy
		);
		return res;
	}

	/**
	 * @brief Changes the rectangle size from its center.
	 * 
	 * @details The returned rectangle will have both its width and height
	 *          changed by `d`, and both the X and Y changed by `-d/2`.
	 * 
	 * @param d Increment in both X and Y axis.
	 */
	constexpr RectGeneric<T> getInflated(T d) const {
		return getInflated(d, d);
	}

	/**
	 * @brief Alias for rectangle union.
	 */
	constexpr RectGeneric<T> operator+ (const RectGeneric<T>& rhs) const {
		return getUnionRect(rhs);
	}

	/**
	 * @brief Unions the rectangle with another.
	 * 
	 * @return The reference to self.
	 */
	constexpr RectGeneric<T>& operator+= (const RectGeneric<T>& rhs) {
		unionRect(rhs);
		return *this;
	}

	constexpr bool operator== (const RectGeneric<T>& rhs) {
		return (x == rhs.x)
			&& (y == rhs.y)
			&& (w == rhs.w)
			&& (h == rhs.h);
	}

	/**
	 * @brief Casts rectangle to another rectangle type.
	 * 
	 * @tparam TT Scalar type. Value type of the other rectangle.
	 */
	template <typename TT>
	explicit constexpr operator RectGeneric<TT>() const {
		return RectGeneric<TT>(
			static_cast<TT>(x),
			static_cast<TT>(y),
			static_cast<TT>(w),
			static_cast<TT>(h)
		);
	}
};

/**
 * @brief The axis-aligned rectangle type (integral).
 */
typedef RectGeneric<int> Rect;
/**
 * @brief The axis-aligned rectangle type (floating point).
 */
typedef RectGeneric<double> RectF;

/**
 * @brief Three-corner polygon (floating point coordinates).
 */
struct TriangleF {
	typedef PointF CornerType;
	typedef CornerType::ValueType ValueType;

	// In case you didn't know, triangle has 3 corners...
	static constexpr int CORNER_COUNT = 3;

	std::array<CornerType, CORNER_COUNT> corners;

	constexpr TriangleF(const CornerType& pA, const CornerType& pB, const CornerType& pC)
		: corners{pA, pB, pC}
	{}

	constexpr TriangleF(const CornerType (&corners)[3])
		: TriangleF(corners[0], corners[1], corners[2])
	{}

	constexpr TriangleF()
		: TriangleF(CornerType::zero(), CornerType::zero(), CornerType::zero())
	{}

	/**
	 * @brief Transforms triangle using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 */
	constexpr void transform(const Matrix3x3& tm) {
		for (int i = 0; i < CORNER_COUNT; i++) {
			corners[i].transform(tm);
		}
	}

	/**
	 * @brief Transforms triangle using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 */
	constexpr TriangleF getTransformed(const Matrix3x3& tm) const {
		TriangleF res(*this);
		res.transform(tm);
		return res;
	}
};

/**
 * @brief Polygon (floating point coordinates).
 */
struct PolygonF {
	typedef PointF CornerType;
	typedef CornerType::ValueType ValueType;
	typedef std::vector<CornerType> CollectionType;

	CollectionType corners;

	constexpr PolygonF() {}

	/**
	 * @brief Constructs a new PolygonF object from the provided list
	 *        of corners.
	 * 
	 * @tparam Args Must be `PointF`.
	 * @param corners An arbitrary number of `PointF` values.
	 */
	template <typename... Args>
	PolygonF(const Args&... corners) {
		this->corners.reserve(sizeof...(corners));
		// "Fold expression" -- for each arg do...
		// See https://en.cppreference.com/w/cpp/language/fold
		(this->corners.push_back(corners), ...);
	}

	/**
	 * @brief Constructs a new PolygonF object from the provided list
	 *        of corners.
	 * 
	 * @tparam Args Must meet the named requirement "SequenceContainer"
	 *              and the value type must be `PointF`.
	 */
	template <typename T>
	PolygonF(const T& corners) {
		this->corners.reserve(corners.size());
		for (auto& corner : corners) {
			this->corners.push_back(corner);
		}
	}

	constexpr PolygonF(CollectionType&& corners)
		: corners{std::move(corners)}
	{}

	/**
	 * @brief Constructs a new PolygonF object from `TriangleF` object.
	 */
	PolygonF(const TriangleF& triangle) {
		corners.reserve(triangle.CORNER_COUNT);
		for (auto& corner : triangle.corners) {
			corners.push_back(corner);
		}
	}

	/**
	 * @brief Checks whether a polygon is valid in Euclidean plane.
	 * 
	 * @details A valid polygon in Euclidean plane has at least 3 corners.
	 */
	constexpr bool isValidEuclidean() const {
		return (cornerCount() >= 3);
	}

	constexpr size_t cornerCount() const {
		return corners.size();
	}

	/**
	 * @brief Returns a list of the polygon edges.
	 */
	constexpr std::vector<std::pair<CornerType, CornerType>> getEdges() const {
		std::vector<std::pair<CornerType, CornerType>> res;

		if (cornerCount() > 1) {
			res.reserve(cornerCount());
			for (size_t cornIdx = 0; cornIdx < cornerCount(); cornIdx++) {
				auto& p0 = corners[cornIdx];
				auto& p1 = corners[(cornIdx + 1) % cornerCount()];
				res.push_back(std::make_pair(p0, p1));
			}
		}

		return res;
	}

	/**
	 * @brief Returns true if a point is within the bounds of the polygon.
	 * 
	 * @remark Inclusive, i.e., returns true even if it lies on the polygon
	 *         edge.
	 */
	constexpr bool containsPoint(const CornerType& pt) const {
		// Ray casting algorithm -- cast a ray in any direction starting at the
		// point, then count the number of intersections with the polygon
		// borders. Odd count means that the point is inside the polygon, even
		// means it's outside.
		//
		//     +----+
		//    /     '------------+
		//    \        +--+      |
		// <~~~\~~~~~~/~~~~\~X   |
		//      +----+      +----+
		//
		//   3 intersections => inside
		//
		// To make things easier, instead of ray we will use a line, then count
		// the intersections only in one direction from the point. The line
		// will be parallel to the X axis.

		if (!isValidEuclidean()) {
			return false;
		}

		int intersectCount = 0;

		for (auto edge : getEdges()) {
			// Get the line segment of the edge
			CornerType& p0 = edge.first;
			CornerType& p1 = edge.second;
			// Order the endpoints by Y coordinate
			// `p0.y <= p1.y`
			if (p1.y < p0.y) {
				std::swap(p0, p1);
			}

			if (p0.y <= pt.y && pt.y < p1.y) {
				// The line intersects this edge. Also, the line and the edge
				// are not collinear, because the edge is not parallel to the
				// Y axis -- `p0.y < p1.y`

				// v1 = p1 - p0
				// v2 = pt - p0
				// Make both vectors 3D with the Z coordinate = 0.
				// v3 = v1 x v2 (cross product)
				// v3 = (x, y, z) = (0, 0, (v1.x * v2.y) - (v1.y * v2.x))
				// If z > 0, then the point is "to the right" of the line
				// segment (edge). If z = 0, the point lies on it.

				CornerType v1 = p1 - p0;
				CornerType v2 = pt - p0;
				ValueType crossProduct = (v1.x * v2.y) - (v1.y * v2.x);

				if (crossProduct >= 0) {
					intersectCount++;
				}
			}
		}

		return isOdd(intersectCount);
	}

	/**
	 * @brief Triangulates a polygon (converts it to a collection of triangles).
	 * 
	 * @remark The polygon must be simple, i.e., without self-intersecting
	 *         edges.
	 * 
	 * @param result The generated triangles.
	 */
	void triangulate(std::vector<TriangleF>& result) {
		// Invalid polygon cannot be triangulated
		assert(isValidEuclidean());
		// We want to create a new "result"
		assert(result.empty());

		std::vector<std::array<double,2>> inPolygon;
		std::array<double,2> polygonPt;
		std::vector<std::array<std::array<double,2>,3>> outTriangles;

		// Fill vector for triangulatePolygon()
		inPolygon.reserve(cornerCount());
		for (auto& corner : corners) {
			polygonPt[0] = corner.x;
			polygonPt[1] = corner.y;
			inPolygon.push_back(std::move(polygonPt));
		}

		// Triangulate
		triangulatePolygon(inPolygon, outTriangles);

		PointF corners[3];
		// Copy to output vector
		for (auto& triangle : outTriangles) {
			// For each triangle corner...
			for (int i = 0; i < 3; i++) {
				corners[i].x = triangle[i][0];
				corners[i].y = triangle[i][1];
			}
			result.push_back(TriangleF(corners));
		}
	}

	/**
	 * @brief Returns the smallest rectangle that can be formed around the
	 *        polygon.
	 */
	constexpr RectF getBoundingBox() const {
		PointF topLeft = corners[0];
		PointF bottomRight = corners[0];

		// This algorithm is basically about finding the minima and maxima in
		// both X and Y dimensions.
		for (size_t i = 1; i < cornerCount(); i++) {
			const PointF& corner = corners[i];

			if (topLeft.x > corner.x) topLeft.x = corner.x;
			if (topLeft.y > corner.y) topLeft.y = corner.y;
			if (bottomRight.x < corner.x) bottomRight.x = corner.x;
			if (bottomRight.y < corner.y) bottomRight.y = corner.y;
		}

		return RectF(topLeft, bottomRight);
	}

	/**
	 * @brief Set the polygon location.
	 * 
	 * @details The polygon location is determined by the top left corner
	 *          of its bounding box.
	 */
	constexpr void setLocation(ValueType x, ValueType y) {
		// Determine its current location
		PointF topLeft = getBoundingBox().getTopLeft();

		// Calculate the delta
		ValueType dx = x - topLeft.x;
		ValueType dy = y - topLeft.y;

		// Move every corner
		for (auto& corner : corners) {
			corner.x += dx;
			corner.y += dy;
		}
	}
	/**
	 * @brief Set the polygon location.
	 * 
	 * @details The polygon location is determined by the top left corner
	 *          of its bounding box.
	 */
	constexpr void setLocation(const PointF& p) {
		setLocation(p.x, p.y);
	}

	/**
	 * @brief Transforms polygon using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 */
	constexpr void transform(const Matrix3x3& tm) {
		for (size_t i = 0; i < cornerCount(); i++) {
			corners[i].transform(tm);
		}
	}

	/**
	 * @brief Transforms polygon using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 */
	constexpr PolygonF getTransformed(const Matrix3x3& tm) const {
		PolygonF res(*this);
		res.transform(tm);
		return res;
	}

	/**
	 * @brief Returns the squared distance between this polygon's bounds and
	 *        a point.
	 * 
	 * @remark The returned value is the square of the distance. The caller may
	 *         apply square root on the returned value if they need to.
	 */
	constexpr ValueType sqrDistanceBounds(const PointF& pt) const {
		ValueType minDist = INFINITY, dist;
		for (auto edge : getEdges()) {
			CornerType& p0 = edge.first;
			CornerType& p1 = edge.second;

			dist = sqrDistancePointLineSegment(pt.x, pt.y, p0.x, p0.y, p1.x,
				p1.y);
			
			if (minDist > dist) {
				minDist = dist;
			}
		}

		return minDist;
	}

	/**
	 * @brief Returns the squared distance between this polygon and a point.
	 * 
	 * @remark The distance is 0 if the point lies inside the polygon bounds.
	 * 
	 * @remark The returned value is the square of the distance. The caller may
	 *         apply square root on the returned value if they need to.
	 */
	constexpr ValueType sqrDistance(const PointF& pt) const {
		return (containsPoint(pt) ? 0 : sqrDistanceBounds(pt));
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
	 * @brief Constructs a transparent color.
	 */
	constexpr Color()
		: Color(0, 0, 0, 0)
	{}
	/**
	 * @brief Duplicates a color.
	 */
	constexpr Color(const Color& other)
		: Color(other.r, other.g, other.b, other.a)
	{}
	/**
	 * @brief Constructs a color.
	 * 
	 * @param r Intensity of red channel.
	 * @param g Intensity of green channel.
	 * @param b Intensity of blue channel.
	 * @param a Intensity of alpha channel.
	 */
	constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r{r}
		, g{g}
		, b{b}
		, a{a}
	{}
	/**
	 * @brief Constructs an opaque color.
	 * 
	 * @param r Intensity of red channel.
	 * @param g Intensity of green channel.
	 * @param b Intensity of blue channel.
	 */
	constexpr Color(uint8_t r, uint8_t g, uint8_t b)
		: Color(r, g, b, 0xff)
	{}
	/**
	 * @brief Constructs a color.
	 * 
	 * @param rgba The color stored as 32bit number with red channel being the
	 *             most significant byte and alpha channel the least significant
	 *             byte.
	 */
	static constexpr Color rgba(uint32_t rgba) {
		Color res((rgba >> 24) & 0xff, (rgba >> 16) & 0xff, (rgba >> 8) & 0xff,
			rgba & 0xff);
		return res;
	}
	/**
	 * @brief Constructs an opaque color.
	 * 
	 * @param rgb The color stored as 32bit number with red channel being the
	 *            second most significant byte and alpha channel the least
	 *            significant byte. The MSB is ignored.
	 */
	static constexpr Color rgb(uint32_t rgb) {
		Color res = Color::rgba((rgb << 8) | 0xff);
		return res;
	}
	static constexpr Color black() { return Color::rgb(0); }
	static constexpr Color white() { return Color::rgb(0xffffff); }
	static constexpr Color red() { return Color::rgb(0xff0000); }
	static constexpr Color green() { return Color::rgb(0x00ff00); }
	static constexpr Color blue() { return Color::rgb(0x0000ff); }
	static constexpr Color cyan() { return Color::rgb(0x00ffff); }
	static constexpr Color magenta() { return Color::rgb(0xff00ff); }
	static constexpr Color yellow() { return Color::rgb(0xffff00); }
	static constexpr Color darkRed() { return Color::rgb(0x8b0000); }
	static constexpr Color skyBlue() { return Color::rgb(0x87ceeb); }
	static constexpr Color grayscale(uint8_t intensity) {
		return Color(intensity, intensity, intensity);
	}
	static constexpr Color bad() { return Color::darkRed(); }
	static constexpr Color playerDefault() { return Color::red(); }
	static constexpr Color player(unsigned id) {
		switch (id) {
			case 0: return Color::playerDefault();
			case 1: return Color::green();
			case 2: return Color::skyBlue();
			case 3: return Color::magenta();
			default: return Color::playerDefault();
		}
	}
	static constexpr Color obstacle() { return Color::grayscale(0x80); }
	static constexpr uint8_t GHOST_ALPHA = 0xa4;
	// Used in editor for brushes, dragged objects, etc.
	static constexpr Color ghost() { return Color::white().setAlpha(GHOST_ALPHA); }
	// Used in editor for brushes, dragged objects, etc.
	static constexpr Color badGhost() { return Color::bad().setAlpha(GHOST_ALPHA); }

	constexpr bool operator== (const Color& rhs) const {
		return
			(this->r == rhs.r) &&
			(this->g == rhs.g) &&
			(this->b == rhs.b) &&
			(this->a == rhs.a);
	}

	constexpr Color& operator= (const Color&) = default;

	static constexpr uint8_t modulateChannel(uint8_t ch, uint8_t mod) {
		return (static_cast<unsigned>(ch) * static_cast<unsigned>(mod)) / 0xffU;
	}

	constexpr Color getModulated(const Color& mod) const {
		return Color(
			modulateChannel(this->r, mod.r),
			modulateChannel(this->g, mod.g),
			modulateChannel(this->b, mod.b),
			modulateChannel(this->a, mod.a)
		);
	}

	/**
	 * @brief Modifies the alpha channel of the color.
	 * 
	 * @return Reference to self.
	 * 
	 * @remark Useful when creating a predefined color using one of the static
	 *         methods and passing it as a parameter to a function. That way
	 *         you don't need to create a temporary variable and pass the value
	 *         straight away.
	 * 
	 * @example `canvas->setFillingColor(Color::red().setAlpha(0x80))`
	 */
	constexpr Color& setAlpha(uint8_t alpha) {
		this->a = alpha;
		return *this;
	}
};

/**
 * @brief Simple timer implementation.
 * 
 * @details The timer doesn't produce events. Instead, it must be queried when
 *          needed.
 * 
 *          The timer's interval may not be modified. Instead, a new timer
 *          instance must be created.
 * 
 * @remark The timer may overflow.
 */
class Timer {
private:
	std::clock_t m_interval;
	std::clock_t m_nextTimestamp;
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
	/**
	 * @brief Returns the timer interval.
	 */
	std::clock_t getInterval() const { return m_interval; }
};

/**
 * @brief Continuous animation class.
 * 
 * @details This class may be used for animations like transformations, color
 *          changing, etc. It uses "relative time" for time measurement, meaning
 *          it measures the running time relative to its duration.
 * 
 *          Example: An animation is 16 ms long. If the measured time is equal
 *          to 0.5, the animation has been running for 8 ms. 0.25 means it's
 *          been running for 4 ms, etc.
 * 
 *          Example usage: Circle radius should grow from 20 px to 45 px for
 *          10 ms. Construct the object using `AnimationContinuous(10)`. To
 *          calculate the immediate radius use the equation:
 *              `r(t) = r(0) + t * delta(r)`
 *          that is:
 *              `r(t) = 20 + t * 25`
 *          At t=0 the radius will be 20 px, at t=1 the radius will be 45 px.
 */
class AnimationContinuous {
private:
	std::clock_t m_start;
	double m_duration;
public:
	/**
	 * @brief Constructs a new AnimationContinuous object.
	 * 
	 * @param duration Animation duration in milliseconds.
	 */
	AnimationContinuous(double duration)
		: m_duration{msToClocks(duration)}
	{
		reset();
	}

	/**
	 * @brief Resets the animation.
	 */
	void reset() {
		m_start = std::clock();
	}
	/**
	 * @brief Get the relative running time.
	 */
	double getTime() const {
		std::clock_t runTime = std::clock() - m_start;
		return static_cast<double>(runTime) / m_duration;
	}
	/**
	 * @brief Check if the animation has finished.
	 */
	bool isCompleted() const {
		return (getTime() >= 1.0);
	}
};


template <typename T>
constexpr T msToClocks(T ms) {
	// ms / 1000 * CPS -> convert ms to s, then convert s to clocks.
	// The operands are swapped here to avoid unnecessary FP conversions
	// but keep precision
	return ms * CLOCKS_PER_SEC / 1000;
}

#endif // TYPES_HPP
