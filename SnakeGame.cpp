#include "SnakeGame.h"
#include <iostream>
#include <random>
#include <string>

SnakeGame::SnakeGame() : isRunning(true), snakeSize(3), direction("left"), widthDis(0, SCREEN_WIDTH-APPLE_SIZE), heightDis(0, SCREEN_HEIGHT-APPLE_SIZE) {
    spawnPos.first = SCREEN_WIDTH / 2;
    spawnPos.second = SCREEN_HEIGHT / 2;
    for (int i = 0; i < snakeSize; i++) {
        SDL_Rect rect = { spawnPos.first - snakeSize + i, spawnPos.second, SNAKE_BODY_SIZE, SNAKE_BODY_SIZE };
        snakeRects.push_back(rect);
    }

    generateApple();
}

void SnakeGame::initializeWindow() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    initializeTextRendering();
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}


SDL_Renderer* SnakeGame::getRenderer() const{
    return renderer;
}

SDL_Window* SnakeGame::getWindow() const {
    return window;
}

void SnakeGame::generateApple() {

    bool isCollision;
    SDL_Rect appleRect;
    do {
        isCollision = false;
        // Generate random width and height
        int randomWidth = widthDis(gen);
        int randomHeight = heightDis(gen);
        appleRect = { randomWidth, randomHeight, APPLE_SIZE, APPLE_SIZE };
        for (int i = 0; i < snakeRects.size(); i++) {
            if (checkCollision(appleRect, snakeRects[i]))
                isCollision = true;
        }
    } while (isCollision);

    currentApple = appleRect;
    
}

void SnakeGame::drawApple() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &currentApple);
}

void SnakeGame::drawSnake() {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snakeRects.size(); i++) {
        SDL_RenderFillRect(renderer, &snakeRects[i]);
    }
}

void SnakeGame::initializeTextRendering() {
    // Load a font file (optional - built-in fonts are available as well)
    const char* fontFilePath = "FreeSans.ttf";
    int fontSize = 24;
    font = TTF_OpenFont(fontFilePath, fontSize);
}

void SnakeGame::drawScore() {

    // Set the text color (in this example, white color)
    SDL_Color textColor = { 255, 255, 255 };
    string text = "Score: " + to_string(score); // Text to render

    // Create a transparent surface with the same size as the text
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // We don't need the surface after creating the texture

    if (!textTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    SDL_Rect textRect;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textRect.w, &textRect.h);
    textRect.x = 100;
    textRect.y = 100;
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);
}

void SnakeGame::moveSnake() {
    increseSnakeSize();
    snakeRects.pop_back();
}

void SnakeGame::increseSnakeSize() {
    SDL_Rect snakeHead = snakeRects.front();
    int x = 0, y = 0;
    if (direction == "left") {
        x = snakeHead.x - SNAKE_BODY_SIZE;
        y = snakeHead.y;
    }
    else if (direction == "right") {
        x = snakeHead.x + SNAKE_BODY_SIZE;
        y = snakeHead.y;
    }
    else if (direction == "up") {
        x = snakeHead.x;
        y = snakeHead.y - SNAKE_BODY_SIZE;
    }
    else if (direction == "down") {
        x = snakeHead.x;
        y = snakeHead.y + SNAKE_BODY_SIZE;
    }
    if (x >= SCREEN_WIDTH) {
        x = 0;
    }
    if (x < 0) {
        x = SCREEN_WIDTH-SNAKE_BODY_SIZE;
    }
    if (y >= SCREEN_HEIGHT) {
        y = 0;
    }
    if (y < 0) {
        y = SCREEN_HEIGHT- SNAKE_BODY_SIZE;
    }
    snakeRects.insert(snakeRects.begin(), SDL_Rect{ x, y, SNAKE_BODY_SIZE, SNAKE_BODY_SIZE });
}

void SnakeGame::checkSelfCollision(const vector<SDL_Rect> &rects) {
    SDL_Rect snakeHead = rects.front();
    for (int i = 1; i < rects.size(); i++) {
        if (checkCollision(snakeHead, rects[i])) {
            isRunning = false;
        }
    }
    
}

bool SnakeGame::checkCollision(const SDL_Rect& rectA, const SDL_Rect& rectB) {
    // Check for collision between two rectangles
    return (rectA.x < rectB.x + rectB.w &&
            rectA.x + rectA.w > rectB.x &&
            rectA.y < rectB.y + rectB.h &&
            rectA.y + rectA.h > rectB.y);
}

// Function to handle input events
void SnakeGame::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                // Handle up arrow key press
                if (direction != "down")
                    direction = "up";
                break;
            case SDLK_DOWN:
                // Handle down arrow key press
                if (direction != "up")
                    direction = "down";
                break;
            case SDLK_LEFT:
                // Handle left arrow key press
                if (direction != "right")
                    direction = "left";
                break;
            case SDLK_RIGHT:
                // Handle right arrow key press
                if (direction != "left")
                    direction = "right";
                break;
            default:
                break;
            }
        }
        // Add code to handle other input events (e.g., keyboard, mouse) here
    }
}

// Function to update the game state
void SnakeGame::update() {
    // Add code to update the game state here
    moveSnake();

    checkSelfCollision(snakeRects);
    if (checkCollision(currentApple, snakeRects.front())) {
        score++;
        generateApple();
        increseSnakeSize();
    }
}


void SnakeGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen with black color
    SDL_RenderClear(renderer);
    
    // Add code to render game objects here
    //drawRect(renderer, 0, 0, 50, 50);
    drawSnake();
    drawApple();
    drawScore();

    SDL_RenderPresent(renderer); // Show the rendered frame on the screen
}

