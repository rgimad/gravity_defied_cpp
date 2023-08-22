#pragma once
#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Graphics.h"

class Graphics;

class Image {
private:
    SDL_Surface *surface;
public:
    Image();
    Image(SDL_Surface *surf);
    ~Image();
    Graphics *getGraphics();
    int getWidth();
    int getHeight();
    static Image *createImage(int w, int h);
    static Image *createImage(std::string path);
    SDL_Surface* getSurface();
};
