/**
 * @file AnimatedSpriteBase.cpp
 * @author Tomáš Ludrovan
 * @brief AnimatedSpriteBase class
 * @version 0.1
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/AnimatedSpriteBase.hpp"

AnimatedSpriteBase::AnimatedSpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, isAnimationRunningFlag{false}
{}

void AnimatedSpriteBase::startAnimation()
{
	if (isAnimationRunning()) {
		stopAnimation();
	}
	isAnimationRunningFlag = true;
}

void AnimatedSpriteBase::stopAnimation()
{
	isAnimationRunningFlag = false;
}

bool AnimatedSpriteBase::isAnimationRunning()
{
	return isAnimationRunningFlag;
}
