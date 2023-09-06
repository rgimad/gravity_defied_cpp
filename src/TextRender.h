#pragma once

#include <string>
#include <vector>
#include <memory>

#include "IGameMenuElement.h"

class Micro;
class Font;
class Graphics;

class TextRender : public IGameMenuElement {
private:
    std::string text;
    static inline std::shared_ptr<Font> defaultFont;
    std::shared_ptr<Font> font;
    int dx = 0;
    inline static int fieldMaxWidth = 100;
    inline static int fieldMaxHeightUnused = 100;
    bool isDrawSprite;
    int spriteNo;
    Micro* micro;

public:
    TextRender(std::string text, Micro* var2);
    static int getBaselinePosition();
    void setFont(std::shared_ptr<Font> font);
    static void setDefaultFont(std::shared_ptr<Font> font);
    static void setMaxArea(int w, int h);
    void setText(std::string text);
    bool isNotTextRender();
    void menuElemMethod(int var1);
    void render(Graphics* graphics, int y, int x);
    static std::vector<TextRender*> makeMultilineTextRenders(std::string text, Micro* micro);
    void setDx(int var1);
    void setDrawSprite(bool isDrawSprite, int spriteNo);
};
