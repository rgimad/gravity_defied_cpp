#pragma once

#include <iostream>
#include <cstdint>
#include <filesystem>
#include <string>

enum {
    LEAGUES_MAX = 4,
    RECORD_NO_MAX = 3,
    PLAYER_NAME_MAX = 3,
};

const char cheatCode[] = "RKE";

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