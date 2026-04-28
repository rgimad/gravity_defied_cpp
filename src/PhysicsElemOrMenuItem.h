#pragma once

#include <string>

#include "Micro.h"
#include "IGameMenuElement.h"

class Graphics;
class GameMenu;
class IMenuManager;

// FIXME: should be split into separate classes PhysicsState / MenuItem
class PhysicsElemOrMenuItem : public IGameMenuElement {
private:
    std::string text;
    GameMenu* gameMenu;
    IMenuManager* menuManager;

public:
    int xF16;
    int yF16;
    int angleF16;
    int velocityXF16;
    int velocityYF16;
    // Angular velocity
    int angularVelocityF16;
    // Force accumulator X
    int forceAccumXF16;
    // Force accumulator Y
    int forceAccumYF16;
    // Torque
    int torqueF16;
    int timerNo; // UNUSED
    Micro* micro;

    PhysicsElemOrMenuItem();
    PhysicsElemOrMenuItem(int timerNo, Micro* micro);
    PhysicsElemOrMenuItem(std::string text, GameMenu* gameMenu, IMenuManager* menuManager);
    void setToZeros();
    void setText(std::string text);
    std::string getText();
    bool isNotTextRender();
    void menuElemMethod(int action);
    void setGameMenu(GameMenu* gameMenu);
    void render(Graphics* graphics, int y, int x);
};
