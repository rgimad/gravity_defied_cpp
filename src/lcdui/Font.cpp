#include "Font.h"

#include <stdexcept>

CMRC_DECLARE(assets);

Font::Font(FontStyle style, FontSize pointSize)
{
    if (!ttfRwOps) {
        cmrc::embedded_filesystem internalFs = cmrc::assets::get_filesystem();
        cmrc::file fileData = internalFs.open("FontSansSerif.ttf");
        SDL_RWops* raw = SDL_RWFromConstMem(fileData.begin(), fileData.size());
        if (!raw) {
            throw std::runtime_error(SDL_GetError());
        }

        ttfRwOps = raw;
    }

    int realSize = getRealFontSize(pointSize);
    TTF_Font* font = TTF_OpenFontRW(ttfRwOps, SDL_TRUE, realSize);
    TTF_SetFontStyle(font, style);
    this->ttfFont = font;
    this->height = realSize;
}

Font::~Font()
{
    TTF_CloseFont(ttfFont);
}

int Font::getBaselinePosition() const
{
    return height;
}

int Font::getHeight() const
{
    return height;
}

TTF_Font* Font::getTtfFont() const
{
    return ttfFont;
}

int Font::charWidth(char c)
{
    return stringWidth(std::string(1, c));
}

int Font::stringWidth(const std::string& s)
{
    int width, height;
    if (TTF_SizeText(ttfFont, s.c_str(), &width, &height) == -1)
        throw std::runtime_error(TTF_GetError());
    return width;
}

int Font::substringWidth(const std::string& string, int offset, int len)
{
    return stringWidth(string.substr(offset, len));
}

int Font::getRealFontSize(FontSize size)
{
    switch (size) {
    case SIZE_LARGE:
        return 32;
    case SIZE_MEDIUM:
        return 16;
    case SIZE_SMALL:
        return 12;
    default:
        throw std::runtime_error("unknown font size: " + std::to_string(size));
    }
}
