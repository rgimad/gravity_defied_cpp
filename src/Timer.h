#pragma once

#include <stdint.h>

class Timer {
private:
    int id;
    int64_t startTimeMs;
    int64_t timeoutMs;

public:
    Timer(int id, int64_t timeoutMs);

    bool ready();
    int getId();
};