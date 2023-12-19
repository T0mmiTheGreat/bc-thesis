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

	Size2d getSize() override;
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
	void startAnimation() override;
	void stopAnimation() override;
	void frameEvent() override;

	const std::string& getText() const;
	void setText(const std::string& value);
	AnimationType getAnimationType() const;
	void setAnimationType(AnimationType value);
	void mouseMoveEvent(int x, int y);
};

#endif // MAINMENUITEMSPRITE_HPP
