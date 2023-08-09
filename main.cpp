#include "SnakeGame.h"
#include <SDL.h>



int main(int argc, char* args[]) {

    SnakeGame snakeGame;
    snakeGame.initializeWindow();
    SDL_Window* window = snakeGame.getWindow();
    SDL_Renderer* renderer = snakeGame.getRenderer();
    
    const int TARGET_FPS = 12;
    const int FRAME_DELAY = 1000 / TARGET_FPS;

    while (snakeGame.isRunning) {
        Uint64 frameStart = SDL_GetPerformanceCounter();

        snakeGame.processInput();
        snakeGame.update();
        snakeGame.render(renderer);

        Uint64 frameEnd = SDL_GetPerformanceCounter();

        float deltaTime = (frameEnd - frameStart) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        if (deltaTime < FRAME_DELAY) {
            SDL_Delay(floor(FRAME_DELAY - deltaTime));
        }
        //SDL_Delay(floor(120.0f - elapsedMS));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}