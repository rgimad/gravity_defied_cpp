#pragma once

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Canvas;

class CanvasImpl {
private:
    Canvas *canvas;

    SDL_Window *window;
    SDL_Renderer *renderer;

    const int width = 240;
    const int height = 320;

public:
    CanvasImpl(Canvas *canvas);
    ~CanvasImpl();

    void repaint();
    int getWidth();
    int getHeight();

    SDL_Renderer* getRenderer();
    bool quit();
    void delay(int ms);
};