#include "GameMenu.h"

#include "Micro.h"
#include "TextRender.h"
#include "IGameMenuElement.h"
#include "GameCanvas.h"
#include "MenuManager.h"
#include "lcdui/Font.h"
#include "lcdui/Graphics.h"

GameMenu::GameMenu(std::string var1, Micro *micro, GameMenu *var3, std::vector<int8_t> var4) {
    field_94 = var1;
    field_95 = -1;
    this->micro = micro;
    gameMenu = var3;
    canvasWidth = micro->gameCanvas->getWidth();
    canvasHeight = micro->gameCanvas->getHeight();
    font = Font::getFont(64, 1, 16);
    font3 = Font::getFont(64, 0, 8);
    if (canvasWidth >= 128) {
        font2 = Font::getFont(64, 1, 0);
    } else {
        font2 = font3;
    }

    TextRender::setDefaultFont(font3);
    TextRender::setMaxArea(canvasWidth, canvasHeight);
    field_101 = 1;
    if (canvasWidth <= 100) {
        xPos = 6;
    } else {
        xPos = 9;
    }

    if (canvasHeight <= 100) {
        field_94 = "";
    }

    field_104 = xPos + 7;
    field_103 = 2;
    field_110 = 0;
    if (field_94 != "") {
        field_107 = (canvasHeight - (field_101 << 1) - 10 - font->getBaselinePosition()) / (font2->getBaselinePosition() + field_103);
    } else {
        field_107 = (canvasHeight - (field_101 << 1) - 10) / (font2->getBaselinePosition() + field_103);
    }

    if (!var4.empty()) {
        field_111 = true;
        nameCursorPos = 0;
        xPos = 8;
        strArr = var4;
    } else {
        field_111 = false;
    }

    if (field_107 > 13) {
        field_107 = 13;
    }

}

void GameMenu::method_68(int var1) {
    field_103 = var1;
}

void GameMenu::method_69(std::string var1) {
    field_94 = var1;
}

void GameMenu::method_70() {
    if (field_111) {
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

void GameMenu::method_71() {
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

void GameMenu::addMenuElement(IGameMenuElement *var1) {
    int var2 = field_101;
    field_107 = 1;
    vector.push_back(var1);
    if (field_94 != "") {
        var2 = font->getBaselinePosition() + 2;
    }

    if (canvasHeight < 100) {
        ++var2;
    } else {
        var2 += 4;
    }

    for (int var3 = 0; var3 < static_cast<int>(vector.size()) - 1; ++var3) {
        if (vector[var3]->isNotTextRender()) {
            var2 += font2->getBaselinePosition() + field_103;
        } else {
            var2 += (TextRender::getBaselinePosition() < GameCanvas::spriteSizeY[5] ? GameCanvas::spriteSizeY[5] : TextRender::getBaselinePosition()) + field_103;
        }

        if (var2 > canvasHeight - (field_101 << 1) - 10) {
            break;
        }

        ++field_107;
    }

    if (field_107 > 13) {
        field_107 = 13;
    }

    method_70();
}

void GameMenu::processGameActionDown() {
    if (field_111) {
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

void GameMenu::processGameActionUp() {
    if (field_111) {
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

void GameMenu::processGameActionUpd(int var1) {
    if (field_111) {
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
                IGameMenuElement *var3;
                if ((var3 = vector[var2]) != nullptr && var3->isNotTextRender()) {
                    var3->menuElemMethod(var1);
                    return;
                }
            }
        }
    }
}

void GameMenu::render_76(Graphics *graphics) {
    int var2;
    int i;
    if (field_111) {
        graphics->setColor(0, 0, 20);
        graphics->setFont(font);
        int8_t var7 = 1;
        graphics->drawString("Enter Name", xPos, var7, 20);
        var2 = var7 + font->getHeight() + (field_103 << 2);
        graphics->setFont(font2);

        for (i = 0; i < 3; ++i) {
            graphics->drawChar((char) strArr[i], xPos + i * font2->charWidth('W') + 1, var2, 17);
            if (i == nameCursorPos) {
                graphics->drawChar('^', xPos + i * font2->charWidth('W') + 1, var2 + font2->getHeight(), 17);
            }
        }

    } else {
        graphics->setColor(0, 0, 0);
        var2 = field_101;
        if (field_94 != "") {
            graphics->setFont(font);
            graphics->drawString(field_94, xPos, var2, 20);
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
            IGameMenuElement *var4 = vector[i];
            graphics->setColor(0, 0, 0);
            var4->render(graphics, var2, field_104);
            if (i == field_95 && var4->isNotTextRender()) {
                int var5 = xPos - micro->gameCanvas->helmetSpriteWidth / 2;
                int var6 = var2 + font2->getBaselinePosition() / 2 - micro->gameCanvas->helmetSpriteHeight / 2;
                graphics->setClip(var5, var6, micro->gameCanvas->helmetSpriteWidth, micro->gameCanvas->helmetSpriteHeight);
                graphics->drawImage(micro->gameCanvas->helmetImage, var5 - micro->gameCanvas->helmetSpriteWidth * (field_110 % 6), var6 - micro->gameCanvas->helmetSpriteHeight * (field_110 / 6), 20);
                graphics->setClip(0, 0, canvasWidth, canvasHeight);
                ++field_110;
                if (field_110 > 30) {
                    field_110 = 0;
                }
            }

            if (var4->isNotTextRender()) {
                var2 += font2->getBaselinePosition() + field_103;
            } else {
                var2 += (TextRender::getBaselinePosition() < GameCanvas::spriteSizeY[5] ? GameCanvas::spriteSizeY[5] : TextRender::getBaselinePosition()) + field_103;
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

void GameMenu::setGameMenu(GameMenu *gameMenu) {
    this->gameMenu = gameMenu;
}

GameMenu* GameMenu::getGameMenu() {
    return gameMenu;
}

int GameMenu::method_79() {
    return field_95;
}

void GameMenu::clearVector() {
    vector.clear();
    field_105 = 0;
    field_106 = 0;
    field_95 = -1;
}

std::string GameMenu::makeString() {
    return std::string(reinterpret_cast<char*>(strArr.data()));
}

std::vector<int8_t> GameMenu::getStrArr() {
    return strArr;
}

void GameMenu::method_83(int var1) {
    method_70();

    while (field_95 < var1) {
        ++field_95;
        if (field_95 > field_106) {
            ++field_105;
            ++field_106;
        }
    }
}
