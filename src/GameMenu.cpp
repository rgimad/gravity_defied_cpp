#include "GameMenu.h"

#include "Micro.h"
#include "TextRender.h"
#include "IGameMenuElement.h"
#include "GameCanvas.h"
#include "MenuManager.h"
#include "lcdui/Font.h"
#include "lcdui/FontStorage.h"
#include "lcdui/Graphics.h"

GameMenu::GameMenu(std::string title, Micro* micro, GameMenu* parentMenu, char* inputString)
{
    menuTitle = title;
    selectedItemIndex = -1;
    this->micro = micro;
    gameMenu = parentMenu;
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
    marginPadding = 1;
    if (canvasWidth <= 100) {
        xPos = 6;
    } else {
        xPos = 9;
    }

    if (canvasHeight <= 100) {
        menuTitle = "";
    }

    renderXOffset = xPos + 7;
    itemSpacing = 2;
    helmetAnimFrame = 0;
    if (menuTitle != "") {
        maxVisibleItems = (canvasHeight - (marginPadding << 1) - 10 - font->getBaselinePosition()) / (font2->getBaselinePosition() + itemSpacing);
    } else {
        maxVisibleItems = (canvasHeight - (marginPadding << 1) - 10) / (font2->getBaselinePosition() + itemSpacing);
    }

    if (inputString) {
        isInputMode = true;
        nameCursorPos = 0;
        xPos = 8;
        strArr = inputString;
    } else {
        isInputMode = false;
    }

    if (maxVisibleItems > 13) {
        maxVisibleItems = 13;
    }
}

void GameMenu::setItemSpacing(int spacing)
{
    itemSpacing = spacing;
}

void GameMenu::setMenuTitle(std::string title)
{
    menuTitle = title;
}

void GameMenu::resetToFirstItem()
{
    if (isInputMode) {
        nameCursorPos = 0;
    } else {
        if (!vector.empty()) {
            selectedItemIndex = 0;

            for (int i = 0; i < static_cast<int>(vector.size()) && i < maxVisibleItems; ++i) {
                if (vector[i]->isNotTextRender()) {
                    selectedItemIndex = i;
                    break;
                }
            }

            scrollOffsetFirst = 0;
            scrollOffsetLast = vector.size() - 1;
            if (scrollOffsetLast > maxVisibleItems - 1) {
                scrollOffsetLast = maxVisibleItems - 1;
            }
        }
    }
}

void GameMenu::resetToLastItem()
{
    selectedItemIndex = vector.size() - 1;

    for (int i = vector.size() - 1; i > 0; --i) {
        if (vector[i]->isNotTextRender()) {
            selectedItemIndex = i;
            break;
        }
    }

    scrollOffsetFirst = vector.size() - maxVisibleItems;
    if (scrollOffsetFirst < 0) {
        scrollOffsetFirst = 0;
    }

    scrollOffsetLast = vector.size() - 1;
    if (scrollOffsetLast > selectedItemIndex + maxVisibleItems) {
        scrollOffsetLast = selectedItemIndex + maxVisibleItems;
    }
}

void GameMenu::addMenuElement(IGameMenuElement* element)
{
    int yPos = marginPadding;
    maxVisibleItems = 1;
    vector.push_back(element);
    if (menuTitle != "") {
        yPos = font->getBaselinePosition() + 2;
    }

    if (canvasHeight < 100) {
        ++yPos;
    } else {
        yPos += 4;
    }

    for (int i = 0; i < static_cast<int>(vector.size()) - 1; ++i) {
        if (vector[i]->isNotTextRender()) {
            yPos += font2->getBaselinePosition() + itemSpacing;
        } else {
            yPos += (TextRender::getBaselinePosition() < GameCanvas::spriteSizeY[5] ? GameCanvas::spriteSizeY[5] : TextRender::getBaselinePosition()) + itemSpacing;
        }

        if (yPos > canvasHeight - (marginPadding << 1) - 10) {
            break;
        }

        ++maxVisibleItems;
    }

    if (maxVisibleItems > 13) {
        maxVisibleItems = 13;
    }

    resetToFirstItem();
}

void GameMenu::processGameActionDown()
{
    if (isInputMode) {
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
        if (!(vector[selectedItemIndex]->isNotTextRender())) {
            ++scrollOffsetLast;
            selectedItemIndex = scrollOffsetLast;
            ++scrollOffsetFirst;
            return;
        }

        ++selectedItemIndex;
        if (selectedItemIndex > static_cast<int>(vector.size()) - 1) {
            resetToFirstItem();
            return;
        }

        bool foundNonText = false;

        int i;
        for (i = selectedItemIndex; i <= scrollOffsetLast + 1; ++i) {
            if (vector[i]->isNotTextRender()) {
                foundNonText = true;
                break;
            }
        }

        if (foundNonText) {
            selectedItemIndex = i;
        } else if (scrollOffsetLast < static_cast<int>(vector.size()) - 1) {
            ++scrollOffsetLast;
            ++scrollOffsetFirst;
        } else {
            --selectedItemIndex;
        }

        if (selectedItemIndex > scrollOffsetLast) {
            ++scrollOffsetFirst;
            ++scrollOffsetLast;
            if (scrollOffsetLast > static_cast<int>(vector.size()) - 1) {
                scrollOffsetLast = vector.size() - 1;
            }

            selectedItemIndex = scrollOffsetLast;
        }
    }
}

void GameMenu::processGameActionUp()
{
    if (isInputMode) {
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
        --selectedItemIndex;
        if (selectedItemIndex < 0) {
            resetToLastItem();
            return;
        }

        bool foundNonText = false;

        int i;
        for (i = selectedItemIndex; i >= scrollOffsetFirst; --i) {
            if (vector[i]->isNotTextRender()) {
                foundNonText = true;
                break;
            }
        }

        if (!foundNonText) {
            if (scrollOffsetFirst > 0) {
                --scrollOffsetFirst;
                if (static_cast<int>(vector.size()) > maxVisibleItems - 1) {
                    --scrollOffsetLast;
                    return;
                }
            } else {
                resetToLastItem();
            }

            return;
        }

        selectedItemIndex = i;
        if (selectedItemIndex < scrollOffsetFirst) {
            --scrollOffsetFirst;
            if (scrollOffsetFirst < 0) {
                selectedItemIndex = 0;
                scrollOffsetFirst = 0;
            }

            if (static_cast<int>(vector.size()) > maxVisibleItems - 1) {
                --scrollOffsetLast;
            }
        }
    }
}

void GameMenu::processGameActionUpd(int action)
{
    if (isInputMode) {
        switch (action) {
        case 1:
            if (nameCursorPos == 2) {
                micro->menuManager->switchToMenu(gameMenu, false);
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
        if (selectedItemIndex != -1) {
            for (int i = selectedItemIndex; i < static_cast<int>(vector.size()); ++i) {
                IGameMenuElement* element;
                if ((element = vector[i]) != nullptr && element->isNotTextRender()) {
                    element->menuElemMethod(action);
                    return;
                }
            }
        }
    }
}

void GameMenu::render(Graphics* graphics)
{
    int yPos;
    int i;
    if (isInputMode) {
        graphics->setColor(0, 0, 20);
        graphics->setFont(font);
        int8_t yStart = 1;
        graphics->drawString("Enter Name", xPos, yStart, 20);
        yPos = yStart + font->getHeight() + (itemSpacing << 2);
        graphics->setFont(font2);

        for (i = 0; i < 3; ++i) {
            graphics->drawChar((char)strArr[i], xPos + i * font2->charWidth('W') + 1, yPos, 17);
            if (i == nameCursorPos) {
                graphics->drawChar('^', xPos + i * font2->charWidth('W') + 1, yPos + font2->getHeight(), 17);
            }
        }

    } else {
        graphics->setColor(0, 0, 0);
        yPos = marginPadding;
        if (menuTitle != "") {
            graphics->setFont(font);
            graphics->drawString(menuTitle, xPos, yPos, 20);
            yPos += font->getBaselinePosition() + 2;
        }

        if (scrollOffsetFirst > 0) {
            micro->gameCanvas->drawSprite(graphics, 2, xPos - 3, yPos);
        }

        if (canvasHeight < 100) {
            ++yPos;
        } else {
            yPos += 4;
        }

        graphics->setFont(font2);

        for (i = scrollOffsetFirst; i < scrollOffsetLast + 1; ++i) {
            IGameMenuElement* element = vector[i];
            graphics->setColor(0, 0, 0);
            element->render(graphics, yPos, renderXOffset);
            if (i == selectedItemIndex && element->isNotTextRender()) {
                int helmetX = xPos - micro->gameCanvas->helmetSpriteWidth / 2;
                int helmetY = yPos + font2->getBaselinePosition() / 2 - micro->gameCanvas->helmetSpriteHeight / 2;
                graphics->setClip(helmetX, helmetY, micro->gameCanvas->helmetSpriteWidth, micro->gameCanvas->helmetSpriteHeight);
                graphics->drawImage(micro->gameCanvas->helmetImage.get(), helmetX - micro->gameCanvas->helmetSpriteWidth * (helmetAnimFrame % 6), helmetY - micro->gameCanvas->helmetSpriteHeight * (helmetAnimFrame / 6), 20);
                graphics->setClip(0, 0, canvasWidth, canvasHeight);
                ++helmetAnimFrame;
                if (helmetAnimFrame > 30) {
                    helmetAnimFrame = 0;
                }
            }

            if (element->isNotTextRender()) {
                yPos += font2->getBaselinePosition() + itemSpacing;
            } else {
                yPos += (TextRender::getBaselinePosition() < GameCanvas::spriteSizeY[5] ? GameCanvas::spriteSizeY[5] : TextRender::getBaselinePosition()) + itemSpacing;
            }
        }

        if (static_cast<int>(vector.size()) > scrollOffsetLast && scrollOffsetLast != static_cast<int>(vector.size()) - 1) {
            if (GameCanvas::spriteSizeY[3] + yPos > canvasHeight) {
                micro->gameCanvas->drawSprite(graphics, 3, xPos - 3, canvasHeight - GameCanvas::spriteSizeY[3]);
                return;
            }

            micro->gameCanvas->drawSprite(graphics, 3, xPos - 3, yPos - 2);
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

int GameMenu::getSelectedItemIndex()
{
    return selectedItemIndex;
}

void GameMenu::clearVector()
{
    vector.clear();
    scrollOffsetFirst = 0;
    scrollOffsetLast = 0;
    selectedItemIndex = -1;
}

std::string GameMenu::makeString()
{
    return std::string(strArr);
}

char* GameMenu::getStrArr() const
{
    return strArr;
}

void GameMenu::navigateToItem(int targetIndex)
{
    resetToFirstItem();

    while (selectedItemIndex < targetIndex) {
        ++selectedItemIndex;
        if (selectedItemIndex > scrollOffsetLast) {
            ++scrollOffsetFirst;
            ++scrollOffsetLast;
        }
    }
}
