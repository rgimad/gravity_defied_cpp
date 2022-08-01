#pragma once

#include <cstring>

#include "Micro.h"

class TimerOrMotoPartOrMenuElem { // TODO  extends TimerTask implements IGameMenuElement
private:
    char *text;
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
    TimerOrMotoPartOrMenuElem(char *text, GameMenu *gameMenu, IMenuManager *menuManager);
    void setToZeros();
    void run();
    void setText(char *text);
    char* getText();
    bool isNotTextRender();
    void menuElemMethod(int var1);
    void setGameMenu(GameMenu *gameMenu);
    void render(Graphics *graphics, int y, int x);
};