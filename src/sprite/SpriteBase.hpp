/**
 * @file SpriteBase.hpp
 * @author Tomáš Ludrovan
 * @brief SpriteBase class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SPRITEBASE_HPP
#define SPRITEBASE_HPP

#include <memory>

#include "sprite/ISprite.hpp"
#include "paintingproxy/IPaintingProxy.hpp"

// TODO: Begin change + End change (to optimize painting)

/**
 * @brief Base class for all sprites.
 * 
 * @remark Descendants should not override `repaint()` method to provide their
 *         "painting logic", but instead use override `repaintAsVisible()`. It
 *         is called from the `repaint()` after it has been checked that the
 *         sprite is visible.
 * 
 *         Descendants should not call the `invalidateRect()` method directly
 *         from the `paintingProxy` object, but instead use this class's (and
 *         its descendants') `invalidate()` and `invalidateRect()` methods.
 *         They may override this method to provide their own invalidation
 *         logic, but they should still prefer the inherited methods over the
 *         direct `paintingProxy->invalidateRect()` call.
 */
class SpriteBase : virtual public ISprite {
private:
	template <typename T>
	void propertyChangeValue(T& prop, const T& value);
protected:
	std::shared_ptr<IPaintingProxy> paintingProxy;
	bool isVisible;
	
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @details This method is called from `repaint()` after it has been
	 *          observed that the "is visible" property is `true`.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	virtual void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) = 0;
	/**
	 * @brief Invalidates the area taken by the sprite.
	 */
	virtual void invalidate();
	/**
	 * @brief Invalidates the whole screen.
	 */
	virtual void invalidateRect();
	/**
	 * @brief Invalidates a rectangular area on the screen.
	 */
	virtual void invalidateRect(const Rect& rect);

	/**
	 * @brief Generic property getter.
	 * 
	 * @tparam T Type of the property.
	 * @param prop Property value.
	 */
	template <typename T>
	const T& propertyGetter(const T& prop) const;
	/**
	 * @brief Generic property setter.
	 * 
	 * @tparam T Type of the property. Must implement `operator== ()`
	 * @param prop Reference to property.
	 * @param value New property value.
	 */
	template <typename T>
	void propertySetterComparable(T& prop, const T& value);
	/**
	 * @brief Generic property setter.
	 * 
	 * @tparam T Type of the property.
	 * @param prop Reference to property.
	 * @param value New property value.
	 */
	template <typename T>
	void propertySetterNoncomparable(T& prop, const T& value);
public:
	SpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
	virtual ~SpriteBase();
	/**
	 * @brief Getter for the "is visible" property.
	 * 
	 * @details This read-write property indicates whether the sprite is shown.
	 *          If the value is `false`, the sprite will not be painted when
	 *          repainting takes place.
	 */
	virtual bool getIsVisible() override;
	/**
	 * @brief Setter for the "is visible" property.
	 * 
	 * @copydetail SpriteBase::getIsVisible()
	 */
	virtual void setIsVisible(bool value) override;
	/**
	 * @brief Forces the sprite to update.
	 * 
	 * @details Calling this method will make the sprite invalidate the area
	 *          on the screen taken by the sprite.
	 */
	virtual void forceUpdate() override;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	virtual void repaint(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
};

template<typename T>
inline void SpriteBase::propertyChangeValue(T& prop, const T& value)
{
	invalidate();
	prop = value;
	invalidate();
}

template<typename T>
inline const T& SpriteBase::propertyGetter(const T& prop) const
{
	return prop;
}

template<typename T>
inline void SpriteBase::propertySetterComparable(T& prop, const T& value)
{
	if (prop != value) {
		propertyChangeValue(prop, value);
	}
}

template<typename T>
inline void SpriteBase::propertySetterNoncomparable(T& prop, const T& value)
{
	propertyChangeValue(prop, value);
}

#endif // SPRITEBASE_HPP
