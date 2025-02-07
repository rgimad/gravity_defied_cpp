#pragma once

#include <cstdint>
#include <cstring>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

namespace Time {
int64_t currentTimeMillis();
std::string timeToString(const uint64_t time);
void sleep(const uint64_t ms);
};