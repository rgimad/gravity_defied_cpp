#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

using namespace std;

class Font {
private:
    int height;
public:
    enum FontSize {
        SIZE_SMALL = 8,
        SIZE_MEDIUM = 0,
        SIZE_LARGE = 16
    };

    Font(TTF_Font *font, int pointSize);
    ~Font();

    int getBaselinePosition();
    int getHeight();
    int charWidth(char c);
    int stringWidth(string s);
    int substringWidth(string string, int offset, int len);
    static Font *getFont(int face, int style, int size);
    static int getRealFontSize(int size);
    static Font *getDefaultFont();

    TTF_Font *font;
};