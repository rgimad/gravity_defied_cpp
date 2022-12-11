#include "Timer.h"

#include "Helpers.h"

Timer::Timer(int id, int64_t timeoutMs) {
    this->id = id;
    this->timeoutMs = timeoutMs;

    this->startTimeMs = Helpers::currentTimeMillis();
}

bool Timer::ready() {
    return Helpers::currentTimeMillis() - startTimeMs > timeoutMs;
}

int Timer::getId() {
    return id;
}