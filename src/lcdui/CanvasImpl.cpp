#include "CanvasImpl.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <iostream>

#include "Canvas.h"
#include "../config.h"

CanvasImpl::CanvasImpl(Canvas* canvas)
{
    this->canvas = canvas;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error(IMG_GetError());
    }

    if (TTF_Init() == -1) {
        throw std::runtime_error(TTF_GetError());
    }

    uint32_t windowFlags = SDL_WINDOW_SHOWN;

    if (GlobalSetting::WindowFullscreen) {
        windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    window = SDL_CreateWindow(
        0,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        GlobalSetting::DefaultScreenWidth, GlobalSetting::DefaultScreenHeight,
        windowFlags);

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

CanvasImpl::~CanvasImpl()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void CanvasImpl::repaint()
{
    SDL_RenderPresent(renderer);
}

int CanvasImpl::getWidth()
{
    if (windowWidth < 0) {
        return GlobalSetting::DefaultScreenWidth;
    }

    return windowWidth;
}

int CanvasImpl::getHeight()
{
    if (windowHeight < 0) {
        return GlobalSetting::DefaultScreenHeight;
    }

    return windowHeight;
}

SDL_Renderer* CanvasImpl::getRenderer()
{
    return renderer;
}

void CanvasImpl::processEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            exit(0); // IMPROVE This is a super dumb way to finish the game, but it works
            break;
        case SDL_KEYDOWN: {
            int keyCode = convertKeyCharToKeyCode(e.key.keysym.sym);
            // std::cout << "Key pressed: " << keyCode << std::endl;
            if (keyCode != 0) {
                canvas->publicKeyPressed(keyCode);
            }
        } break;
        case SDL_KEYUP: {
            int sdlCode = e.key.keysym.sym;
            int keyCode = convertKeyCharToKeyCode(sdlCode);
            // std::cout << "Key released: " << keyCode << std::endl;
            if (keyCode != 0) {
                canvas->publicKeyReleased(keyCode);
            } else {
                if (sdlCode == SDLK_ESCAPE) {
                    // std::cout << "ESC released" << std::endl;
                    canvas->pressedEsc();
                }
            }
        } break;
        default:
            break;
        }
    }
}

int CanvasImpl::convertKeyCharToKeyCode(SDL_Keycode keyCode)
{
    switch (keyCode) {
    case SDLK_RETURN:
        return Canvas::Keys::FIRE;
    case SDLK_LEFT:
    case SDLK_a:
        return Canvas::Keys::LEFT;
    case SDLK_RIGHT:
    case SDLK_d:
        return Canvas::Keys::RIGHT;
    case SDLK_UP:
    case SDLK_w:
        return Canvas::Keys::UP;
    case SDLK_DOWN:
    case SDLK_s:
        return Canvas::Keys::DOWN;
    default:
        std::cout << "unknown keyEvent: " << keyCode << std::endl;
        return 0;
    }
}

void CanvasImpl::setWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}