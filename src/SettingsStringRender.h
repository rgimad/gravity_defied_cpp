#pragma once

#include <vector>
#include <string>

#include "IMenuManager.h"
#include "IGameMenuElement.h"

class GameMenu;
class Micro;
class Graphics;

class SettingsStringRender : public IMenuManager, public IGameMenuElement {
private:
    std::vector<std::string> optionsList;
    int currentOptionPos;
    int maxAvailableOption;
    std::string text;
    IMenuManager* menuManager;
    GameMenu* currentGameMenu = nullptr;
    GameMenu* parentGameMenu = nullptr;
    // Is On/Off toggle mode (vs multi-option)
    bool isToggleMode;
    // Selection confirmed flag
    bool isSelectionConfirmed = false;
    std::string selectedOptionName;
    Micro* micro = nullptr;
    std::vector<SettingsStringRender*> settingsStringRenders;
    bool hasSprite;
    bool isDrawSprite8;
    bool useColon;

    void selectCurrentOptionName();

public:
    SettingsStringRender(std::string text, int isDisabled, IMenuManager* menuManager, std::vector<std::string> optionsList, bool isToggle, Micro* micro, GameMenu* gameMenu, bool useColon);
    void setFlags(bool hasSprite, bool isDrawSprite8);
    void setOptionsList(std::vector<std::string> options);
    void init();
    void setParentGameMenu(GameMenu* parentGameMenu);
    void setText(std::string text);
    bool isNotTextRender();
    void menuElemMethod(int action);
    void render(Graphics* graphics, int y, int x);
    void setAvailableOptions(int maxAvailableOption);
    int getMaxAvailableOptionPos();
    int getMaxOptionPos();
    std::vector<std::string> getOptionsList();
    void setCurentOptionPos(int pos);
    int getCurrentOptionPos();
    GameMenu* getCurrentMenu();
    GameMenu* getParentGameMenu();
    void switchToMenu(GameMenu* menu, bool skipSelectionReset);
    void saveStateAndCloseRecordStore();
    void handleMenuSelection(IGameMenuElement* element);
    std::vector<SettingsStringRender*> getSettingsStringRenders();
    // Returns true if selection was confirmed, then clears flag
    bool checkAndClearSelectionFlag();
};
