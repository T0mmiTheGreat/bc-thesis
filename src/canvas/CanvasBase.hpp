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

/**
 * @brief The base class for canvases.
 * 
 * @details Provides some convenient definitions for descendants.
 * 
 *          The getWidth()/getHeight() methods use the getRect() method to
 *          obtain the canvas dimensions.
 * 
 *          The filling color, stroking color and stroke width variables are
 *          defined, along with the definitions of getters and setters for
 *          them.
 * 
 *          If the descendants don't have a specific way of filling and stroking
 *          shapes at the same time, this class defines the draw<Sth>() methods
 *          to call fill<Sth>() and then stroke<Sth>() so the descendants don't
 *          have to.
 */
class CanvasBase : public ICanvas {
protected:
	/**
	 * @brief Color used for filling operations.
	 */
	Color fColor;
	/**
	 * @brief Color used for stroking operations.
	 */
	Color sColor;
	/**
	 * @brief Stroke width.
	 */
	double sWidth;
	/**
	 * @brief Color modulation.
	 */
	Color colorMod;

	/**
	 * @brief Returns the size of an image.
	 * 
	 * @remark This method returns empty size. If descendants are not willing to
	 *         implement all the overloads of the copyImage() method, they
	 *         should override this method.
	 */
	virtual Size2d getImageSize(ImageId img);
	/**
	 * @brief Returns the filling color with modulation applied.
	 */
	virtual Color getFillingColorMod();
	/**
	 * @brief Returns the stroking color with modulation applied.
	 */
	virtual Color getStrokingColorMod();
public:
	CanvasBase();
	/**
	 * @brief Get width of the canvas in screen coordinates.
	 * 
	 * @details Uses `getRect()` to obtain the width.
	 */
	virtual int getWidth() override;
	/**
	 * @brief Get height of the canvas in screen coordinates.
	 * 
	 * @details Uses `getRect()` to obtain the height.
	 */
	virtual int getHeight() override;

	/**
	 * @brief Get the current color used for filling operations.
	 * 
	 * @details Returns the `fColor` value.
	 */
	virtual Color getFillingColor() override;
	/**
	 * @brief Get the current color used for stroking operations.
	 * 
	 * @details Returns the `sColor` value.
	 */
	virtual Color getStrokingColor() override;
	/**
	 * @brief Set the color for use in filling operations.
	 * 
	 * @details Modifies the `fColor` value.
	 */
	virtual void setFillingColor(Color color) override;
	/**
	 * @brief Set the color for use in stroking operations.
	 * 
	 * @details Modifies the `sColor` value.
	 */
	virtual void setStrokingColor(Color color) override;

	/**
	 * @brief Get the stroke thickness in stroking operations.
	 * 
	 * @details Returns the `sWidth` value.
	 */
	virtual double getStrokeWidth() override;
	/**
	 * @brief Set the stroke thickness for stroking operations.
	 * 
	 * @details Modifies the `sWidth` value.
	 */
	virtual void setStrokeWidth(double value) override;
	
	/**
	 * @brief Get the current color and alpha modulation of drawing operations.
	 */
	virtual Color getColorMod();
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
	virtual void setColorMod(const Color& value);

	/**
	 * @brief Fill and stroke an elliptical area.
	 * 
	 * @details Calls `fillEllipse()`, then `strokeEllipse()`.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void drawEllipse(int x, int y, int rx, int ry) override;
	/**
	 * @brief Fill and stroke a circular area.
	 * 
	 * @details Calls `fiillCircle()`, then `strokeCircle()`.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void drawCircle(int x, int y, int r) override;
	/**
	 * @brief Fill and stroke a rectangular area.
	 * 
	 * @details Calls `fillRectangle()`, then `strokeRectangle()`.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void drawRectangle(int x, int y, int w, int h) override;
	
	/**
	 * @brief Paint whole image.
	 * 
	 * @param img The image.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * 
	 * @remark If descendants are not going to override this method, they should
	 *         override the getImageSize() method.
	 */
	virtual void copyImage(ImageId img, int x, int y) override;
	/**
	 * @brief Paint scaled image.
	 * 
	 * @param img The image.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param scale Scaling of the image in both directions.
	 * 
	 * @remark If descendants are not going to override this method, they should
	 *         override the getImageSize() method.
	 */
	virtual void copyImage(ImageId img, int x, int y, double scale) override;
	/**
	 * @brief Paint scaled image.
	 * 
	 * @param img The image.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param xScale Scaling of the image in the X dimension.
	 * @param yScale Scaling of the image in the Y dimension.
	 * 
	 * @remark If descendants are not going to override this method, they should
	 *         override the getImageSize() method.
	 */
	virtual void copyImage(ImageId img, int x, int y, double xScale,
		double yScale) override;
	/**
	 * @brief Paint portion of image.
	 * 
	 * @param img The image.
	 * @param srcRect Portion of the source image to paint.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * 
	 * @remark If descendants are not going to override this method, they should
	 *         override the getImageSize() method.
	 */
	virtual void copyImage(ImageId img, const Rect& srcRect, int x, int y) override;
	/**
	 * @brief Paint scaled portion of image.
	 * 
	 * @param img The image.
	 * @param srcRect Portion of the source image to paint.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param scale Scaling of the image in both directions.
	 * 
	 * @remark If descendants are not going to override this method, they should
	 *         override the getImageSize() method.
	 */
	virtual void copyImage(ImageId img, const Rect& srcRect, int x, int y,
		double scale) override;
	/**
	 * @brief 
	 * 
	 * @param img The image.
	 * @param srcRect Portion of the source image to paint.
	 * @param x X coordinate of the top left corner of the image.
	 * @param y Y coordinate of the top left corner of the image.
	 * @param xScale Scaling of the image in the X dimension.
	 * @param yScale Scaling of the image in the Y dimension.
	 * 
	 * @remark If descendants are not going to override this method, they should
	 *         override the getImageSize() method.
	 */
	virtual void copyImage(ImageId img, const Rect& srcRect, int x, int y,
		double xScale, double yScale) override;
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

#endif // CANVASBASE_HPP
