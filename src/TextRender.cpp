#include "TextRender.h"

#include "Micro.h"
#include "GameCanvas.h"
#include "lcdui/Font.h"
#include "lcdui/Graphics.h"

TextRender::TextRender(std::string text, Micro *var2) {
    text = text;
    micro = var2;
    isDrawSprite = false;
    spriteNo = 0;
    font = nullptr;
}

int TextRender::getBaselinePosition() {
    return defaultFont->getBaselinePosition();
}

void TextRender::setFont(Font *value) {
    font = value;
}

void TextRender::setDefaultFont(Font *value) {
    defaultFont = value;
}

void TextRender::setMaxArea(int w, int h) {
    fieldMaxWidth = w;
    fieldMaxHeightUnused = h;
}

void TextRender::setText(std::string text) {
    text = text;
}

bool TextRender::isNotTextRender() {
    return false;
}

void TextRender::menuElemMethod(int var1) {
}

void TextRender::render(Graphics *graphics, int y, int x) {
    Font *preservedFont = graphics->getFont();
    graphics->setFont(defaultFont);
    if (font != nullptr) {
        graphics->setFont(font);
    }

    graphics->drawString(text, x + dx, y, 20);
    if (isDrawSprite) {
        micro->gameCanvas->drawSprite(graphics, spriteNo, x, y);
    }

    graphics->setFont(preservedFont);
}

/*

static TextRender[] TextRender::makeMultilineTextRenders(std::string text, Micro micro) {
    int startPos = 0;
    int endPos = 0;
    int8_t var4 = 25;

    Vector vector;
    for (vector = new Vector(); endPos < text.length(); startPos = ++endPos - 1) {
        int var6;
        if ((var6 = text.indexOf(" ", startPos)) == -1) {
            endPos = var6 = text.length();
        }

        while (endPos < text.length() && defaultFont.substringWidth(text, startPos, var6 - startPos) < fieldMaxWidth - var4) {
            endPos = var6 + 1;
            if ((var6 = text.indexOf(" ", var6 + 1)) == -1) {
                if (defaultFont.substringWidth(text, startPos, text.length() - 1 - startPos) <= fieldMaxWidth - var4) {
                    endPos = text.length();
                }
                break;
            }
        }

        vector.addElement(new TextRender(text.substring(startPos, endPos), micro));
    }

    TextRender[] result = new TextRender[vector.size()];
    vector.copyInto(result);
    return result;
}

*/

void TextRender::setDx(int var1) {
    dx = var1;
}

void TextRender::setDrawSprite(bool isDrawSprite, int spriteNo) {
    isDrawSprite = isDrawSprite;
    spriteNo = spriteNo;
}
