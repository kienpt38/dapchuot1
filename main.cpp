#include "game.h"

Game game;

int SDL_main(int argc, char* argv[]) {
    if (!game.init("Dap Chuot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600)) {
        return -1;
    }

    game.loadResources();

    while (game.running()) {
        game.handleEvents();
        game.update();
        game.render();
        SDL_Delay(16);
    }

    game.clean();
    return 0;
}
