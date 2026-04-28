#pragma once

#include <string>
#include <vector>
#include <memory>

#include "IGameMenuElement.h"

class Micro;
class Font;
class Graphics;

class GameMenu {
private:
    GameMenu* gameMenu;
    std::string menuTitle;
    int selectedItemIndex;
    std::vector<IGameMenuElement*> vector;
    Micro* micro;
    std::shared_ptr<Font> font;
    std::shared_ptr<Font> font2;
    std::shared_ptr<Font> font3;
    // Margin/padding value
    int marginPadding;
    // Spacing between menu items
    int itemSpacing;
    // X offset for rendering
    int renderXOffset;
    // First visible item index
    int scrollOffsetFirst;
    // Last visible item index
    int scrollOffsetLast;
    // Maximum number of visible items
    int maxVisibleItems;
    int canvasWidth;
    int canvasHeight;
    // Animation frame counter for helmet sprite
    int helmetAnimFrame;
    // Is in name input mode
    bool isInputMode;
    int nameCursorPos;
    char* strArr;

public:
    int xPos;

    GameMenu(std::string title, Micro* micro, GameMenu* parentMenu, char* inputString = nullptr);
    void setItemSpacing(int spacing);
    void setMenuTitle(std::string title);
    void resetToFirstItem();
    void resetToLastItem();
    void addMenuElement(IGameMenuElement* element);
    void processGameActionDown();
    void processGameActionUp();
    void processGameActionUpd(int action);
    void render(Graphics* graphics);
    void setGameMenu(GameMenu* gameMenu);
    GameMenu* getGameMenu();
    int getSelectedItemIndex();
    void clearVector();
    std::string makeString();
    char* getStrArr() const;
    void navigateToItem(int targetIndex);
};
