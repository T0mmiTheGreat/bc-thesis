/**
 * @file LogoSprite.hpp
 * @author Tomáš Ludrovan
 * @brief LogoSprite class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include <string>

#include "types.hpp"
#include "sprite/SpriteBase.hpp"

class LogoSprite : public SpriteBase {
private:
	const std::string LOGO_TEXT = "Awesome Logo";
	const FontId LOGO_FONT = FONT_OLDENGL_72;

	Timer m_timer;
	uint8_t m_opacity;
	int m_frameNumber;
public:
	LogoSprite();

	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because this object may optimize it
	 *          to gain performance.
	 */
	Size2d getSize() override;
	/**
	 * @brief Starts a sprite animation.
	 * 
	 * @note If the method is called while an animation is running, the running
	 *       animation is stopped.
	 */
	void startAnimation() override;
	/**
	 * @brief Stops an animation if one is running.
	 * 
	 * @remarks No-op if animation is not running.
	 * 
	 *          It is NOT necessary to call this method after the animation
	 *          finishes. If it would be, the object itself should call it.
	 */
	void stopAnimation() override;
	/**
	 * @brief Event that happens every frame.
	 * 
	 * @details This method is mainly used to progress any running animations.
	 */
	void frameEvent() override;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 */
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // LOGOSPRITE_HPP
