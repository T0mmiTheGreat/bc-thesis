/**
 * @file CanvasFactory.cpp
 * @author Tomáš Ludrovan
 * @brief CanvasFactory class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "canvas/CanvasFactory.hpp"

#include <memory>

#include "canvas/SDLCanvas.hpp"

std::shared_ptr<ICanvas> CanvasFactory::createSdlCanvas()
{
	return std::make_shared<SDLCanvas>();
}

