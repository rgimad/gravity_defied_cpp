#include "Image.h"
#include <cmrc/cmrc.hpp>

CMRC_DECLARE(assets);

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

Image* Image::createImage(std::string path) {
    auto internalFs = cmrc::assets::get_filesystem();
    auto fileData = internalFs.open(path);

    SDL_RWops *raw = SDL_RWFromConstMem(fileData.begin(), fileData.size());
    if (!raw) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Surface *surf = IMG_Load_RW(raw, SDL_TRUE);
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
