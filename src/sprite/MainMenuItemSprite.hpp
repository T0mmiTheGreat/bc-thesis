/**
 * @file MainMenuItemSprite.hpp
 * @author Tomáš Ludrovan
 * @brief MainMenuItemSprite class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAINMENUITEMSPRITE_HPP
#define MAINMENUITEMSPRITE_HPP

#include <string>

#include "sprite/SpriteBase.hpp"

/**
 * @brief Sprite for main menu items.
 * 
 * @details On hover logic is also implemented here.
 */
class MainMenuItemSprite : public SpriteBase {
public:
	enum AnimationType {
		ANIM_GROW,
		ANIM_SHRINK,
	};
private:
	const int RECT_WIDTH_NORMAL = 165;
	const int RECT_HEIGHT_NORMAL = 35;
	const double MAX_SCALE = 1.1;
	const int RECT_WIDTH_MAX = static_cast<int>(RECT_WIDTH_NORMAL * MAX_SCALE);
	const int RECT_HEIGHT_MAX = static_cast<int>(RECT_HEIGHT_NORMAL * MAX_SCALE);
	const unsigned GROWTH_FRAME_COUNT = 4;
	const unsigned SHRINK_FRAME_COUNT = GROWTH_FRAME_COUNT;
	const FontId FONT = FONT_BRLNSTR_20;

	Timer m_timer;
	double m_scale;
	int m_frameNumber;
	std::string m_text;
	AnimationType m_animationType;
public:
	MainMenuItemSprite();

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

	/**
	 * @brief Getter for the `m_text`.
	 */
	const std::string& getText() const;
	/**
	 * @brief Setter for the `m_text`.
	 */
	void setText(const std::string& value);
	/**
	 * @brief Getter for the `m_animationType`.
	 */
	AnimationType getAnimationType() const;
	/**
	 * @brief Setter for the `m_animationType`.
	 */
	void setAnimationType(AnimationType value);
	/**
	 * @brief A mouse cursor moved.
	 * 
	 * @param x The x coordinate. 0 is the left border of the window. The
	 *          coordinate increases to the right.
	 * @param y The y coordinate. 0 is the top border of the window. The
	 *          coordinate increases downwards.
	 */
	void mouseMoveEvent(int x, int y);
};

#endif // MAINMENUITEMSPRITE_HPP
