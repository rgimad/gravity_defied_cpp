#include "MathF16.h"

int MathF16::multiplyF16(int a, int b)
{
    return (int)((int64_t)a * (int64_t)b >> 16);
}

int MathF16::divideF16(int a, int b)
{
    return (int)(((int64_t)a << 32) / (int64_t)b >> 16);
}

int MathF16::atanF16(int angle)
{
    return (int)roundf(atan(angle / (float)0xFFFF) * 65536);
}

int MathF16::sinF16(int angle)
{
    return (int)roundf(sin(angle / (float)0xFFFF) * 65536);
}

int MathF16::cosF16(int angle)
{
    return sinF16(PiHalfF16 - angle);
}

int MathF16::atan2F16(int dx, int dy)
{
    if ((dy < 0 ? -dy : dy) < 3) {
        return (dx > 0 ? 1 : -1) * PiHalfF16;
    } else {
        int atanVal = atanF16(divideF16(dx, dy));
        if (dx > 0) {
            return dy > 0 ? atanVal : PiF16 + atanVal;
        } else {
            return dy > 0 ? atanVal : atanVal - PiF16;
        }
    }
}
