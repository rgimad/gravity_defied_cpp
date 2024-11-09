#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <stdexcept>

#include "CanvasImpl.h"
#include "Graphics.h"
#include "Command.h"
#include "CommandListener.h"
#include "Displayable.h"
#include "Command.h"
#include "../Micro.h"
#include "../config.h"

class CanvasImpl;

class Canvas : public Displayable {
private:
    std::unique_ptr<CanvasImpl> impl;
    std::unique_ptr<Graphics> graphics; // IMPROVE this is probably not the best place for graphics
    CommandListener* commandListener;

    std::unordered_set<Command*, Command::HashFunction, Command::EqualFunction> currentCommands;

public:
    Canvas();
    ~Canvas();
    bool isShown() override;
    int getWidth() override;
    int getHeight() override;
    void setWindowTitle(const std::string& title);
    CanvasImpl* getCanvasImpl();
    void repaint();
    void serviceRepaints();
    void removeCommand(Command* command);
    void addCommand(Command* command);
    void setCommandListener(CommandListener* listener);
    void publicKeyPressed(const Keys keyCode);
    void publicKeyReleased(const Keys keyCode);
    void pressedEsc();
    virtual void paint(Graphics* g) = 0;
    virtual void keyPressed(const Keys keyCode) = 0;
    virtual void keyReleased(const Keys keyCode) = 0;
};