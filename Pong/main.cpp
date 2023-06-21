#include "Game.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int VIRTUAL_WIDTH = 432;
const int VIRTUAL_HEIGHT = 243;
const int FPS = 60;

int main(int argc, char* argv[]) {
    
    Game game;
    if (!game.initSystem("Pong", SCREEN_WIDTH, SCREEN_HEIGHT, true)) {
        std::cout << "Failed to initialize!\n";
        return 1;
    }
    if (!game.loadResources()) {
        std::cout << "Failed to load resources!\n";
        return 1;
    }

    game.start();

    while (game.isRunning()) {
        uint32_t startTime = SDL_GetTicks();
        SDL_SetEventFilter(Game::resizingEventWatcher, &game);
        game.input();
        game.update(1.0f / FPS);
        game.render();

        uint32_t endTime = SDL_GetTicks();
        uint32_t elapsedTime = endTime - startTime;
        if (elapsedTime < 1000 / FPS) {
            SDL_Delay(1000 / FPS - elapsedTime);
        }
    }

    game.close();

    return 0;
}
