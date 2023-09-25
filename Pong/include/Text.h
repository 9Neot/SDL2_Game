#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

class Text {
public:
	void init(SDL_Renderer*& renderer, TTF_Font*& font, const std::string& text, const SDL_Color& color);
	void render(float x, float y);
	std::string getText();
	void setText(std::string text);
	int getWidth();
	int getHeight();
	void free();

private:
	TTF_Font* font;
	std::string text;
	SDL_Color color;
	SDL_Renderer* renderer; 
	SDL_Texture* texture;
	int width;
	int height;

	void createNewTexture();
};