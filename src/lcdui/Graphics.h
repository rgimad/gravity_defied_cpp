#pragma once
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Image.h"

constexpr auto PI_CONV = 3.1415926 / 180.0;

class Image;

class Graphics {
private:
    SDL_Renderer *renderer;
    // TODO: FontRenderContext frc;
    // void _ellipse(int cx, int cy, int xradius, int yradius);
    void _putpixel(int x, int y);

public:
    enum Anchors {
        HCENTER = 1,
        VCENTER = 2,
        LEFT = 4,
        RIGHT = 8,
        TOP = 16,
        BOTTOM = 32,
        BASELINE = 64
    };
    Graphics(SDL_Renderer *renderer);
    // TODO: void drawString(String s, int x, int y, int anchor);
    void setColor(int r, int g, int b);
    // TODO: void setFont(Font font);
    // TODO: Font getFont();
    void setClip(int x, int y, int w, int h);
    // TODO: void drawChar(char c, int x, int y, int anchor);
    void fillRect(int x, int y, int w, int h);
    // TODO: void fillArc(int x, int y, int w, int h, int startAngle, int arcAngle);
    void drawArc(int x, int y, int w, int h, int startAngle, int arcAngle);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawImage(Image *image, int x, int y, int anchor);
    static int getAnchorX(int x, int size, int anchor);
    static int getAnchorY(int y, int size, int anchor);
};