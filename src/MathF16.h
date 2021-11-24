#pragma once
#include <cstdint>
#include <cmath>

class MathF16 {
private:
    static int multiplyF16(int a, int b);
    static int divideF16(int a, int b);
    static int atanF16(int angle);

public:
    static const int PiHalfF16 = 102944;
    static const int PiF16 = 205887;

    static int sinF16(int angle);
    static int cosF16(int angle);
    static int atan2F16(int dx, int dy);
};