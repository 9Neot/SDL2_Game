#include "../include/Paddle.h"

void Paddle::init(const SDL_FRect& rect, const SDL_Color& color) {
	RectObject::init(rect, color);
	this->dy = 0.0f;
}
