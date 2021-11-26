#include "Graphics.h"

Graphics::Graphics(SDL_Renderer *renderer) {
    this->renderer = renderer;
}

void Graphics::setColor(int r, int g, int b) {
    SDL_SetRenderDrawColor(renderer, (Uint8)r, (Uint8)g, (Uint8)b, 255);
}

void Graphics::setClip(int x, int y, int w, int h) {
    SDL_Rect clipRect {x, y, w, h};
    SDL_RenderSetClipRect(renderer, &clipRect);
}

void Graphics::fillRect(int x, int y, int w, int h) {
    SDL_Rect rect {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void Graphics::drawArc(int x, int y, int w, int h, int startAngle, int arcAngle) {
    // Draws an elliptical arc left-top at (x, y), with axes given by
    // xradius and yradius, traveling from startAngle to endangle.
    // Bresenham-based if complete
    int xradius = w/2, yradius = h/2;
    x += xradius;
    y += yradius;
    if (xradius == 0 && yradius == 0) {
        return;
    }
    if (arcAngle < startAngle)
        arcAngle += 360;
    // draw complete ellipse if (0, 360) specified
    // if (startAngle == 0 && arcAngle == 360) {
    //     _ellipse(x, y, xradius, yradius);
    //     return;
    // }
    for (int angle = startAngle; angle < arcAngle; angle++) {
        drawLine(x + int(xradius * cos (angle * PI_CONV)),
                y - int(yradius * sin (angle * PI_CONV)),
                x + int(xradius * cos ((angle + 1) * PI_CONV)),
                y - int(yradius * sin ((angle + 1) * PI_CONV)));
    }
}

void Graphics::_putpixel(int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void Graphics::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

int Graphics::getAnchorX(int x, int size, int anchor) {
    if ((anchor & LEFT) != 0) {
        return x;
    }
    if ((anchor & RIGHT) != 0) {
        return x - size;
    }
    if ((anchor & HCENTER) != 0) {
        return x - size / 2;
    }
    throw std::runtime_error("unknown anchor = " + std::to_string(anchor));
}

int Graphics::getAnchorY(int y, int size, int anchor) {
    if ((anchor & TOP) != 0) {
        return y;
    }
    if ((anchor & BOTTOM) != 0) {
        return y - size;
    }
    if ((anchor & VCENTER) != 0) {
        return y - size / 2;
    }
    throw std::runtime_error("unknown anchor = " + std::to_string(anchor));
}
