#pragma once

#include <string>

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class Command {
public:
    enum class Type {
        SCREEN = 1,
        BACK = 2,
        CANCEL = 3,
        OK = 4,
        HELP = 5,
        STOP = 6,
        EXIT = 7,
        ITEM = 8
    };

    struct EqualFunction {
        bool operator()(Command* lhs, Command* rhs) const
        {
            return lhs->name == rhs->name && lhs->type == rhs->type && lhs->priority == rhs->priority;
        }
    };

    struct HashFunction {
        size_t operator()(Command* command) const
        {
            std::size_t seed = 0;
            hash_combine(seed, command->name);
            hash_combine(seed, command->type);
            hash_combine(seed, command->priority);
            return seed;
        }
    };

    Command(const std::string name, const Command::Type type, const int priority);

    const std::string name;
    const Command::Type type;
    const int priority;
};