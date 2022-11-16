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

// void Graphics::fillArc(int x, int y, int w, int h, int startAngle, int arcAngle) {
//     // NOTE this impl assumes startAngle < arcAngle
//     int  _x, _y,      // circle centered point
//         xx,yy,rr,   // x^2,y^2,r^2
//         ux,uy,      // u
//         vx,vy,      // v
//         sx,sy;      // pixel position

//     arcAngle += startAngle;

//     (void)w; (void)h;
//     int r = 30; // TODO
//     rr = r*r;
//     ux = double(r)*cos(double(startAngle)*PI_CONV);
//     uy = double(r)*sin(double(startAngle)*PI_CONV);
//     vx = double(r)*cos(double(arcAngle)*PI_CONV);
//     vy = double(r)*sin(double(arcAngle)*PI_CONV);

//     if (abs(arcAngle - startAngle) < 180) { // small pie
//         for (_y = -r, yy = _y*_y, sy = y + _y; _y <= +r; _y++, yy = _y*_y, sy++)
//             for (_x = -r, xx = _x*_x, sx = x + _x; _x <= +r; _x++, xx = _x*_x, sx++)
//                 if (xx + yy <= rr)           // inside circle
//                     if (((_x*uy) - (_y*ux) <= 0)  // x,y is above a0 in clockwise direction
//                         && ((_x*vy) - (_y*vx) >= 0)) // x,y is below a1 in counter clockwise direction
//                         _putpixel(sx, sy);
//     } else { // big pie
//         for (_y = -r, yy = _y*_y, sy = y + _y; _y <= +r; _y++, yy = _y*_y, sy++)
//             for (_x = -r, xx = _x*_x, sx = x + _x; _x <= +r; _x++, xx = _x*_x, sx++)
//                 if (xx + yy <= rr)           // inside circle
//                     if (((_x*uy) - (_y*ux) <= 0)  // x,y is above a0 in clockwise direction
//                         || ((_x*vy) - (_y*vx) >= 0)) // x,y is below a1 in counter clockwise direction
//                         _putpixel(sx, sy);
//     }
// }

void Graphics::fillArc(int x, int y, int w, int h, int startAngle, int arcAngle) {
    int endAngle = startAngle + arcAngle;
    double a = w/2.0, b = h/2.0;
    double e = sqrt(1.0 - (b*b)/(a*a));
    for (int _y = y - b; _y < y + b; _y++) {
        for (int _x = x - a; _x < x + a; _x++) {
            // _putpixel(_x, _y);
            // std::cout << atan2(_y, _x) << "\n";
            // SDL_Log("_y = %d, _x = %d atan2 = %lf\n", _y, _x, atan2(_y - y, _x - x));
            // if (atan2(_y - y, _x - x) >= startAngle*PI_CONV && atan2(_y - y, _x - x) <= endAngle*PI_CONV) {
            //     _putpixel(_x, _y);
            // }
            // if (atan2(_y - y, _x - x) < 3.14/2.0) {
            //     _putpixel(_x, _y);
            // }
            double ang = -atan2(_y - y, _x - x); // - because we assume all angles like in trigonom. circle
            
            double rad = b/sqrt(1 - e*e*cos(ang)*cos(ang));
            double dist = sqrt((_x - x)*(_x - x) + (_y - y)*(_y - y));
            
            if (ang >= startAngle*PI_CONV && ang <= endAngle*PI_CONV && dist <= rad) {
                _putpixel(_x, _y);
            }
        }
    }
}

void Graphics::_putpixel(int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void Graphics::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Graphics::drawImage(Image *image, int x, int y, int anchor) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image->getSurface());
    x = getAnchorX(x, image->getWidth(), anchor);
    y = getAnchorY(y, image->getHeight(), anchor);
    SDL_Rect dstRect{x, y, image->getWidth(), image->getHeight()};
    SDL_RenderCopy(renderer, texture, 0, &dstRect);
    SDL_DestroyTexture(texture);
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
    throw std::runtime_error("unknown xanchor = " + std::to_string(anchor));
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
    throw std::runtime_error("unknown yanchor = " + std::to_string(anchor));
}
