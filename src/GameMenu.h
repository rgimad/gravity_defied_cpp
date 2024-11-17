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
    std::string name;
    int selectedItemIdx;
    int viewItemIdx;

    std::vector<IGameMenuElement*> menuItems;
    Micro* micro;
    std::shared_ptr<Font> font;
    std::shared_ptr<Font> font2;
    std::shared_ptr<Font> font3;
    int menuOffsetY;
    int menuSpacing;
    int menuOffsetX;
    // int itemsToShowWindowStart;
    // int itemsToShowWindowEnd;
    int numberOfItemsToFit;
    int canvasWidth;
    int canvasHeight;
    int selectedMenuItemTickSpriteNo;
    bool isTextInput;
    int nameCursorPos;
    char* strArr;

    int getNumberOfItemToFitOnScreen() const;
    void calculateMenuWindowBoundaries(int* itemsToShowWindowStart, int* itemsToShowWindowEnd);

public:
    int xPos;

    GameMenu(std::string var1, Micro* micro, GameMenu* var3, char* inputString = nullptr);
    void rolloverMenuToBottom();
    void rolloverMenuToTop();
    void addMenuElement(IGameMenuElement* var1);
    void processGameActionDown();
    void processGameActionUp();
    void processGameActionUpd(int var1);
    void render_76(Graphics* graphics);
    void setGameMenu(GameMenu* gameMenu);
    GameMenu* getGameMenu();
    void clearVector();
    // std::string makeString();
    char* getStrArr() const;
    void startAtPosition(const int pos);
};
