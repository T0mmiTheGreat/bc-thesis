/**
 * @file MainMenuSprite.hpp
 * @author Tomáš Ludrovan
 * @brief MainMenuSprite class
 * @version 0.1
 * @date 2023-12-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAINMENUSPRITE_HPP
#define MAINMENUSPRITE_HPP

#include <array>
#include <memory>

#include "sprite/SpriteBase.hpp"
#include "sprite/MainMenuItemSprite.hpp"

class MainMenuSprite : public SpriteBase {
private:
	static const unsigned ITEM_SPACING = 10;
	static const size_t ITEM_COUNT = 2;

	MainMenuItemSprite m_playBtn;
	MainMenuItemSprite m_quitBtn;

	void posChangedEvent(int oldX, int oldY, int newX, int newY) override;
	std::array<MainMenuItemSprite*, ITEM_COUNT> getMenuItems();
public:
	MainMenuSprite();

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
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 */
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
	
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

#endif // MAINMENUSPRITE_HPP
