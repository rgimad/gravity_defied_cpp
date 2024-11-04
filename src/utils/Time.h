#pragma once

#include <cstdint>
#include <cstring>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

namespace Time {
int64_t currentTimeMillis();
std::string timeToString(const int64_t time);
void sleep(int64_t ms);
};