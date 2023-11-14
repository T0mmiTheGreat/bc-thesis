/**
 * @file scene.h
 * @author Tomáš Ludrovan
 * @brief Scene class declaration
 * @version 0.1
 * @date 2023-11-14
 */
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <SDL2pp/SDL2pp.hh>


/**
 * @brief Takes care of screen output.
 */
class Scene {
public:
	/**
	 * @brief Base class for sprites - members of scene.
	 */
	class SceneSprite {
	public:
		/**
		 * @brief Renders itself using the Scene::renderer.
		 * 
		 * @details One can swap the target of the renderer so the sprite is
		 *          rendered onto a texture instead of the window.
		 */
		virtual void render() = 0;
		/**
		 * @brief Returns a rectangle containing the sprite.
		 * 
		 * @details The rectangle doesn't have to be the smallest possible. The
		 *          only requirement is that every pixel of the sprite must be
		 *          within that rectangle.
		 */
		virtual void getBounds(SDL_Rect *out_bounds) = 0;
	};
private:
	std::vector<std::shared_ptr<SceneSprite>> sprites;
	bool is_invalid; // If so, it should be painted
public:
	static SDL2pp::SDL sdl;
	static SDL2pp::Window win; // Global window
	static SDL2pp::Renderer renderer; // Global renderer
	Scene() : is_invalid{false} {}
	virtual ~Scene() {}

	static int getW() { return win.GetWidth(); }
	static int getH() { return win.GetHeight(); }
	
	/**
	 * @brief Mark an areain the scene that needs to be repainted.
	 * 
	 * @param invalid_rect The area to repaint.
	 */
	void invalidate(SDL_Rect *invalid_rect);
	/**
	 * @brief Renders itself using the global renderer.
	 * 
	 * @details If the render target is the window, it will be refreshed.
	 */
	void paint();
	/**
	 * @brief Includes sprite in the sprite list for painting.
	 */
	void add_sprite(std::shared_ptr<SceneSprite> sprite);
};

#endif // SCENE_H
