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
	IMG_ICON_SELECT_TOOL,
	IMG_ICON_PLAYER_TOOL,
	IMG_ICON_OBSTACLE_TOOL,
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
		Matrix3x3::ValueType h;

		resF.x = this->x * tm[0][0];
		resF.y = this->x * tm[0][1];
		h      = this->x * tm[0][2];
		resF.x += this->y * tm[1][0];
		resF.y += this->y * tm[1][1];
		h      += this->y * tm[1][2];
		resF.x += tm[2][0];
		resF.y += tm[2][1];
		h      += tm[2][2];

		resF.x /= h;
		resF.y /= h;

		*this = static_cast<PointGeneric<T>>(resF);
	}

	/**
	 * @brief Returns the point transformed using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 */
	constexpr PointGeneric<T> getTransformed(const Matrix3x3& tm) const {
		PointGeneric<Matrix3x3::ValueType> res(*this);
		res.transform(tm);
		return res;
	}

	constexpr bool operator== (const PointGeneric<T>& rhs) const {
		return (this->x == rhs.x && this->y = rhs.y);
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
	typedef PointF::ValueType ValueType;

	// In case you didn't know, triangle has 3 corners...
	static constexpr int CORNER_COUNT = 3;

	std::array<PointF, CORNER_COUNT> corners;

	constexpr TriangleF(const PointF& pA, const PointF& pB, const PointF& pC)
		: corners{pA, pB, pC}
	{}

	constexpr TriangleF(const PointF (&corners)[3])
		: TriangleF(corners[0], corners[1], corners[2])
	{}

	constexpr TriangleF()
		: TriangleF(PointF::zero(), PointF::zero(), PointF::zero())
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
	typedef PointF::ValueType ValueType;

	std::vector<PointF> corners;

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

	constexpr PolygonF(std::vector<PointF>&& corners)
		: corners{corners}
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

		// This algorithm as basically about finding the minima and maxima in
		// both X and Y dimensions.
		for (int i = 1; i < cornerCount(); i++) {
			const PointF& corner = corners[i];

			if (topLeft.x > corner.x) topLeft.x = corner.x;
			if (topLeft.y > corner.y) topLeft.y = corner.y;
			if (bottomRight.x < corner.x) bottomRight.x = corner.x;
			if (bottomRight.y < corner.y) bottomRight.y = corner.y;
		}

		return RectF(topLeft, bottomRight);
	}

	/**
	 * @brief Transforms polygon using transformation matrix.
	 * 
	 * @param tm Transformation matrix.
	 */
	constexpr void transform(const Matrix3x3& tm) {
		for (int i = 0; i < cornerCount(); i++) {
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

	static constexpr Color black() { return Color(0, 0, 0); }
	static constexpr Color white() { return Color(0xff, 0xff, 0xff); }
	static constexpr Color red() { return Color(0xff, 0, 0); }
	static constexpr Color green() { return Color(0, 0xff, 0); }
	static constexpr Color blue() { return Color(0, 0, 0xff); }

	constexpr bool operator== (const Color& rhs) const {
		return
			(this->r == rhs.r) &&
			(this->g == rhs.g) &&
			(this->b == rhs.b) &&
			(this->a == rhs.a);
	}

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
