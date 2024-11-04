#include "Time.h"

#include <SDL2/SDL.h>
#include <chrono>

namespace Time {
int64_t currentTimeMillis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string timeToString(const int64_t time)
{
    const int minutes = (int)(time / 100L / 60L);
    const int seconds = (int)(time / 100L % 60L);
    const int subseconds = (int)(time % 100L);

    std::stringstream ss;
    ss << std::setfill('0')
       << std::setw(2) << minutes
       << ':'
       << std::setw(2) << seconds
       << '.'
       << std::setw(2) << subseconds;
    return ss.str();
}

void sleep(int64_t ms)
{
    SDL_Delay(ms);
}
}