#pragma once

class CommandListener;
class Command;

class Displayable {
public:
    virtual bool isShown() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    // virtual void setCommandListener(CommandListener *listener) = 0; // TODO
    // virtual void addCommand(Command *command) = 0; // TODO
    // virtual void removeCommand(Command *command) = 0; // TODO
};
