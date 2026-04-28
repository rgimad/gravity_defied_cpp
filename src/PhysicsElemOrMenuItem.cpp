#include "PhysicsElemOrMenuItem.h"

#include "IMenuManager.h"
#include "GameMenu.h"
#include "lcdui/Graphics.h"

PhysicsElemOrMenuItem::PhysicsElemOrMenuItem()
{
    setToZeros();
}

PhysicsElemOrMenuItem::PhysicsElemOrMenuItem(int timerNo, Micro* micro)
{
    this->micro = micro;
    this->timerNo = timerNo;
}

PhysicsElemOrMenuItem::PhysicsElemOrMenuItem(std::string text, GameMenu* gameMenu, IMenuManager* menuManager)
{
    this->text = text + ">";
    this->gameMenu = gameMenu;
    this->menuManager = menuManager;
}

void PhysicsElemOrMenuItem::setToZeros()
{
    xF16 = yF16 = angleF16 = 0;
    velocityXF16 = velocityYF16 = angularVelocityF16 = 0;
    forceAccumXF16 = forceAccumYF16 = torqueF16 = 0;
}

void PhysicsElemOrMenuItem::setText(std::string text)
{
    this->text = text + ">";
}

std::string PhysicsElemOrMenuItem::getText()
{
    return text;
}

bool PhysicsElemOrMenuItem::isNotTextRender()
{
    return true;
}

void PhysicsElemOrMenuItem::menuElemMethod(int action)
{
    switch (action) {
    case 1:
    case 2:
        menuManager->handleMenuSelection(this);
        gameMenu->setGameMenu(menuManager->getCurrentMenu());
        menuManager->switchToMenu(gameMenu, false);
    case 3:
    default:
        break;
    }
}

void PhysicsElemOrMenuItem::setGameMenu(GameMenu* gameMenu)
{
    this->gameMenu = gameMenu;
}

void PhysicsElemOrMenuItem::render(Graphics* graphics, int y, int x)
{
    graphics->drawString(text, x, y, 20);
}