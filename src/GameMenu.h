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
    int field_95;
    std::vector<IGameMenuElement*> vector;
    Micro* micro;
    std::shared_ptr<Font> font;
    std::shared_ptr<Font> font2;
    std::shared_ptr<Font> font3;
    int menuOffsetY;
    int menuSpacing;
    int menuOffsetX;
    int field_105;
    int field_106;
    int field_107;
    int canvasWidth;
    int canvasHeight;
    int selectedMenuItemTickSpriteNo;
    bool isTextInput;
    int nameCursorPos;
    char* strArr;

public:
    int xPos;

    GameMenu(std::string var1, Micro* micro, GameMenu* var3, char* inputString = nullptr);
    void method_70();
    void method_71();
    void addMenuElement(IGameMenuElement* var1);
    void processGameActionDown();
    void processGameActionUp();
    void processGameActionUpd(int var1);
    void render_76(Graphics* graphics);
    void setGameMenu(GameMenu* gameMenu);
    GameMenu* getGameMenu();
    void clearVector();
    std::string makeString();
    char* getStrArr() const;
    void method_83(int var1);
};
