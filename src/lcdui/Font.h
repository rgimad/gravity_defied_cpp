#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

#include <cmrc/cmrc.hpp>
#include "../config.h"

class Font {
public:
    enum FontSize {
        SIZE_SMALL = 8,
        SIZE_MEDIUM = 0,
        SIZE_LARGE = 16
    };

    enum FontStyle {
        STYLE_PLAIN = TTF_STYLE_NORMAL,
        STYLE_BOLD = TTF_STYLE_BOLD,
        STYLE_ITALIC = TTF_STYLE_ITALIC
    };

    enum FontFace {
        FACE_SYSTEM = 0
    };

    Font(FontStyle style, FontSize pointSize);
    ~Font();

    int getBaselinePosition() const;
    int getHeight() const;
    TTF_Font* getTtfFont() const;
    int charWidth(char c);
    int stringWidth(const std::string& s);
    int substringWidth(const std::string& string, int offset, int len);

private:
    static inline SDL_RWops* ttfRwOps = nullptr;
    TTF_Font* ttfFont;
    int height;

    static int getRealFontSize(FontSize size);
};