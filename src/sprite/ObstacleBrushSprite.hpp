/**
 * @file ObstacleBrushSprite.hpp
 * @author Tomáš Ludrovan
 * @brief ObstacleBrushSprite class
 * @version 0.1
 * @date 2024-02-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OBSTACLEBRUSHSPRITE_HPP
#define OBSTACLEBRUSHSPRITE_HPP

#include "types.hpp"
#include "sprite/BoundedSpriteBase.hpp"

/**
 * @brief Used as obstacle tool brush in stage editor.
 * 
 * @details The brush appears as a dashed line segment between the last placed
 *          corner of the constructed obstacle and the mouse cursor.
 */
class ObstacleBrushSprite : public BoundedSpriteBase {
public:
	enum Costume {
		COSTUME_NORMAL, // Obstacle edge may be placed
		COSTUME_BAD,    // Obstacle edge may not be placed
		COSTUME_DOT,    // Before starting a new obstacle. `p1` is ignored.
	};
private:
	static constexpr Color STROKE_NORMAL = Color::ghost();
	static constexpr Color STROKE_BAD = Color::badGhost();
	static constexpr Color FILL_DOT = STROKE_NORMAL;
	static constexpr int DOT_RADIUS = 6;

	Costume m_costume;
	Point m_p0;
	Point m_p1;

	void repaintLineColor(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect,
		const Color& color);
	void repaintNormal(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect);
	void repaintBad(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect);
	void repaintDot(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect);
public:
	ObstacleBrushSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	Rect getBounds() override;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect,
	 *         but it must modify the invalidRect value to mach the area it
	 *         painted.
	 */
	void repaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override;

	/**
	 * @brief Getter for the sprite costume.
	 */
	ObstacleBrushSprite::Costume getCostume() const;
	/**
	 * @brief Setter for the sprite costume.
	 */
	void setCostume(ObstacleBrushSprite::Costume value);
	/**
	 * @brief Getter for the origin of the obstacle edge.
	 */
	const Point& getP0() const;
	/**
	 * @brief Setter for the origin of the obstacle edge.
	 */
	void setP0(const Point& value);
	/**
	 * @brief Getter for the end point of the obstacle edge.
	 */
	const Point& getP1() const;
	/**
	 * @brief Setter for the end point of the obstacle edge.
	 */
	void setP1(const Point& value);
};

#endif // OBSTACLEBRUSHSPRITE_HPP
