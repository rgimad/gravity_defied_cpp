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
    GameMenu *gameMenu;
    std::string field_94;
    int field_95;
    std::vector<IGameMenuElement*> vector;
    Micro *micro;
    Font *font;
    Font *font2;
    Font *font3;
    int field_101;
    int field_103;
    int field_104;
    int field_105;
    int field_106;
    int field_107;
    int canvasWidth;
    int canvasHeight;
    int field_110;
    bool field_111;
    int nameCursorPos;
    std::vector<int8_t> strArr;

public:
    int xPos;

    GameMenu(std::string var1, Micro *micro, GameMenu *var3, std::vector<int8_t> var4);
    void method_68(int var1);
    void method_69(std::string var1);
    void method_70();
    void method_71();
    void addMenuElement(IGameMenuElement *var1);
    void processGameActionDown();
    void processGameActionUp();
    void processGameActionUpd(int var1);
    void render_76(Graphics *graphics);
    void setGameMenu(GameMenu *gameMenu);
    GameMenu* getGameMenu();
    int method_79();
    void clearVector();
    std::string makeString();
    std::vector<int8_t> getStrArr();
    void method_83(int var1);
};