#pragma once

#include <stdint.h>

namespace Time {
int64_t currentTimeMillis();
void sleep(int64_t ms);
};