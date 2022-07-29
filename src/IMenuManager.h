#pragma once

#include "GameMenu.h"

class IMenuManager {
    virtual GameMenu getGameMenu() = 0;
    virtual void method_1(GameMenu var1, boolean var2) = 0;
    virtual void saveSmthToRecordStoreAndCloseIt() = 0;
    virtual void processMenu(IGameMenuElement var1) = 0;
};
