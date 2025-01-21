#include "Font.h"
#include "FontStorage.h"

std::shared_ptr<Font> FontStorage::getFont(Font::FontStyle style, Font::FontSize size)
{
    auto pair = std::make_pair(style, size);
    if (fontsMap.find(pair) != fontsMap.end()) {
        return fontsMap[pair];
    } else {
        auto font = std::make_shared<Font>(style, size);
        fontsMap.insert({ pair, font });
        return font;
    }
}

void FontStorage::clearAll()
{
    fontsMap.clear();
}
