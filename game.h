#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int x, int y, int w, int h);
    void loadResources();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const { return isRunning; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* startTexture;
    SDL_Texture* bgTexture;
    SDL_Texture* mouseTexture;
    TTF_Font* font;
    SDL_Rect mouseRect;

    bool isRunning;
    bool gameStarted;
    int score;
    Uint32 startTime;
    Uint32 lastMouseMove;

    void drawText(const std::string& text, int x, int y);
    void randomizeMousePosition();
};

#endif
