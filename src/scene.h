#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <SDL2pp/SDL2pp.hh>


class Scene {
public:
	class SceneSprite {
	public:
		virtual void render() = 0;
		virtual void getBounds(SDL_Rect *out_bounds) = 0;
	};
private:
	std::vector<std::shared_ptr<SceneSprite>> sprites;
	bool is_invalid;
public:
	static SDL2pp::SDL sdl;
	static SDL2pp::Window win;
	static SDL2pp::Renderer renderer;
	Scene() : is_invalid{false} {}
	virtual ~Scene() {}

	static int getW() {
		int w;
		SDL_GetWindowSize(win.Get(), &w, nullptr);
		return w;
	};
	static int getH() {
		int h;
		SDL_GetWindowSize(win.Get(), nullptr, &h);
		return h;
	}
	
	void invalidate(SDL_Rect *invalid_rect);
	void paint();
	void add_sprite(std::shared_ptr<SceneSprite> sprite);
};

#endif // SCENE_H
