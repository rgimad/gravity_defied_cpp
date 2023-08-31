#pragma once

#include <string>
#include <SDL2/SDL.h>

class Image {
private:
    SDL_Surface* surface;

public:
    Image(const std::string& embeddedPath);
    Image(int width, int height);
    ~Image();

    int getWidth() const;
    int getHeight() const;
    SDL_Surface* getSurface() const;
};
