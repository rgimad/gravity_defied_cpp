#pragma once

#include "Font.h"

#include <memory>
#include <map>

typedef std::map<std::pair<Font::FontStyle, Font::FontSize>, std::shared_ptr<Font>> FontsMap;

class FontStorage {
public:
    static std::shared_ptr<Font> getFont(Font::FontStyle style, Font::FontSize size);
    static void clearAll();

private:
    static inline FontsMap fontsMap;
};
