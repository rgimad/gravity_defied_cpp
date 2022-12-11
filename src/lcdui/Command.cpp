#include "Command.h"

Command::Command(std::string name, int type, int priority) : name(name), type(type), priority(priority) {
}

Command::~Command() {

}

bool operator==(const Command& l, const Command& r) {
    return l.name == r.name && l.type == r.type && l.priority == r.priority;
}