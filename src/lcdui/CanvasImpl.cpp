#include "CanvasImpl.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

#include "Canvas.h"

CanvasImpl::CanvasImpl(Canvas *canvas) {
    this->canvas = canvas;

    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error(IMG_GetError());
    }

    if (TTF_Init() == -1) {
        throw std::runtime_error(TTF_GetError());
    }

    window = SDL_CreateWindow(
        0,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
    SDL_RenderClear(renderer);
}

CanvasImpl::~CanvasImpl() {
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void CanvasImpl::repaint() {
    SDL_RenderPresent(renderer);
}

int CanvasImpl::getWidth() {
    return width;
}

int CanvasImpl::getHeight() {
    return height;
}

SDL_Renderer* CanvasImpl::getRenderer() {
    return renderer;
}

bool CanvasImpl::quit() {
    SDL_Event e;

    while (SDL_PollEvent( &e ) != 0){
        if (e.type == SDL_QUIT) {
            return true;
        }
    }

    return false;
}

void CanvasImpl::delay(int ms) {
    SDL_Delay(ms);
}