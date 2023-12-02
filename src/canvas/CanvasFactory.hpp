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

class CanvasFactory {
public:
	static std::shared_ptr<ICanvas> createSdlCanvas();
};

#endif // CANVASFACTORY_HPP
