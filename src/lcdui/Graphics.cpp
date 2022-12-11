#include "Graphics.h"

Graphics::Graphics(SDL_Renderer *renderer) {
    this->renderer = renderer;
    this->currentColor = {0, 0, 0, 255};
}

void Graphics::drawString(const std::string &s, int x, int y, int anchor) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font->font, s.c_str(), currentColor);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    int width, height;
    if (TTF_SizeText(font->font, s.c_str(), &width, &height) == -1) throw std::runtime_error(TTF_GetError());

    x = getAnchorX(x, width, anchor);
    y = getAnchorY(y, height, anchor);
    SDL_Rect dstRect{x, y, width, height};

    SDL_RenderCopy(renderer, message, nullptr, &dstRect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void Graphics::setColor(int r, int g, int b) {
    currentColor.r = r;
    currentColor.g = g;
    currentColor.b = b;
    currentColor.a = 255;
    SDL_SetRenderDrawColor(renderer, (Uint8)r, (Uint8)g, (Uint8)b, 255);
}

void Graphics::setFont(Font *font) {
    this->font = font;
}

Font* Graphics::getFont() {
    return font;
}

void Graphics::setClip(int x, int y, int w, int h) {
    SDL_Rect clipRect {x, y, w, h};
    SDL_RenderSetClipRect(renderer, &clipRect);
}

void Graphics::drawChar(char c, int x, int y, int anchor) {
    drawString(std::string(1, c), x, y, anchor);
}

void Graphics::fillRect(int x, int y, int w, int h) {
    SDL_Rect rect {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * Draws the outline of a circular or elliptical arc covering the specified rectangle,
 * using the current color and stroke style. The resulting arc begins at startAngle
 * and extends for arcAngle degrees, using the current color. Angles are interpreted
 * such that 0 degrees is at the 3 o'clock position. A positive value indicates a
 * counter-clockwise rotation while a negative value indicates a clockwise rotation.
 *
 * The center of the arc is the center of the rectangle whose origin is (x, y) and
 * whose size is specified by the width and height arguments.
 *
 * The resulting arc covers an area width + 1 pixels wide by height + 1 pixels tall.
 * If either width or height is less than zero, nothing is drawn.
 *
 * The angles are specified relative to the non-square extents of the bounding rectangle
 * such that 45 degrees always falls on the line from the center of the ellipse to the
 * upper right corner of the bounding rectangle. As a result, if the bounding rectangle
 * is noticeably longer in one axis than the other, the angles to the start and end of
 * the arc segment will be skewed farther along the longer axis of the bounds.
 *
 * Parameters:
 * x - the x coordinate of the upper-left corner of the arc to be drawn
 * y - the y coordinate of the upper-left corner of the arc to be drawn
 * width - the width of the arc to be drawn
 * height - the height of the arc to be drawn
 * startAngle - the beginning angle
 * arcAngle - the angular extent of the arc, relative to the start angle
 */
void Graphics::drawArc(int x, int y, int width, int heigth, int startAngle, int arcAngle) {
    // Draws an elliptical arc left-top at (x, y), with axes given by
    // xradius and yradius, traveling from startAngle to endangle.
    // Bresenham-based if complete
    int xradius = width/2, yradius = heigth/2;
    x += xradius;
    y += yradius;
    if (xradius == 0 && yradius == 0) {
        return;
    }

    // draw complete ellipse if (0, 360) specified
    // if (startAngle == 0 && arcAngle == 360) {
    //     _ellipse(x, y, xradius, yradius);
    //     return;
    // }

    for (int angle = startAngle; angle < startAngle + arcAngle; angle++) {
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

// int norm_ang(int angle) {
    // // reduce the angle  
    // angle =  angle % 360; 

    // // force it to be the positive remainder, so that 0 <= angle < 360  
    // angle = (angle + 360) % 360;  

    // // force into the minimum absolute value residue class, so that -180 < angle <= 180  
    // if (angle > 180)  
    //     angle -= 360;  
    // return angle;
// }

int to_360(int ang) {
    // if (ang != 0 && ang % 360 == 0) {
    //     ang = 360;
    // } else {
    //     ang %= 360;
    // }
    if (ang >= 0 && ang <= 360) {
        return ang;
    }
    if (ang < 0) {
        ang += 360;
    }
    return ang;
}

void Graphics::fillArc(int x, int y, int w, int h, int startAngle, int arcAngle) {
    int endAngle = startAngle + arcAngle;
    // startAngle = norm_ang(startAngle);
    // endAngle = norm_ang(endAngle);
    // SDL_Log("startAngle = %d, endAngle = %d\n", startAngle, endAngle);
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
            double ang = atan2(-(_y - y), _x - x); // cuz in screen y grows downwards (in maths y grows upwards)
            
            // double rad = b/sqrt(1 - e*e*cos(ang)*cos(ang));
            // double dist = sqrt((_x - x)*(_x - x) + (_y - y)*(_y - y));
            double rad = b*b/(1 - e*e*cos(ang)*cos(ang));
            double dist = ((_x - x)*(_x - x) + (_y - y)*(_y - y));

            int ang2 = to_360(ang/PI_CONV);
            
            if (ang2 >= to_360(startAngle)
                && ang2 <= to_360(endAngle)
                && dist <= rad)
            {
                _putpixel(_x, _y);
            }

            if (endAngle > 360) {
                if (ang2 < endAngle % 360 && dist <= rad) {
                    _putpixel(_x, _y);
                }
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