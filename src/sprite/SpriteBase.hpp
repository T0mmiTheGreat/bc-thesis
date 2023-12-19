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
#include "sysproxy/ISysProxy.hpp"

class SpriteBase : public ISprite {
protected:
	int x;
	int y;
	std::shared_ptr<ISysProxy> sysProxy;

	void invalidateBounds();
public:
	SpriteBase();
	virtual int getX() override;
	virtual int getY() override;
	virtual void setX(int value) override;
	virtual void setY(int value) override;
	virtual void setPos(int x, int y) override;
	virtual Rect getBounds() override;
};

#endif // SPRITEBASE_HPP
