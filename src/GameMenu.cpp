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
    clearVector();
    name = var1;
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

    xPos = 9;
    menuOffsetX = xPos + 7;
    menuSpacing = 2;
    selectedMenuItemTickSpriteNo = 0;

    numberOfItemsToFit = getNumberOfItemToFitOnScreen();

    if (inputString) {
        isTextInput = true;
        nameCursorPos = 0;
        xPos = 8;
        strArr = inputString;
    } else {
        isTextInput = false;
    }
}

void GameMenu::rolloverMenuToBottom()
{
    if (isTextInput) {
        nameCursorPos = 0;
    } else {
        if (menuItems.empty()) {
            return;
        }

        viewItemIdx = 0;

        int itemsToShowWindowStart = 0;
        int itemsToShowWindowEnd = 0;
        calculateMenuWindowBoundaries(&itemsToShowWindowStart, &itemsToShowWindowEnd);

        for (int i = itemsToShowWindowStart; i <= itemsToShowWindowEnd; ++i) {
            if (menuItems[i]->isNotTextRender()) {
                selectedItemIdx = i;
                viewItemIdx = i;
                return;
            }
        }
    }
}

void GameMenu::rolloverMenuToTop()
{
    viewItemIdx = menuItems.size() - 1;

    int itemsToShowWindowStart = 0;
    int itemsToShowWindowEnd = 0;
    calculateMenuWindowBoundaries(&itemsToShowWindowStart, &itemsToShowWindowEnd);

    for (int i = itemsToShowWindowEnd; i >= itemsToShowWindowStart; --i) {
        if (menuItems[i]->isNotTextRender()) {
            selectedItemIdx = i;
            viewItemIdx = i;
            return;
        }
    }
}

void GameMenu::addMenuElement(IGameMenuElement* var1)
{
    menuItems.push_back(var1);
    numberOfItemsToFit = getNumberOfItemToFitOnScreen();
    rolloverMenuToBottom();
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
    } else if (menuItems.size() != 0) {
        const int lastMenuItemIdx = static_cast<int>(menuItems.size()) - 1;
        const int itemsToShowWindowStart = std::min(viewItemIdx + 1, lastMenuItemIdx);
        const int itemsToShowWindowEnd = std::min(viewItemIdx + numberOfItemsToFit, lastMenuItemIdx);
        ++viewItemIdx;

        if (viewItemIdx > lastMenuItemIdx) {
            rolloverMenuToBottom();
            return;
        }

        for (int i = itemsToShowWindowStart; i <= itemsToShowWindowEnd; ++i) {
            if (menuItems[i]->isNotTextRender()) {
                selectedItemIdx = i;
                viewItemIdx = i;
                return;
            }
        }

        selectedItemIdx = itemsToShowWindowEnd;
        viewItemIdx = itemsToShowWindowEnd;
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
    } else if (menuItems.size() != 0) {
        const int itemsToShowWindowStart = std::max(viewItemIdx - numberOfItemsToFit, 0);
        const int itemsToShowWindowEnd = std::max(viewItemIdx - 1, 0);
        --viewItemIdx;

        if (viewItemIdx < 0) {
            rolloverMenuToTop();
            return;
        }

        for (int i = itemsToShowWindowEnd; i >= itemsToShowWindowStart; --i) {
            if (menuItems[i]->isNotTextRender()) {
                selectedItemIdx = i;
                viewItemIdx = i;
                return;
            }
        }

        selectedItemIdx = itemsToShowWindowStart;
        viewItemIdx = itemsToShowWindowStart;
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
        if (selectedItemIdx != -1) {
            for (int var2 = selectedItemIdx; var2 < static_cast<int>(menuItems.size()); ++var2) {
                IGameMenuElement* var3;

                if ((var3 = menuItems[var2]) != nullptr && var3->isNotTextRender()) {
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
        int itemsToShowWindowStart = 0;
        int itemsToShowWindowEnd = 0;
        calculateMenuWindowBoundaries(&itemsToShowWindowStart, &itemsToShowWindowEnd);

        Log::write(
            Log::LogLevel::Debug,
            "%s, selectedItemIdx: %d, viewItemIdx: %d, window start: %d, window end: %d, number of items to fit: %d, menu items count: %d\n",
            name.c_str(),
            selectedItemIdx,
            viewItemIdx,
            itemsToShowWindowStart,
            itemsToShowWindowEnd,
            numberOfItemsToFit,
            menuItems.size());

        graphics->setColor(0, 0, 0);
        var2 = menuOffsetY;

        if (!name.empty()) {
            graphics->setFont(font);
            graphics->drawString(
                name,
                xPos,
                var2,
                Graphics::TOP | Graphics::LEFT);
            var2 += font->getBaselinePosition() + 2;
        }

        // scrolling text: draw top range cursor
        if (itemsToShowWindowStart > 0) {
            micro->gameCanvas->drawSprite(graphics, 2, xPos - 3, var2);
        }

        graphics->setFont(font2);

        for (i = itemsToShowWindowStart; i <= itemsToShowWindowEnd; ++i) {
            IGameMenuElement* var4 = menuItems[i];
            graphics->setColor(0, 0, 0);
            var4->render(graphics, var2, menuOffsetX);

            // draw helmet icon near selected item
            if (i == selectedItemIdx && var4->isNotTextRender()) {
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

        // scrolling text: draw bottom range cursor
        if (static_cast<int>(menuItems.size()) > itemsToShowWindowEnd && itemsToShowWindowEnd != static_cast<int>(menuItems.size()) - 1) {
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
    menuItems.clear();
    viewItemIdx = -1;
    selectedItemIdx = -1;
}

char* GameMenu::getStrArr() const
{
    return strArr;
}

void GameMenu::startAtPosition(const int pos)
{
    rolloverMenuToBottom();
    viewItemIdx = pos;
    selectedItemIdx = pos;
}

int GameMenu::getNumberOfItemToFitOnScreen() const
{
    int screenHeightForMenuItems = canvasHeight - (menuOffsetY << 1);

    if (!name.empty()) {
        screenHeightForMenuItems -= font->getBaselinePosition() + 10;
    }

    const int menuItemHeight = font2->getBaselinePosition() + menuSpacing;
    return std::floor(screenHeightForMenuItems / menuItemHeight) - 1;
}

void GameMenu::calculateMenuWindowBoundaries(int* itemsToShowWindowStart, int* itemsToShowWindowEnd) {
    if (menuItems.empty()) {
        return;
    }

    if (static_cast<int>(menuItems.size()) <= numberOfItemsToFit) {
        *itemsToShowWindowStart = 0;
        *itemsToShowWindowEnd = menuItems.size() - 1;
    } else {
        *itemsToShowWindowStart = viewItemIdx;
        *itemsToShowWindowEnd = *itemsToShowWindowStart + numberOfItemsToFit - 1;

        if (*itemsToShowWindowEnd > static_cast<int>((menuItems.size() - 1))) {
            *itemsToShowWindowEnd = menuItems.size() - 1;
        }

        if (*itemsToShowWindowEnd - *itemsToShowWindowStart < numberOfItemsToFit) {
            *itemsToShowWindowStart = *itemsToShowWindowEnd - numberOfItemsToFit + 1;
        }
    }

    *itemsToShowWindowStart = std::max(*itemsToShowWindowStart, 0);
    *itemsToShowWindowEnd = std::min(*itemsToShowWindowEnd, static_cast<int>(menuItems.size() - 1));

}
