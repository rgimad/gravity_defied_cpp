#pragma once

class Command;
class Displayable;

class CommandListener {
public:
    virtual void commandAction(Command *command, Displayable *displayable) = 0;
};
