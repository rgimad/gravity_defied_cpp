#include "time.h"

#include <chrono>
#include <thread>

namespace Time {
    int64_t currentTimeMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void sleep(int64_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}