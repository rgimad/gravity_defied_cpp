#include "Timer.h"

#include "utils/Time.h"

Timer::Timer(int id, int64_t timeoutMs)
{
    this->id = id;
    this->timeoutMs = timeoutMs;

    this->startTimeMs = Time::currentTimeMillis();
}

bool Timer::ready()
{
    return Time::currentTimeMillis() - startTimeMs > timeoutMs;
}

int Timer::getId()
{
    return id;
}