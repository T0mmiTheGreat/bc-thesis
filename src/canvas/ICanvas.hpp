/**
 * @file ICanvas.hpp
 * @author Tomáš Ludrovan
 * @brief ICanvas interface
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ICANVAS_HPP
#define ICANVAS_HPP

#include <string>

#include "types.hpp"

/**
 * @brief Library-independent canvas.
 * 
 * @remark The Y coordinate in drawing operations has the value of 0 at the
 *         canvas top border and grows towards the bottom border.
 */
class ICanvas {
public:
	virtual ~ICanvas() {}
	/**
	 * @brief Get width of the canvas in screen coordinates.
	 */
	virtual int getWidth() = 0;
	/**
	 * @brief Get height of the canvas in screen coordinates.
	 */
	virtual int getHeight() = 0;
	/**
	 * @brief Get the canvas bounds.
	 */
	virtual Rect getRect() = 0;

	/**
	 * @brief Get the current color used for filling operations.
	 */
	virtual Color getFillingColor() = 0;
	/**
	 * @brief Get the current color used for stroking operations.
	 */
	virtual Color getStrokingColor() = 0;
	/**
	 * @brief Set the color for use in filling operations.
	 */
	virtual void setFillingColor(Color color) = 0;
	/**
	 * @brief Set the color for use in stroking operations.
	 */
	virtual void setStrokingColor(Color color) = 0;

	/**
	 * @brief Get the stroke thickness in stroking operations.
	 */
	virtual double getStrokeWidth() = 0;
	/**
	 * @brief Set the stroke thickness for stroking operations.
	 */
	virtual void setStrokeWidth(double value) = 0;

	/**
	 * @brief Get the current color and alpha modulation of drawing operations.
	 */
	virtual Color getColorMod() = 0;
	/**
	 * @brief Set the current color and alpha modulation of drawing operations.
	 * 
	 * @details The modulation is calculated as `ch = ch * (cl / 255)`, where
	 *          `ch` is the color channel (r/g/b/a) and `cl` is the respective
	 *          color channel of `value`. The modulation applies to every
	 *          drawing operation (image, text, filling, ...).
	 *
	 *          A good convention is to reset the value once the painting is
	 *          done:
	 *          \code
	 *          {
	 *              tmp = getColorMod()
	 *              setColorMod(value)
	 *              ...
	 *              setColorMod(tmp)
	 *          }
	 *          \endcode
	 * 
	 */
	virtual void setColorMod(const Color& value) = 0;

	/**
	 * @brief Draw a line.
	 * 
	 * @param x0 X coordinate of the initial point.
	 * @param y0 Y coordinate of the initial point.
	 * @param x1 X coordinate of the end point.
	 * @param y1 Y coordinate of the end point.
	 */
	virtual void strokeLine(int x0, int y0, int x1, int y1) = 0;
	/**
	 * @brief Draw a line using dashed pattern.
	 * 
	 * @param x0 X coordinate of the initial point.
	 * @param y0 Y coordinate of the initial point.
	 * @param x1 X coordinate of the end point.
	 * @param y1 Y coordinate of the end point.
	 */
	virtual void dashedLine(int x0, int y0, int x1, int y1) = 0;

	/**
	 * @brief Fill an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void fillEllipse(int x, int y, int rx, int ry) = 0;
	/**
	 * @brief Stroke an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void strokeEllipse(int x, int y, int rx, int ry) = 0;
	/**
	 * @brief Fill and stroke an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void drawEllipse(int x, int y, int rx, int ry) = 0;

	/**
	 * @brief Fill a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void fillCircle(int x, int y, int r) = 0;
	/**
	 * @brief Stroke a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void strokeCircle(int x, int y, int r) = 0;
	/**
	 * @brief Fill and stroke a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void drawCircle(int x, int y, int r) = 0;
	/**
	 * @brief Stroke a circular area using dashed pattern.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void dashedCircle(int x, int y, int r) = 0;
	
	/**
	 * @brief Fill a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void fillRectangle(int x, int y, int w, int h) = 0;
	/**
	 * @brief Stroke a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void strokeRectangle(int x, int y, int w, int h) = 0;
	/**
	 * @brief Fill and stroke a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void drawRectangle(int x, int y, int w, int h) = 0;

	/**
	 * @brief Fill a polygonal area.
	 * 
	 * @param pog The area to fill.
	 */
	virtual void fillPolygon(const PolygonF& pog) = 0;
	/**
	 * @brief Stroke a polygonal area.
	 * 
	 * @param pog The area to stroke.
	 */
	virtual void strokePolygon(const PolygonF& pog) = 0;
	/**
	 * @brief Fill and stroke a polygonal area.
	 * 
	 * @param pog The area to fill and stroke.
	 */
	virtual void drawPolygon(const PolygonF& pog) = 0;
	
	/**
	 * @brief Paint text.
	 * 
	 * @note The operation uses fill color.
	 * 
	 * @param x X coordinate of the text area rectangle.
	 * @param y Y coordinate of the text area rectangle.
	 * @param text The text.
	 * @param font The font to use.
	 */
	virtual void fillText(int x, int y, const std::string& text, FontId font) = 0;

	/**
	 * @brief Paint whole image.
	 * 
	 * @param img The image.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 */
	virtual void copyImage(ImageId img, int x, int y) = 0;
	/**
	 * @brief Paint scaled image.
	 * 
	 * @param img The image.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param scale Scaling of the image in both directions.
	 */
	virtual void copyImage(ImageId img, int x, int y, double scale) = 0;
	/**
	 * @brief Paint scaled image.
	 * 
	 * @param img The image.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param xScale Scaling of the image in the X dimension.
	 * @param yScale Scaling of the image in the Y dimension.
	 */
	virtual void copyImage(ImageId img, int x, int y, double xScale,
		double yScale) = 0;
	/**
	 * @brief Paint portion of image.
	 * 
	 * @param img The image.
	 * @param srcRect Portion of the source image to paint.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 */
	virtual void copyImage(ImageId img, const Rect& srcRect, int x, int y) = 0;
	/**
	 * @brief Paint scaled portion of image.
	 * 
	 * @param img The image.
	 * @param srcRect Portion of the source image to paint.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param scale Scaling of the image in both directions.
	 */
	virtual void copyImage(ImageId img, const Rect& srcRect, int x, int y,
		double scale) = 0;
	/**
	 * @brief Paint scaled portion of image.
	 * 
	 * @param img The image.
	 * @param srcRect Portion of the source image to paint.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param xScale Scaling of the image in the X dimension.
	 * @param yScale Scaling of the image in the Y dimension.
	 */
	virtual void copyImage(ImageId img, const Rect& srcRect, int x, int y,
		double xScale, double yScale) = 0;
	/**
	 * @brief Paint image.
	 * 
	 * @param img The image.
	 * @param dstRect Area on the canvas to paint into. The image will be
	 *                scaled.
	 */
	virtual void copyImage(ImageId img, const Rect& dstRect) = 0;
	/**
	 * @brief Paint image.
	 * 
	 * @param img The image.
	 * @param srcRect Portion of the source image to paint.
	 * @param dstRect Area on the canvas to paint into. The image will be
	 *                scaled.
	 */
	virtual void copyImage(ImageId img, const Rect& srcRect,
		const Rect& dstRect) = 0;
};

#endif // ICANVAS_HPP
