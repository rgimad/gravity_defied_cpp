#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <iostream>

#include "Canvas.h"
#include "../config.h"
#include "../utils/Log.h"

class Canvas;

class CanvasImpl {
private:
    int windowWidth = -1;
    int windowHeight = -1;

    Canvas* canvas;

    SDL_Window* window;
    SDL_Renderer* renderer;

    static Keys convertKeyCharToKeyCode(const SDL_Keycode keyCode);

public:
    CanvasImpl(Canvas* canvas);
    ~CanvasImpl();

    void repaint();
    int getWidth();
    int getHeight();

    SDL_Renderer* getRenderer();
    void processEvents();
    void setWindowTitle(const std::string& title);
};