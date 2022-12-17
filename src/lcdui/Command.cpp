#include "Command.h"

Command::Command(std::string name, int type, int priority) : name(name), type(type), priority(priority) {
}

Command::~Command() {

}