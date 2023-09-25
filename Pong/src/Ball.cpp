#include "../include/Ball.h"

void Ball::init(const SDL_FRect& rect, const SDL_Color& color) {
	RectObject::init(rect, color);
	if (RandomFloat::generate(0.0f, 1.0f) >= 0.5f) {
		this->dx = 120.0f;
	}
	else {
		this->dx = -120.0f;
	}
	this->dy = RandomFloat::generate(-60.0f, 60.0f);
}

void Ball::reset() {
	rect.x = (VIRTUAL_WIDTH - rect.w) / 2;
	rect.y = (VIRTUAL_HEIGHT - rect.h) / 2;
	if (RandomFloat::generate(0.0f, 1.0f) >= 0.5f) {
		this->dx = 100.0f;
	}
	else {
		this->dx = -100.0f;
	}
	this->dy = RandomFloat::generate(-60.0f, 60.0f);
}

bool Ball::collides(const RectObject* object) {
	if (this->rect.x > object->getRect().x + object->getRect().w 
		|| this->rect.x + this->rect.w < object->getRect().x 
		|| this->rect.y > object->getRect().y + object->getRect().h
		|| this->rect.y + this->rect.h < object->getRect().y) {
		return false;
	}
	return true;
}
