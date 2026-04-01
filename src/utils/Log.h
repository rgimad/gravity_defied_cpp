#pragma once

#include <cstdarg>
#include <cstdlib>
#include <cstdio>

struct Log {
    enum class LogLevel {
        All = 0,
        Debug = 1,
        Info = 2,
        Warning = 3,
        Error = 4,
        Fatal = 5,
        None = 6
    };

    static Log::LogLevel logLevel;
    static void write(const Log::LogLevel level, const char* szFormat, ...);
};