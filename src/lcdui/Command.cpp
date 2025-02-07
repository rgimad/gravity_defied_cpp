#include "Command.h"

Command::Command(const std::string name, const Command::Type type, const int priority)
    : name(name)
    , type(type)
    , priority(priority)
{
}