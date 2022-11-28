#include "test_GameCanvas.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <memory>

// #include "../Graphics.h"
// #include "../Image.h"
#include "../../GameCanvas.h"

void test_GameCanvas() {
    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error(IMG_GetError());
    }

    if (TTF_Init() == -1) {
        throw std::runtime_error(TTF_GetError());
    }

    SDL_Window *window = SDL_CreateWindow(
        0,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        320, 240,
        SDL_WINDOW_SHOWN);

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
    SDL_RenderClear(renderer);

    Graphics g(renderer);

    // TEST CODE HERE

    std::unique_ptr<Micro> micro = std::make_unique<Micro>();
    std::unique_ptr<GameCanvas> gameCanvas = std::make_unique<GameCanvas>(micro.get());

    Micro::field_249 = true;
    gameCanvas->render_160(&g);
    // gameCanvas->renderGreyCircles();

    // !TEST CODE HERE

    SDL_RenderPresent(renderer); // Send to screen

    // Event loop
    SDL_Event event;
    bool flag = true;
    while (flag) {
        while (SDL_WaitEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                flag = false;
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}
