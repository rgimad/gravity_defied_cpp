#include "TextRender.h"

#include "Micro.h"
#include "GameCanvas.h"
#include "lcdui/Font.h"
#include "lcdui/Graphics.h"

TextRender::TextRender(std::string text, Micro* micro)
{
    this->text = text;
    this->micro = micro;
    isDrawSprite = false;
    spriteNo = 0;
    font = nullptr;
}

int TextRender::getBaselinePosition()
{
    return defaultFont->getBaselinePosition();
}

void TextRender::setFont(std::shared_ptr<Font> font)
{
    this->font = font;
}

void TextRender::setDefaultFont(std::shared_ptr<Font> font)
{
    defaultFont = font;
}

void TextRender::setMaxArea(int w, int h)
{
    fieldMaxWidth = w;
    fieldMaxHeightUnused = h;
}

void TextRender::setText(std::string text)
{
    this->text = text;
}

bool TextRender::isNotTextRender()
{
    return false;
}

void TextRender::menuElemMethod(int index)
{
    (void)index; // UNUSED: parameter not used in implementation
}

void TextRender::render(Graphics* graphics, int y, int x)
{
    std::shared_ptr<Font> preservedFont = graphics->getFont();
    graphics->setFont(defaultFont);
    if (font) {
        graphics->setFont(font);
    }

    graphics->drawString(text, x + dx, y, 20);
    if (isDrawSprite) {
        micro->gameCanvas->drawSprite(graphics, spriteNo, x, y);
    }

    graphics->setFont(preservedFont);
}

std::vector<TextRender*> TextRender::makeMultilineTextRenders(std::string text, Micro* micro)
{
    std::size_t startPos = 0;
    std::size_t endPos = 0;
    int8_t padding = 25;

    std::vector<TextRender*> vector;
    for (; endPos < text.length(); startPos = ++endPos - 1) {
        std::size_t spacePos;
        if ((spacePos = text.find(" ", startPos)) == std::string::npos) {
            endPos = spacePos = text.length();
        }

        while (endPos < text.length() && defaultFont->substringWidth(text, startPos, spacePos - startPos) < fieldMaxWidth - padding) {
            endPos = spacePos + 1;
            if ((spacePos = text.find(" ", spacePos + 1)) == std::string::npos) {
                if (defaultFont->substringWidth(text, startPos, text.length() - 1 - startPos) <= fieldMaxWidth - padding) {
                    endPos = text.length();
                }
                break;
            }
        }

        vector.push_back(new TextRender(text.substr(startPos, endPos - startPos), micro));
    }

    return vector;
}

void TextRender::setDx(int dx)
{
    this->dx = dx;
}

void TextRender::setDrawSprite(bool isDrawSprite, int spriteNo)
{
    this->isDrawSprite = isDrawSprite;
    this->spriteNo = spriteNo;
}
