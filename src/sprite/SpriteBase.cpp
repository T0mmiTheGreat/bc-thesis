/**
 * @file SpriteBase.cpp
 * @author Tomáš Ludrovan
 * @brief SpriteBase class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/SpriteBase.hpp"

SpriteBase::SpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy)
	: paintingProxy{paintingProxy}
{}
