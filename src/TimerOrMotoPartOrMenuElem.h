#pragma once

#include <string>

#include "Micro.h"
#include "IGameMenuElement.h"

class Graphics;
class GameMenu;
class IMenuManager;

class TimerOrMotoPartOrMenuElem : public IGameMenuElement { // TODO  extends TimerTask implements IGameMenuElement
private:
    std::string text;
    GameMenu *gameMenu;
    IMenuManager *menuManager;

public:
    int xF16;
    int yF16;
    int angleF16;
    int field_382;
    int field_383;
    int field_384;
    int field_385;
    int field_386;
    int field_387;
    int timerNo;
    Micro *micro;

    TimerOrMotoPartOrMenuElem();
    TimerOrMotoPartOrMenuElem(int timerNo, Micro *micro) ;
    TimerOrMotoPartOrMenuElem(std::string text, GameMenu *gameMenu, IMenuManager *menuManager);
    void setToZeros();
    void run();
    void setText(std::string text);
    std::string getText();
    bool isNotTextRender();
    void menuElemMethod(int var1);
    void setGameMenu(GameMenu *gameMenu);
    void render(Graphics *graphics, int y, int x);
};