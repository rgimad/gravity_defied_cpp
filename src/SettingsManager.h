#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <array>

#include "config.h"
#include "utils/FileStream.h"

class SettingsManager {
public:
    struct Settings {
        int8_t perspective; // 0
        int8_t shadows; // 1
        int8_t driverSprite; // 2
        int8_t bikeSprite; // 3
        int8_t lookAhead; // 4
        int8_t availableLeagues; // 5
        int8_t availableLevels; // 6 
        int8_t availableTracks[3]; // 7-9
        // int8_t availableEasyTracks; // 7
        // int8_t availableMediumTracks; // 8
        // int8_t availableHardTracks; // 9
        int8_t selectedLevel; // 10
        int8_t selectedTrack; // 11
        int8_t selectedLeague; // 12
        int8_t unknown2; // 13
        int8_t input; // 14
        int8_t unknown3; //15
        char playerName[3]; //16-19
    };
    static_assert(sizeof(Settings) == 19);

    union SettingsConverter {
        Settings settings;
        int8_t bytes[sizeof(Settings)];
    };

    static SettingsManager::Settings loadSettings();
    static void saveSettings(SettingsManager::Settings settings);
private:
    
};