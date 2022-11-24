#include "Font.h"

#include <stdexcept>
#include <cassert>

#define FONT_PATH "./FontSansSerif.ttf"

Font::Font(TTF_Font *font, int pointSize) {
    assert(font != nullptr);
    this->font = font;
    this->height = pointSize;
}

Font::~Font() {
    assert(font != nullptr);
    TTF_CloseFont(font);
}

int Font::getBaselinePosition() {
    return height;
}

int Font::getHeight() {
    return height;
}

int Font::charWidth(char c) {
    return stringWidth(std::string(1, c));
}

int Font::stringWidth(const std::string &s) {
    int width, height;
    if (TTF_SizeText(font, s.c_str(), &width, &height) == -1) throw std::runtime_error(TTF_GetError());
    return width;
}

int Font::substringWidth(const std::string &string, int offset, int len) {
    return stringWidth(string.substr(offset, len));
}

// face: now unused
Font* Font::getFont(int face, int style, int size) {
    (void)face;
    size = Font::getRealFontSize(size);
    TTF_Font *font = TTF_OpenFont(FONT_PATH, size);
    if (!font) throw std::runtime_error(TTF_GetError());
    TTF_SetFontStyle(font, style); // {plain: 0, bold: 1, italic: 2}
    return new Font(font, size);
}

int Font::getRealFontSize(int size) {
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

Font* Font::getDefaultFont() {
    return Font::getFont(0, 0, Font::SIZE_MEDIUM);
}