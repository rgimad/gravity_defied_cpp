#include "GameMenu.h"

#include "Micro.h"
#include "TextRender.h"
#include "IGameMenuElement.h"
#include "GameCanvas.h"
#include "MenuManager.h"
#include "lcdui/Font.h"
#include "lcdui/FontStorage.h"
#include "lcdui/Graphics.h"

GameMenu::GameMenu(std::string var1, Micro* micro, GameMenu* var3, char* inputString)
{
    name = var1;
    field_95 = -1;
    this->micro = micro;
    gameMenu = var3;
    canvasWidth = micro->gameCanvas->getWidth();
    canvasHeight = micro->gameCanvas->getHeight();

    font = FontStorage::getFont(Font::STYLE_BOLD, Font::SIZE_LARGE);
    font3 = FontStorage::getFont(Font::STYLE_PLAIN, Font::SIZE_SMALL);

    if (canvasWidth >= 128) {
        font2 = FontStorage::getFont(Font::STYLE_BOLD, Font::SIZE_MEDIUM);
    } else {
        font2 = font3;
    }

    TextRender::setDefaultFont(font3);
    TextRender::setMaxArea(canvasWidth, canvasHeight);
    menuOffsetY = 1;

    if (canvasWidth <= 100) {
        xPos = 6;
    } else {
        xPos = 9;
    }

    if (canvasHeight <= 100) {
        name = "";
    }

    menuOffsetX = xPos + 7;
    menuSpacing = 2;
    selectedMenuItemTickSpriteNo = 0;

    if (name != "") {
        field_107 = (canvasHeight - (menuOffsetY << 1) - 10 - font->getBaselinePosition()) / (font2->getBaselinePosition() + menuSpacing);
    } else {
        field_107 = (canvasHeight - (menuOffsetY << 1) - 10) / (font2->getBaselinePosition() + menuSpacing);
    }

    if (inputString) {
        isTextInput = true;
        nameCursorPos = 0;
        xPos = 8;
        strArr = inputString;
    } else {
        isTextInput = false;
    }

    if (field_107 > 13) {
        field_107 = 13;
    }
}

void GameMenu::method_70()
{
    if (isTextInput) {
        nameCursorPos = 0;
    } else {
        if (!vector.empty()) {
            field_95 = 0;

            for (int var1 = 0; var1 < static_cast<int>(vector.size()) && var1 < field_107; ++var1) {
                if (vector[var1]->isNotTextRender()) {
                    field_95 = var1;
                    break;
                }
            }

            field_105 = 0;
            field_106 = vector.size() - 1;

            if (field_106 > field_107 - 1) {
                field_106 = field_107 - 1;
            }
        }
    }
}

void GameMenu::method_71()
{
    field_95 = vector.size() - 1;

    for (int var1 = vector.size() - 1; var1 > 0; --var1) {
        if (vector[var1]->isNotTextRender()) {
            field_95 = var1;
            break;
        }
    }

    field_105 = vector.size() - field_107;

    if (field_105 < 0) {
        field_105 = 0;
    }

    field_106 = vector.size() - 1;

    if (field_106 > field_95 + field_107) {
        field_106 = field_95 + field_107;
    }
}

void GameMenu::addMenuElement(IGameMenuElement* var1)
{
    int var2 = menuOffsetY;
    field_107 = 1;
    vector.push_back(var1);

    if (name != "") {
        var2 = font->getBaselinePosition() + 2;
    }

    if (canvasHeight < 100) {
        ++var2;
    } else {
        var2 += 4;
    }

    for (int var3 = 0; var3 < static_cast<int>(vector.size()) - 1; ++var3) {
        if (vector[var3]->isNotTextRender()) {
            var2 += font2->getBaselinePosition() + menuSpacing;
        } else {
            var2 += (TextRender::getBaselinePosition() < GameCanvas::spriteSizeY[5] ? GameCanvas::spriteSizeY[5] : TextRender::getBaselinePosition()) + menuSpacing;
        }

        if (var2 > canvasHeight - (menuOffsetY << 1) - 10) {
            break;
        }

        ++field_107;
    }

    if (field_107 > 13) {
        field_107 = 13;
    }

    method_70();
}

void GameMenu::processGameActionDown()
{
    if (isTextInput) {
        if (strArr[nameCursorPos] == 32) {
            strArr[nameCursorPos] = 90;
            return;
        }

        --strArr[nameCursorPos];

        if (strArr[nameCursorPos] < 65) {
            strArr[nameCursorPos] = 32;
            return;
        }
    } else if (vector.size() != 0) {
        if (!(vector[field_95]->isNotTextRender())) {
            ++field_106;
            field_95 = field_106;
            ++field_105;
            return;
        }

        ++field_95;

        if (field_95 > static_cast<int>(vector.size()) - 1) {
            method_70();
            return;
        }

        bool var3 = false;

        int var2;

        for (var2 = field_95; var2 <= field_106 + 1; ++var2) {
            if (vector[var2]->isNotTextRender()) {
                var3 = true;
                break;
            }
        }

        if (var3) {
            field_95 = var2;
        } else if (field_106 < static_cast<int>(vector.size()) - 1) {
            ++field_106;
            ++field_105;
        } else {
            --field_95;
        }

        if (field_95 > field_106) {
            ++field_105;
            ++field_106;

            if (field_106 > static_cast<int>(vector.size()) - 1) {
                field_106 = vector.size() - 1;
            }

            field_95 = field_106;
        }
    }
}

void GameMenu::processGameActionUp()
{
    if (isTextInput) {
        if (strArr[nameCursorPos] == 32) {
            strArr[nameCursorPos] = 65;
            return;
        }

        ++strArr[nameCursorPos];

        if (strArr[nameCursorPos] > 90) {
            strArr[nameCursorPos] = 32;
            return;
        }
    } else if (vector.size() != 0) {
        --field_95;

        if (field_95 < 0) {
            method_71();
            return;
        }

        bool var3 = false;
        int var2;

        for (var2 = field_95; var2 >= field_105; --var2) {
            if (vector[var2]->isNotTextRender()) {
                var3 = true;
                break;
            }
        }

        if (!var3) {
            if (field_105 > 0) {
                --field_105;

                if (static_cast<int>(vector.size()) > field_107 - 1) {
                    --field_106;
                    return;
                }
            } else {
                method_71();
            }

            return;
        }

        field_95 = var2;

        if (field_95 < field_105) {
            --field_105;

            if (field_105 < 0) {
                field_95 = 0;
                field_105 = 0;
            }

            if (static_cast<int>(vector.size()) > field_107 - 1) {
                --field_106;
            }
        }
    }
}

void GameMenu::processGameActionUpd(int var1)
{
    if (isTextInput) {
        switch (var1) {
        case 1:
            if (nameCursorPos == 2) {
                micro->menuManager->method_1(gameMenu, false);
                return;
            }

            ++nameCursorPos;
            return;
        case 2:
            ++nameCursorPos;

            if (nameCursorPos > 2) {
                nameCursorPos = 2;
                return;
            }

            break;
        case 3:
            --nameCursorPos;

            if (nameCursorPos < 0) {
                nameCursorPos = 0;
            }
        }

    } else {
        if (field_95 != -1) {
            for (int var2 = field_95; var2 < static_cast<int>(vector.size()); ++var2) {
                IGameMenuElement* var3;

                if ((var3 = vector[var2]) != nullptr && var3->isNotTextRender()) {
                    var3->menuElemMethod(var1);
                    return;
                }
            }
        }
    }
}

void GameMenu::render_76(Graphics* graphics)
{
    int var2;
    int i;

    if (isTextInput) {
        graphics->setColor(0, 0, 20);
        graphics->setFont(font);
        int8_t var7 = 1;
        graphics->drawString(
            "Enter Name",
            xPos,
            var7,
            Graphics::TOP | Graphics::LEFT);
        var2 = var7 + font->getHeight() + (menuSpacing << 2);
        graphics->setFont(font2);

        for (i = 0; i < 3; ++i) {
            graphics->drawChar(
                (char)strArr[i],
                xPos + i * font2->charWidth('W') + 1,
                var2,
                Graphics::TOP | Graphics::HCENTER);

            if (i == nameCursorPos) {
                graphics->drawChar(
                    '^',
                    xPos + i * font2->charWidth('W') + 1,
                    var2 + font2->getHeight(),
                    Graphics::TOP | Graphics::HCENTER);
            }
        }

    } else {
        graphics->setColor(0, 0, 0);
        var2 = menuOffsetY;

        if (name != "") {
            graphics->setFont(font);
            graphics->drawString(
                name,
                xPos,
                var2,
                Graphics::TOP | Graphics::LEFT);
            var2 += font->getBaselinePosition() + 2;
        }

        if (field_105 > 0) {
            micro->gameCanvas->drawSprite(graphics, 2, xPos - 3, var2);
        }

        if (canvasHeight < 100) {
            ++var2;
        } else {
            var2 += 4;
        }

        graphics->setFont(font2);

        for (i = field_105; i < field_106 + 1; ++i) {
            IGameMenuElement* var4 = vector[i];
            graphics->setColor(0, 0, 0);
            var4->render(graphics, var2, menuOffsetX);

            // TODO: field_95 - selected menu items idx ??
            if (i == field_95 && var4->isNotTextRender()) {
                const int selectedMenuItemTickX = xPos - micro->gameCanvas->helmetSpriteWidth / 2;
                const int selectedMenuItemTickY = var2 + font2->getBaselinePosition() / 2; // - micro->gameCanvas->helmetSpriteHeight / 2;
                graphics->setClip(
                    selectedMenuItemTickX,
                    selectedMenuItemTickY,
                    micro->gameCanvas->helmetSpriteWidth,
                    micro->gameCanvas->helmetSpriteHeight);
                graphics->drawImage(
                    micro->gameCanvas->helmetImage.get(),
                    selectedMenuItemTickX - micro->gameCanvas->helmetSpriteWidth * (selectedMenuItemTickSpriteNo % 6),
                    selectedMenuItemTickY - micro->gameCanvas->helmetSpriteHeight * (selectedMenuItemTickSpriteNo / 6),
                    Graphics::TOP | Graphics::LEFT);
                graphics->setClip(0, 0, canvasWidth, canvasHeight);
                ++selectedMenuItemTickSpriteNo;

                if (selectedMenuItemTickSpriteNo > 30) {
                    selectedMenuItemTickSpriteNo = 0;
                }
            }

            if (var4->isNotTextRender()) {
                var2 += font2->getBaselinePosition() + menuSpacing;
            } else {
                var2 += (TextRender::getBaselinePosition() < GameCanvas::spriteSizeY[5] ? GameCanvas::spriteSizeY[5] : TextRender::getBaselinePosition()) + menuSpacing;
            }
        }

        if (static_cast<int>(vector.size()) > field_106 && field_106 != static_cast<int>(vector.size()) - 1) {
            if (GameCanvas::spriteSizeY[3] + var2 > canvasHeight) {
                micro->gameCanvas->drawSprite(graphics, 3, xPos - 3, canvasHeight - GameCanvas::spriteSizeY[3]);
                return;
            }

            micro->gameCanvas->drawSprite(graphics, 3, xPos - 3, var2 - 2);
        }
    }
}

void GameMenu::setGameMenu(GameMenu* gameMenu)
{
    this->gameMenu = gameMenu;
}

GameMenu* GameMenu::getGameMenu()
{
    return gameMenu;
}

void GameMenu::clearVector()
{
    vector.clear();
    field_105 = 0;
    field_106 = 0;
    field_95 = -1;
}

std::string GameMenu::makeString()
{
    return std::string(strArr);
}

char* GameMenu::getStrArr() const
{
    return strArr;
}

void GameMenu::method_83(int var1)
{
    method_70();

    while (field_95 < var1) {
        ++field_95;
        if (field_95 > field_106) {
            ++field_105;
            ++field_106;
        }
    }
}
