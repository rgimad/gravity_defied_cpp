#include "Command.h"

Command::Command(std::string name, int type, int priority) : name(name), type(type), priority(priority) {
}

/*

TODO

@Override
public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;
    Command command = (Command) o;
    return type == command.type &&
            priority == command.priority &&
            Objects.equals(name, command.name);
}

@Override
public int hashCode() {
    return Objects.hash(name, type, priority);
}

@Override
public String toString() {
    return "Command(\"" + name + "\", " + type + ", " + priority + ")";
}
*/