#include "time.h"

#include <SDL2/SDL.h>
#include <chrono>

namespace Time {
int64_t currentTimeMillis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
}

void sleep(int64_t ms)
{
    SDL_Delay(ms);
}
}