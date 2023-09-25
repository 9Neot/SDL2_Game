#include "../include/Text.h"

void Text::init(SDL_Renderer*& renderer, TTF_Font*& font, const std::string& text, const SDL_Color& color) {
	this->renderer = renderer;
	this->font = font;
	this->text = text;
	this->color = color;
	createNewTexture();
}

void Text::createNewTexture() {
	free();
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) {
		std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << "\n";
	}
	else {
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture) {
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
		}
		else {
			width = surface->w;
			height = surface->h;
		}
	}
	//Get rid of old loaded surface
	SDL_FreeSurface(surface);
}

void Text::render(float x, float y) {
	if (texture) {
		SDL_FRect desRect = { x, y, (float) this->width, (float) this->height };
		SDL_RenderCopyF(renderer, texture, nullptr, &desRect);
	}
}

void Text::free() {
	if (this->texture) {
		SDL_DestroyTexture(this->texture);
		this->texture = nullptr;
		this->width = 0;
		this->height = 0;
	}
}

std::string Text::getText() {
	return this->text;
}

void Text::setText(std::string text) {
	if (this->text != text) {
		this->text = text;
		createNewTexture();
	}
}

int Text::getWidth() {
	return width;
}

int Text::getHeight() {
	return height;
}
