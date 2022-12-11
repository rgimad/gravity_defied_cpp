#pragma once

#include <string>

class Command {
public:
    enum Type {
        SCREEN = 1,
        BACK = 2,
        CANCEL = 3,
        OK = 4,
        HELP = 5,
        STOP = 6,
        EXIT = 7,
        ITEM = 8
    };

    Command(std::string name, int type, int priority);
    ~Command();

    friend bool operator==(const Command& l, const Command& r);

    const std::string name;
    const int type;
    const int priority;
};