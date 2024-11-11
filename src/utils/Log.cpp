#include "Log.h"

Log::LogLevel Log::logLevel = Log::LogLevel::Error;

void Log::write(const Log::LogLevel level, const char* szFormat, ...)
{
    if (level < Log::logLevel) {
        return;
    }

    va_list args;
    va_start(args, szFormat);
    vprintf(szFormat, args);
    va_end(args);
}



