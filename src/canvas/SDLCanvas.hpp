/**
 * @file SDLCanvas.hpp
 * @author Tomáš Ludrovan
 * @brief SDLCanvas class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SDLCANVAS_HPP
#define SDLCANVAS_HPP

#include "canvas/CanvasBase.hpp"

/**
 * @brief Concrete implementation of ICanvas for the SDL library.
 */
class SDLCanvas : public CanvasBase {
public:
	virtual int getWidth() override;
	virtual int getHeight() override;

	virtual void setFillingColor(Color color) override;
	virtual void setStrokingColor(Color color) override;

	virtual void fillEllipse(int x, int y, int rx, int ry) override;
	virtual void strokeEllipse(int x, int y, int rx, int ry) override;
	virtual void drawEllipse(int x, int y, int rx, int ry) override;

	virtual void fillCircle(int x, int y, int r) override;
	virtual void strokeCircle(int x, int y, int r) override;
	virtual void drawCircle(int x, int y, int r) override;
};

#endif // SDLCANVAS_HPP
