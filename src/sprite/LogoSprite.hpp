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

	Size2d getSize() override;
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;

	void startAnimation() override;
	void stopAnimation() override;
	void frameEvent() override;
};

#endif // LOGOSPRITE_HPP
