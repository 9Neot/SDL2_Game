#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
//#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Paddle.h"
#include "Ball.h"
#include "Text.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int VIRTUAL_WIDTH;
extern const int VIRTUAL_HEIGHT;

enum State {
	START,
	SERVE,
	PLAY,
	OVER
};

enum Player {
	LEFT,
	RIGHT,
	PLAYER_COUNT
};

#ifdef SDL_MIXER_MAJOR_VERSION
enum Sound {
	HIT_PADDLE,
	HIT_WALL,
	MISS,
	VICTORY,
	SOUND_COUNT
};
#endif

#ifdef SDL_TTF_MAJOR_VERSION
enum Font {
	LARGE,
	MEDIUM,
	SMALL,
	FONT_COUNT
};
#endif

class Game {
public:
	bool initSystem(const char* title, const int& width, const int& height, const bool& resizalbe);
	bool loadResources();
	void start();
	void input();
	void update(const float& dt);
	void render();
	void close();
	bool isRunning();
	static int resizingEventWatcher(void* data, SDL_Event* event);
	
private:
	bool running;
	int scores[PLAYER_COUNT];
	float paddleSpeed;
	bool servingPlayer;
	std::string title;
	std::string message;

	SDL_Window* window;
	SDL_Renderer* renderer;
	State state;
	Ball* ball;
	Paddle* paddles[PLAYER_COUNT];
	Text* titleText;
	Text* messageText;
	Text* scoreTexts[PLAYER_COUNT];

	//Fonts
#ifdef SDL_TTF_MAJOR_VERSION
	TTF_Font* fonts[FONT_COUNT];
#endif 

	//Sounds
#ifdef SDL_MIXER_MAJOR_VERSION
	Mix_Chunk* sounds[SOUND_COUNT];
	Mix_Music* music;
#endif 

	//Images
#ifdef SDL_IMAGE_MAJOR_VERSION

#endif 

	bool initSDL();
	bool initSDL_Image();
	bool initSDL_ttf();
	bool initSDL_mixer();
	void instantiate();
	void reset();
	void free();
	void destroyResources();
	template<class T>
	bool checkLoading(T*& asset);

};