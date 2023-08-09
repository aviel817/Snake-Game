#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include <random>
#include <SDL_ttf.h>

using namespace std;

class SnakeGame {
    public:
        SnakeGame();
        void initializeWindow();
        SDL_Renderer* getRenderer() const;
        SDL_Window* getWindow() const;
        void processInput();
        void update();
        void render(SDL_Renderer* renderer);
        bool isRunning;

    private:

        //void initializeRandom();
        void initializeTextRendering();
        void generateApple();
        void drawApple();
        void drawSnake();
        void drawScore();
        void increseSnakeSize();
        void moveSnake();
        void checkSelfCollision(const vector<SDL_Rect>& rects);
        bool checkCollision(const SDL_Rect& rectA, const SDL_Rect& rectB);
        const int SCREEN_WIDTH = 800;
        const int SCREEN_HEIGHT = 600;
        const int SNAKE_BODY_SIZE = 30;
        const int APPLE_SIZE = 30;
        vector<SDL_Rect> snakeRects;
        int snakeSize;
        pair<int, int> spawnPos;
        string direction;
        int score = 0;
        SDL_Rect currentApple;
        SDL_Renderer* renderer = NULL;
        SDL_Window* window = NULL;
        random_device rd;
        mt19937 gen;
        uniform_int_distribution<int> widthDis;
        uniform_int_distribution<int> heightDis;
        TTF_Font* font;
};
