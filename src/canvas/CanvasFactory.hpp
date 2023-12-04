/**
 * @file CanvasFactory.hpp
 * @author Tomáš Ludrovan
 * @brief CanvasFactory class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CANVASFACTORY_HPP
#define CANVASFACTORY_HPP

#include <memory>

#include "canvas/ICanvas.hpp"

/**
 * @brief Factory for ICanvas interface.
 */
class CanvasFactory {
public:
	/**
	 * @brief Creates an instance of SDLCanvas.
	 */
	static std::shared_ptr<ICanvas> createSdlCanvas();
};

#endif // CANVASFACTORY_HPP
