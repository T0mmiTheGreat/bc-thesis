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

#include "sprite/SpriteBase.hpp"

class LogoSprite : public SpriteBase {
private:
	Timer m_timer;
	uint8_t m_opacity;
	int m_frameNumber;
public:
	LogoSprite();

	int getW() override;
	int getH() override;
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;

	void startAnimation() override;
	void stopAnimation() override;
	void frameEvent() override;
};

#endif // LOGOSPRITE_HPP
