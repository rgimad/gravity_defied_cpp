
#include "SettingsStringRender.h"

#include "GameMenu.h"
#include "GameCanvas.h"
#include "lcdui/Graphics.h"

SettingsStringRender::SettingsStringRender(std::string text, int isDisabled, IMenuManager* menuManager, std::vector<std::string> optionsList, bool var5, Micro* micro, GameMenu* gameMenu, bool useColon)
{
    this->micro = micro;
    if (useColon) {
        this->useColon = true;
        this->text = text;
        this->menuManager = menuManager;
        isDrawSprite8 = true;
        hasSprite = false;
    } else {
        this->useColon = false;
        this->text = text + ":";
        currentOptionPos = isDisabled;
        this->menuManager = menuManager;
        this->optionsList = optionsList;
        if (this->optionsList.empty()) {
            this->optionsList = { "" };
        }

        maxAvailableOption = optionsList.size() - 1;
        field_146 = var5;
        setCurentOptionPos(isDisabled);
        if (var5) {
            if (isDisabled == 1) {
                selectedOptionName = "Off";
            } else {
                selectedOptionName = "On";
            }
        } else {
            parentGameMenu = gameMenu;
            selectCurrentOptionName();
            init();
        }
    }
}

void SettingsStringRender::setFlags(bool hasSprite, bool isDrawSprite8)
{
    this->hasSprite = hasSprite;
    this->isDrawSprite8 = isDrawSprite8;
}

void SettingsStringRender::setOptionsList(std::vector<std::string> var1)
{
    optionsList = var1;
    if (currentOptionPos > static_cast<int>(optionsList.size()) - 1) {
        currentOptionPos = optionsList.size() - 1;
    }

    if (maxAvailableOption > static_cast<int>(optionsList.size()) - 1) {
        maxAvailableOption = optionsList.size() - 1;
    }

    selectCurrentOptionName();
    init();
}

void SettingsStringRender::init()
{
    currentGameMenu = new GameMenu(text, micro, parentGameMenu);
    settingsStringRenders = std::vector<SettingsStringRender*>(optionsList.size());

    for (int var1 = 0; var1 < static_cast<int>(settingsStringRenders.size()); ++var1) {
        if (var1 > maxAvailableOption) {
            settingsStringRenders[var1] = new SettingsStringRender(optionsList[var1], 0, this, std::vector<std::string>(), false, micro, parentGameMenu, true);
            settingsStringRenders[var1]->setFlags(true, true);
        } else {
            settingsStringRenders[var1] = new SettingsStringRender(optionsList[var1], 0, this, std::vector<std::string>(), false, micro, parentGameMenu, true);
        }

        currentGameMenu->addMenuElement(settingsStringRenders[var1]);
    }
}

void SettingsStringRender::setParentGameMenu(GameMenu* parentGameMenu)
{
    this->parentGameMenu = parentGameMenu;
}

void SettingsStringRender::setText(std::string text)
{
    if (useColon) {
        this->text = text;
    } else {
        this->text = text + ":";
    }
}

bool SettingsStringRender::isNotTextRender()
{
    return true;
}

void SettingsStringRender::menuElemMethod(int var1)
{
    if (useColon) {
        if (var1 == 1) {
            menuManager->processMenu(this);
            return;
        }
    } else {
        switch (var1) {
        case 1:
            if (field_146) {
                ++currentOptionPos;
                if (currentOptionPos > 1) {
                    currentOptionPos = 0;
                }

                if (currentOptionPos == 1) {
                    selectedOptionName = "Off";
                } else {
                    selectedOptionName = "On";
                }

                menuManager->processMenu(this);
                return;
            }

            field_147 = true;
            menuManager->processMenu(this);
            return;
        case 2:
            if (field_146) {
                if (currentOptionPos == 1) {
                    currentOptionPos = 0;
                    selectedOptionName = "On";
                    menuManager->processMenu(this);
                }

                return;
            }

            ++currentOptionPos;
            if (currentOptionPos > static_cast<int>(optionsList.size()) - 1) {
                currentOptionPos = optionsList.size() - 1;
            } else {
                menuManager->processMenu(this);
            }

            selectCurrentOptionName();
            return;
        case 3:
            if (field_146) {
                if (currentOptionPos == 0) {
                    currentOptionPos = 1;
                    selectedOptionName = "Off";
                    menuManager->processMenu(this);
                }

                return;
            }

            --currentOptionPos;
            if (currentOptionPos < 0) {
                currentOptionPos = 0;
            } else {
                selectCurrentOptionName();
                menuManager->processMenu(this);
            }

            selectCurrentOptionName();
        }
    }
}

void SettingsStringRender::selectCurrentOptionName()
{
    selectedOptionName = optionsList[currentOptionPos];
}

void SettingsStringRender::render(Graphics* graphics, int y, int x)
{
    if (useColon) {
        if (!hasSprite) {
            graphics->drawString(text, x, y, 20);
        } else {
            graphics->drawString(text, x + GameCanvas::spriteSizeX[8] + 3, y, 20);
            if (isDrawSprite8) {
                micro->gameCanvas->drawSprite(graphics, 8, x, y - GameCanvas::spriteSizeY[8] / 2 + graphics->getFont()->getHeight() / 2);
            } else {
                micro->gameCanvas->drawSprite(graphics, 9, x, y - GameCanvas::spriteSizeY[9] / 2 + graphics->getFont()->getHeight() / 2);
            }
        }
    } else {
        graphics->drawString(text, x, y, 20);
        int shiftedX = x + graphics->getFont()->stringWidth(text);
        if (currentOptionPos > maxAvailableOption && !field_146) {
            micro->gameCanvas->drawSprite(graphics, 8, shiftedX + 1, y - GameCanvas::spriteSizeY[8] / 2 + graphics->getFont()->getHeight() / 2);
            shiftedX += GameCanvas::spriteSizeX[9] + 1;
        }

        shiftedX += 2;
        graphics->drawString(selectedOptionName, shiftedX, y, 20);
    }
}

void SettingsStringRender::setAvailableOptions(int maxAvailableOption)
{
    this->maxAvailableOption = maxAvailableOption;
    if (maxAvailableOption > static_cast<int>(optionsList.size()) - 1) {
        maxAvailableOption = optionsList.size() - 1;
    }

    if (currentGameMenu != nullptr) {
        for (int i = 0; i < static_cast<int>(settingsStringRenders.size()); ++i) {
            if (i > maxAvailableOption) {
                settingsStringRenders[i]->setFlags(true, true);
            } else {
                settingsStringRenders[i]->setFlags(false, false);
            }
        }
    }
}

int SettingsStringRender::getMaxAvailableOptionPos()
{
    return maxAvailableOption;
}

int SettingsStringRender::getMaxOptionPos()
{
    return optionsList.size() - 1;
}

std::vector<std::string> SettingsStringRender::getOptionsList()
{
    return optionsList;
}

void SettingsStringRender::setCurentOptionPos(int pos)
{
    currentOptionPos = pos;
    if (currentOptionPos > static_cast<int>(optionsList.size()) - 1) {
        currentOptionPos = 0;
    }

    if (currentOptionPos < 0) {
        currentOptionPos = optionsList.size() - 1;
    }

    selectCurrentOptionName();
}

int SettingsStringRender::getCurrentOptionPos()
{
    return currentOptionPos;
}

GameMenu* SettingsStringRender::getGameMenu()
{
    return currentGameMenu;
}

void SettingsStringRender::method_1(GameMenu* var1, bool var2)
{
    (void)var1;
    (void)var2;
}

void SettingsStringRender::saveSmthToRecordStoreAndCloseIt()
{
}

void SettingsStringRender::processMenu(IGameMenuElement* var1)
{
    for (int var2 = 0; var2 < static_cast<int>(settingsStringRenders.size()); ++var2) {
        if (var1 == settingsStringRenders[var2]) {
            currentOptionPos = var2;
            selectCurrentOptionName();
            break;
        }
    }

    menuManager->method_1(parentGameMenu, true);
    menuManager->processMenu(this);
}

std::vector<SettingsStringRender*> SettingsStringRender::getSettingsStringRenders()
{
    return settingsStringRenders;
}

bool SettingsStringRender::method_114()
{
    if (field_147) {
        field_147 = false;
        return true;
    } else {
        return field_147;
    }
}
