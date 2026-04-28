#pragma once

#include "GameMenu.h"

class IMenuManager {
public:
    virtual GameMenu* getCurrentMenu() = 0;
    virtual void switchToMenu(GameMenu* menu, bool skipSelectionReset) = 0;
    virtual void saveStateAndCloseRecordStore() = 0;
    virtual void handleMenuSelection(IGameMenuElement* element) = 0;
};
