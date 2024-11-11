#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>
#include <filesystem>
#include <string>
#include <map>
#include <array>
#include <vector>


constexpr int LEAGUES_MAX = 4;
constexpr int RECORD_NO_MAX = 3;
constexpr int PLAYER_NAME_MAX = 3;


enum class Keys {
    NONE, // = 0,
    UP, // = 1,
    DOWN, // = 6,
    LEFT, // = 2,
    RIGHT, // = 5,
    FIRE, // = 8,
    BACK, // = 9,
};

static std::map<SDL_Keycode, Keys> gameKeyMappings = {
    { SDLK_LEFT, Keys::LEFT },
    { SDLK_RIGHT, Keys::RIGHT },
    { SDLK_UP, Keys::UP },
    { SDLK_DOWN, Keys::DOWN },

    { SDLK_RETURN, Keys::UP },
    { SDLK_ESCAPE, Keys::BACK },
    { SDLK_BACKSPACE, Keys::DOWN },
};
static std::map<SDL_Keycode, Keys> menuKeyMappings = {
    { SDLK_LEFT, Keys::LEFT },
    { SDLK_RIGHT, Keys::RIGHT },
    { SDLK_UP, Keys::UP },
    { SDLK_DOWN, Keys::DOWN },

    { SDLK_RETURN, Keys::FIRE },
    { SDLK_ESCAPE, Keys::BACK },
    { SDLK_BACKSPACE, Keys::BACK },
};

static std::map<Keys, std::array<int, 2>> availableActions = {
    { Keys::UP, { 1, 0 } },
    { Keys::DOWN, { -1, 0 } },
    { Keys::LEFT, { 0, -1 } },
    { Keys::RIGHT, { 0, 1 } },
    // { Keys::FIRE, {1, 0} },
    // { Keys::BACK, {-1, 0} },
};
static std::vector<std::string> onOffLabels = { "On", "Off" };

static std::vector<std::string> inputLabels = { "Keyset 1", "Keyset 2", "Keyset 3" };
static std::vector<std::string> levelLabels = { "Easy", "Medium", "Pro" };

const char cheatCode[] = "RKE";
const char defaultName[] = "AAA";

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
    static std::filesystem::path GlobalSaveFileName;
    static std::filesystem::path SavesPath;
    static std::filesystem::path MrgFilePath;
};