#pragma once

#include <iostream>
#include <cstdint>
#include <filesystem>
#include <string>

struct GlobalSetting {
    static bool WindowFullscreen;
    static uint8_t LogoMultiplier;
    static uint8_t SplashMultiplier;
    static uint8_t TimerFpsTextOffset;
    static uint8_t LoadingBarPadding;
    static uint8_t BarScreenOffset;
    static uint8_t BarH;
    static uint16_t DefaultScreenWidth;
    static uint16_t DefaultScreenHeight;
    static std::string SavesPrefix;
    static std::string GlobalSaveFileName;
    static std::filesystem::path MrgFilePath;
};