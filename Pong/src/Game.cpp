#include "../include/Game.h"

bool Game::initSystem(const char* title, const int& width, const int& height, const bool& resizalbe) {

	if (initSDL()) {

		//Set texture filtering to linear (nearest)
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			std::cout << "Warning: Linear texture filtering not enabled!";
		}

		//Create window
		int resizalbeFlag = resizalbe ? 32 : 0;
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | resizalbeFlag);

		if (!window) {
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
			return false;
		}
	}

	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// Set the logical size for rendering
	SDL_RenderSetLogicalSize(renderer, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

	if (!renderer) {
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
		return false;
	}

	//Initialize extensions
	return initSDL_Image() && initSDL_ttf() && initSDL_mixer();
}

bool Game::loadResources() {
	//Load fonts
#ifdef SDL_TTF_MAJOR_VERSION
	TTF_Font* largeFont = TTF_OpenFont("assets/fonts/VT323-Regular.ttf", 40);
	if (!checkLoading(largeFont)) {
		return false;
	}

	TTF_Font* mediumFont = TTF_OpenFont("assets/fonts/VT323-Regular.ttf", 28);
	if (!checkLoading(mediumFont)) {
		return false;
	}

	TTF_Font* smallFont = TTF_OpenFont("assets/fonts/VT323-Regular.ttf", 16);
	if (!checkLoading(smallFont)) {
		return false;
	}

	fonts[LARGE] = largeFont;
	fonts[MEDIUM]= mediumFont;
	fonts[SMALL] = smallFont;
#endif

	//Load images
#ifdef SDL_IMAGE_MAJOR_VERSION

#endif

	//Load sounds
#ifdef SDL_MIXER_MAJOR_VERSION
	music = Mix_LoadMUS("assets/sounds/Music.wav");
	if (!checkLoading(music)) {
		return false;
	}

	sounds[HIT_PADDLE] = Mix_LoadWAV("assets/sounds/Hit_Paddle.wav");
	if (!checkLoading(sounds[HIT_PADDLE])) {
		return false;
	}
	
	sounds[HIT_WALL] = Mix_LoadWAV("assets/sounds/Hit_Wall.wav");
	if (!checkLoading(sounds[HIT_WALL])) {
		return false;
	}

	sounds[MISS] = Mix_LoadWAV("assets/sounds/Miss.wav");
	if (!checkLoading(sounds[MISS])) {
		return false;
	}

	sounds[VICTORY] = Mix_LoadWAV("assets/sounds/Victory.wav");
	if (!checkLoading(sounds[VICTORY])) {
		return false;
	}

	Mix_VolumeMusic(16);
	Mix_VolumeChunk(sounds[HIT_PADDLE], 64);
	Mix_VolumeChunk(sounds[MISS], 64);
	
#endif 

	return true;
}

bool Game::isRunning() {
	return this->running;
}

void Game::start() {
	title = "Welcome to Pong!";
	message = "Press Enter to begin!";
	paddleSpeed = 200.0f;
	running = true;
	state = START;
	scores[LEFT] = 0;
	scores[RIGHT] = 0;
	if (RandomFloat::generate(0.0f, 1.0f) < 0.5f) {
		servingPlayer = LEFT;
	}
	else {
		servingPlayer = RIGHT;
	}
	instantiate();
	Mix_PlayMusic(music, -1);
}

void Game::input() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
				if (state == START) {
					state = SERVE;
				}
				else if (state == SERVE) {
					state = PLAY;
				}
				else if (state == OVER) {
					this->reset();
				}
			}
			break;	
		default:
			break;	
		}
	}

	if (this->state == SERVE || this->state == PLAY) {
		const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
		if (currentKeyStates[SDL_SCANCODE_W]) {
			paddles[LEFT]->setDY(-paddleSpeed);
		}
		else if (currentKeyStates[SDL_SCANCODE_S]) {
			paddles[LEFT]->setDY(paddleSpeed);
		}
		else {
			paddles[LEFT]->setDY(0.0f);
		}
		if (currentKeyStates[SDL_SCANCODE_UP]) {
			paddles[RIGHT]->setDY(-paddleSpeed);
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
			paddles[RIGHT]->setDY(paddleSpeed);
		}
		else {
			paddles[RIGHT]->setDY(0.0f);
		}
	}
}

void Game::update(const float& dt) {
	if (state == SERVE) {
		titleText->setText("Player " + std::to_string(servingPlayer + 1) + "'s serve!");
		messageText->setText("Press enter to serve!");
		ball->setDY(RandomFloat::generate(-60.0f, 60.0f));
		if (servingPlayer == LEFT) {
			ball->setDX(120.0f);
		}
		else {
			ball->setDX(-120.0f);
		}
	}

	if (state == PLAY) {
		titleText->setText(" ");
		messageText->setText(" ");
		if (ball->collides(paddles[LEFT])) {
			Mix_PlayChannel(-1, sounds[HIT_PADDLE], 0);
			ball->setDX(-ball->getDX() * 1.03f);
			ball->setX(paddles[LEFT]->getRect().x + paddles[LEFT]->getRect().w);
			if (ball->getDY() >= 0) {
				ball->setDY(RandomFloat::generate(10, 100));
			}
			else {
				ball->setDY(RandomFloat::generate(-100, -10));
			}
		}

		if (ball->collides(paddles[RIGHT])) {
			Mix_PlayChannel(-1, sounds[HIT_PADDLE], 0);
			ball->setDX(-ball->getDX() * 1.03f);
			ball->setX(paddles[RIGHT]->getRect().x - ball->getRect().w);
			if (ball->getDY() >= 0) {
				ball->setDY(RandomFloat::generate(10, 100));
			}
			else {
				ball->setDY(RandomFloat::generate(-100, -10));
			}
		}

		if (ball->getRect().y <= 0.0f) {
			Mix_PlayChannel(-1, sounds[HIT_WALL], 0);
			ball->setY(0.0f);
			ball->setDY(-ball->getDY());
		}

		if (ball->getRect().y >= VIRTUAL_HEIGHT - ball->getRect().h) {
			Mix_PlayChannel(-1, sounds[HIT_WALL], 0);
			ball->setY(VIRTUAL_HEIGHT - ball->getRect().h);
			ball->setDY(-ball->getDY());
		}

		if (ball->getRect().x + ball->getRect().w < 0) {	
			this->scores[RIGHT]++;
			servingPlayer = LEFT;
			if (this->scores[RIGHT] == 10) {
				Mix_PlayChannel(-1, sounds[VICTORY], 0);
				state = OVER;
			}
			else {
				Mix_PlayChannel(-1, sounds[MISS], 0);
				ball->reset();
				state = SERVE;
			}
		}

		if (ball->getRect().x > VIRTUAL_WIDTH + ball->getRect().w) {
			this->scores[LEFT]++;
			servingPlayer = RIGHT;
			if (this->scores[LEFT] == 10) {
				Mix_PlayChannel(-1, sounds[VICTORY], 0);
				state = OVER;
			}
			else {
				Mix_PlayChannel(-1, sounds[MISS], 1);
				ball->reset();
				state = SERVE;
			}	
		}

		ball->update(dt);
	}

	if (state == OVER) {
		titleText->setText("Player " + std::to_string(!servingPlayer + 1) + " wins!");
		messageText->setText("Press enter to restart!");
	}

	scoreTexts[LEFT]->setText(std::to_string(scores[LEFT]));
	scoreTexts[RIGHT]->setText(std::to_string(scores[RIGHT]));
	paddles[LEFT]->update(dt);
	paddles[RIGHT]->update(dt);

	if (paddles[LEFT]->getRect().y < 0.0f) {
		paddles[LEFT]->setY(0.0f);
	}
	if (paddles[LEFT]->getRect().y > VIRTUAL_HEIGHT - paddles[LEFT]->getRect().h) {
		paddles[LEFT]->setY(VIRTUAL_HEIGHT - paddles[LEFT]->getRect().h);
	}

	if (paddles[RIGHT]->getRect().y < 0.0f) {
		paddles[RIGHT]->setY(0.0f);
	}
	if (paddles[RIGHT]->getRect().y > VIRTUAL_HEIGHT - paddles[RIGHT]->getRect().h) {
		paddles[RIGHT]->setY(VIRTUAL_HEIGHT - paddles[RIGHT]->getRect().h);
	}
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 40, 45, 52, 255);
	SDL_RenderClear(renderer);

	titleText->render((VIRTUAL_WIDTH - titleText->getWidth()) / 2.0f, VIRTUAL_HEIGHT / 8.0f);
	messageText->render((VIRTUAL_WIDTH - messageText->getWidth()) / 2.0f, VIRTUAL_HEIGHT / 4.0f);

	scoreTexts[LEFT]->render((VIRTUAL_WIDTH - scoreTexts[LEFT]->getWidth()) / 2.0f - 50, VIRTUAL_HEIGHT / 3.0f);
	scoreTexts[RIGHT]->render((VIRTUAL_WIDTH - scoreTexts[RIGHT]->getWidth()) / 2.0f + 50, VIRTUAL_HEIGHT / 3.0f);

	paddles[LEFT]->render(renderer);
	paddles[RIGHT]->render(renderer);

	ball->render(renderer);

	SDL_RenderPresent(renderer);
}

void Game::close() {
	this->free();
	this->destroyResources();
	//Quit SDL subsystems

#ifdef SDL_MIXER_MAJOR_VERSION
	Mix_Quit();
#endif 

#ifdef SDL_TTF_MAJOR_VERSION
	TTF_Quit();
#endif

#ifdef SDL_IMAGE_MAJOR_VERSION
	IMG_Quit();
#endif 

	//Destroy window    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;

	SDL_Quit();
}

void Game::instantiate() {
	SDL_Color white = { 255, 255, 255, 255 };
	titleText = new Text();
	titleText->init(renderer, fonts[MEDIUM], this->title, white);

	messageText = new Text();
	messageText->init(renderer, fonts[SMALL], this->message, white);

	scoreTexts[LEFT] = new Text();
	scoreTexts[LEFT]->init(renderer, fonts[LARGE], std::to_string(scores[LEFT]), white);

	scoreTexts[RIGHT] = new Text();
	scoreTexts[RIGHT]->init(renderer, fonts[LARGE], std::to_string(scores[RIGHT]), white);

	float paddleWidth = 5.0f;
	float paddleHeight = 20.0f;
	float horizontalMargin = 10.0f;
	float verticalMargin = 30.0f;
	float ballSide = 4.0f;
	SDL_FRect rects[PLAYER_COUNT];

	paddles[LEFT] = new Paddle();
	rects[LEFT] = { horizontalMargin, verticalMargin, paddleWidth, paddleHeight };
	paddles[LEFT]->init(rects[LEFT], white);

	paddles[RIGHT] = new Paddle();
	rects[RIGHT] = { VIRTUAL_WIDTH - horizontalMargin - paddleWidth, VIRTUAL_HEIGHT - verticalMargin - paddleHeight, paddleWidth, paddleHeight };
	paddles[RIGHT]->init(rects[RIGHT], white);

	ball = new Ball();
	SDL_FRect ballRect = { (VIRTUAL_WIDTH - ballSide) / 2 , (VIRTUAL_HEIGHT - ballSide) / 2 , ballSide, ballSide };
	ball->init(ballRect, white);
	
}

void Game::free() {
	titleText->free();
	messageText->free();
	scoreTexts[LEFT]->free();
	scoreTexts[RIGHT]->free();
	titleText = nullptr;
	messageText = nullptr;
	scoreTexts[LEFT] = nullptr;
	scoreTexts[RIGHT] = nullptr;

	delete ball;
	ball = nullptr;
	delete paddles[LEFT];
	paddles[LEFT] = nullptr;
	delete paddles[RIGHT];
	paddles[RIGHT] = nullptr;
}

void Game::destroyResources() {
#ifdef SDL_MIXER_MAJOR_VERSION
	//Free all sounds and quit
	Mix_FreeMusic(music);
	music = nullptr;

	for (int i = 0; i < SOUND_COUNT; i++) {
		Mix_FreeChunk(sounds[i]);
		sounds[i] = nullptr;
	}
#endif

#ifdef SDL_TTF_MAJOR_VERSION
	//Free all fonts, text_Texture and quit
	for (int i = 0; i < FONT_COUNT; i++) {
		TTF_CloseFont(fonts[i]);
		fonts[i] = nullptr;
	}
#endif

#ifdef SDL_IMAGE_MAJOR_VERSION

#endif
}

void Game::reset() {
	state = SERVE;
	scores[LEFT] = 0;
	scores[RIGHT] = 0;
	ball->reset();
}

bool Game::initSDL() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	return true;
}

bool Game::initSDL_Image() {

#ifdef SDL_IMAGE_MAJOR_VERSION
	int imgFlags = IMG_INIT_PNG;

	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() << "\n";
		return false;
	}
#endif 

	return true;
}

bool Game::initSDL_ttf() {

#ifdef SDL_TTF_MAJOR_VERSION
	if (TTF_Init() == -1) {
		std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << "\n";
		return false;
	}
#endif

	return true;
}

bool Game::initSDL_mixer() {

#ifdef SDL_MIXER_MAJOR_VERSION
	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error:" << Mix_GetError() << "\n";
		return false;
	}
#endif 

	return true;
}

template<class T>
bool Game::checkLoading(T*& asset) {
	if (!asset) {
		std::cout << "Failed to load something! Error: " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

int Game::resizingEventWatcher(void* data, SDL_Event* event) {
	Game* game = (Game*) data;
	if (event->type == SDL_WINDOWEVENT && (event->window.event == SDL_WINDOWEVENT_RESIZED || event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event->window.event == SDL_WINDOWEVENT_EXPOSED)) {
		SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
		if (window == game->window) {
			int width = event->window.data1;
			int height = event->window.data2;
			if (width != SCREEN_WIDTH || height != SCREEN_HEIGHT) {
				game->render();
			}
		}
	}
	return 1;
}

