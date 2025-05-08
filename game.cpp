#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game()
    : window(nullptr), renderer(nullptr), startTexture(nullptr), bgTexture(nullptr), mouseTexture(nullptr),
      font(nullptr), isRunning(false), gameStarted(false), score(0), startTime(0), lastMouseMove(0) {
    srand(static_cast<unsigned int>(time(0)));
}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int x, int y, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        std::cout << "SDL or TTF Init Failed!\n";
        return false;
    }

    window = SDL_CreateWindow(title, x, y, w, h, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        std::cout << "Window or Renderer creation failed!\n";
        return false;
    }

    isRunning = true;
    return true;
}

void Game::loadResources() {
    SDL_Surface* startSurface = IMG_Load("C:/Users/trung/OneDrive/Hình ảnh/nenbgr.png");
    SDL_Surface* bgSurface = IMG_Load("C:/Users/trung/OneDrive/Hình ảnh/abcxyz/picture/saustart.png");
    SDL_Surface* mouseSurface = IMG_Load("C:/Users/trung/OneDrive/Hình ảnh/abcxyz/picture/chuotmit.png");

    if (!startSurface || !bgSurface || !mouseSurface) {
        std::cout << "Failed to load images!\n";
        isRunning = false;
        return;
    }

    startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    mouseTexture = SDL_CreateTextureFromSurface(renderer, mouseSurface);

    SDL_FreeSurface(startSurface);
    SDL_FreeSurface(bgSurface);
    SDL_FreeSurface(mouseSurface);

    font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    if (!font) {
        std::cout << "Failed to load font!\n";
        isRunning = false;
    }

    mouseRect.w = 100;
    mouseRect.h = 100;
    randomizeMousePosition();
}

void Game::randomizeMousePosition() {
    mouseRect.x = rand() % (800 - mouseRect.w);
    mouseRect.y = rand() % (600 - mouseRect.h);
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            if (!gameStarted && SDL_GetTicks() - startTime >= 15000) {
                // Restart game
                gameStarted = true;
                score = 0;
                startTime = SDL_GetTicks();
                lastMouseMove = SDL_GetTicks();
                randomizeMousePosition();
            } else if (!gameStarted) {
                // Start game
                gameStarted = true;
                score = 0;
                startTime = SDL_GetTicks();
                lastMouseMove = SDL_GetTicks();
                randomizeMousePosition();
            } else if (x >= mouseRect.x && x <= mouseRect.x + mouseRect.w &&
                       y >= mouseRect.y && y <= mouseRect.y + mouseRect.h) {
                score += 10;
                randomizeMousePosition();
            }
        }
    }
}

void Game::update() {
    if (!gameStarted) return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - startTime >= 15000) {
        gameStarted = false;
        return;
    }

    if (currentTime - lastMouseMove >= 1000) {
        randomizeMousePosition();
        lastMouseMove = currentTime;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);

    if (!gameStarted && (SDL_GetTicks() - startTime >= 15000) && score > 0) {
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        drawText("Game Over!", 300, 200);
        drawText("Final Score: " + std::to_string(score), 280, 260);
        drawText("Click to Restart", 280, 320);
    }
    else if (!gameStarted) {
        SDL_RenderCopy(renderer, startTexture, nullptr, nullptr);
        drawText("Click to Start", 300, 260);
    }
    else {
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        SDL_RenderCopy(renderer, mouseTexture, nullptr, &mouseRect);

        int timeLeft = 15 - (SDL_GetTicks() - startTime) / 1000;
        drawText("Score: " + std::to_string(score), 10, 10);
        drawText("Time: " + std::to_string(timeLeft), 10, 50);
    }

    SDL_RenderPresent(renderer);
}

void Game::drawText(const std::string& text, int x, int y) {
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::clean() {
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(mouseTexture);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}
