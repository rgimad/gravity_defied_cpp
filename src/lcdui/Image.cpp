#include "Image.h"

#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmrc/cmrc.hpp>

CMRC_DECLARE(assets);

Image::Image(int width, int height)
{
    SDL_Surface* surf = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    if (!surf) {
        throw std::runtime_error(SDL_GetError());
    }

    this->surface = surf;
}

Image::Image(const std::string& embeddedPath)
{
    cmrc::embedded_filesystem embeddedFs = cmrc::assets::get_filesystem();
    cmrc::file fileData = embeddedFs.open(embeddedPath);

    SDL_RWops* raw = SDL_RWFromConstMem(fileData.begin(), fileData.size());
    if (!raw) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Surface* surf = IMG_Load_RW(raw, SDL_TRUE);
    if (!surf) {
        throw std::runtime_error(IMG_GetError());
    }

    SDL_Surface* surf_conv = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surf);

    if (!surf_conv) {
        throw std::runtime_error(SDL_GetError());
    }

    this->surface = surf_conv;
}

Image::~Image()
{
    SDL_FreeSurface(this->surface);
}

int Image::getWidth() const
{
    return this->surface->w;
}

int Image::getHeight() const
{
    return this->surface->h;
}

SDL_Surface* Image::getSurface() const
{
    return this->surface;
}
