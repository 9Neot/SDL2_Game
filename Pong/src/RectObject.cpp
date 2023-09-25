#include "../include/RectObject.h"

void RectObject::init(const SDL_FRect& rect, const SDL_Color& color) {
	this->rect = rect;
	this->color = color;
}

void RectObject::render(SDL_Renderer*& renderer) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRectF(renderer, &rect);
}

void RectObject::update(float dt) {
	this->rect.x += this->dx * dt;
	this->rect.y += this->dy * dt;
}

void RectObject::setDX(float dx) {
	this->dx = dx;
}

void RectObject::setDY(float dy) {
	this->dy = dy;
}

SDL_FRect RectObject::getRect() const{
	return this->rect;
}

void RectObject::setX(float x) {
	this->rect.x = x;
}

void RectObject::setY(float y) {
	this->rect.y = y;
}

float RectObject::getDX() {
	return this->dx;
}

float RectObject::getDY() {
	return this->dy;
}
