#include "Image.h"

Image::Image() {
    surface = nullptr;
}

Image::Image(SDL_Surface *surf) {
    surface = surf;
}

Image::~Image() {
    SDL_FreeSurface(surface);
}

Graphics* Image::getGraphics() {
    return new Graphics(SDL_CreateSoftwareRenderer(surface));
}

int Image::getWidth() {
    return surface->w;
}

int Image::getHeight() {
    return surface->h;
}

Image* Image::createImage(int w, int h) {
    return new Image(SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0));
}

Image* Image::createImage(const char *path) {
    SDL_Surface *surf = IMG_Load(path);
    if (!surf) {
        throw std::runtime_error(IMG_GetError());
    }
    surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    if (!surf) {
        throw std::runtime_error(SDL_GetError());
    }
    return new Image(surf);
}

SDL_Surface* Image::getSurface() {
    return surface;
}
