#pragma once
#include <SDL.h>

class RectObject {
public:
	void render(SDL_Renderer*& renderer);
	virtual void init(const SDL_FRect& rect, const SDL_Color& color);
	void update(float dt);
	SDL_FRect getRect() const;
	void setDX(float dx);
	void setDY(float dy);
	float getDX();
	float getDY();
	void setX(float x);
	void setY(float y);

protected:
	SDL_FRect rect;
	SDL_Color color;
	float dx;
	float dy;
};