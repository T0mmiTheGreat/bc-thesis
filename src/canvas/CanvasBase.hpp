/**
 * @file CanvasBase.hpp
 * @author Tomáš Ludrovan
 * @brief CanvasBase class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CANVASBASE_HPP
#define CANVASBASE_HPP

#include "canvas/ICanvas.hpp"
#include "types.hpp"

class CanvasBase : public ICanvas {
protected:
	Color fColor;
	Color sColor;
	double sWidth;
public:
	virtual Color getFillingColor() override;
	virtual Color getStrokingColor() override;
	virtual void setFillingColor(Color color) override;
	virtual void setStrokingColor(Color color) override;

	virtual double getStrokeWidth() override;
	virtual void setStrokeWidth(double value) override;

	virtual void drawEllipse(int x, int y, int rx, int ry) override;
	virtual void drawCircle(int x, int y, int r) override;
};

#endif // CANVASBASE_HPP
