/**
 * @file sprite.h
 * @author Tomáš Ludrovan
 * @brief Sprite class declarations
 * @version 0.1
 * @date 2023-11-14
 */
#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

#include "scene.h"

/**
 * @brief Base class for sprites.
 */
class Sprite : public Scene::SceneSprite {
protected:
	Scene &parent;
	int x;
	int y;
public:
	Sprite(Scene &parent) : parent{parent}, x{0}, y{0} {}
	virtual ~Sprite() {}

	virtual int getX() { return x; }
	virtual void setX(int value);
	virtual int getY() { return y; }
	virtual void setY(int value);
};

/**
 * @brief An ellipse-shaped sprite.
 */
class SpriteEllipsis : public Sprite {
private:
	static constexpr int W = 16;
	static constexpr int H = 16;
public:
	SpriteEllipsis(Scene &parent) : Sprite(parent) {}

	void render() override;
	void getBounds(SDL_Rect *out_bounds) override;
};

/**
 * @brief A circle-shaped sprite.
 */
class SpriteCircle : public Sprite {
private:
	static constexpr int R = 36;
public:
	SpriteCircle(Scene &parent) : Sprite(parent) {}

	void render() override;
	void getBounds(SDL_Rect *out_bounds) override;
};

#endif // SPRITE_H
