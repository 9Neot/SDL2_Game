#pragma once
#include "RectObject.h"
#include "RandomFloat.h"

extern const int VIRTUAL_WIDTH;
extern const int VIRTUAL_HEIGHT;

class Ball : public RectObject {
public:
	void init(const SDL_FRect& rect, const SDL_Color& color);
	void reset();
	bool collides(const RectObject* object);
};