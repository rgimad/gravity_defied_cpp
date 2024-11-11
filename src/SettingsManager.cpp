#include "SettingsManager.h"

void SettingsManager::initSettings()
{
    if (!std::filesystem::exists(GlobalSetting::SavesPath)) {
        Log::write(Log::LogLevel::Info, "Creating directory %s\n", GlobalSetting::SavesPath.c_str());
        std::filesystem::create_directory(GlobalSetting::SavesPath);
    }

    const std::filesystem::path saveFileName = GlobalSetting::SavesPath / GlobalSetting::SavesPrefix / GlobalSetting::GlobalSaveFileName;

    if (!std::filesystem::exists(saveFileName)) {
        Log::write(Log::LogLevel::Info, "Creating global settings file %s\n", saveFileName.c_str());
        SettingsManager::Settings settings = {
            .perspective = 0,
            .shadows = 0,
            .driverSprite = 0,
            .bikeSprite = 0,
            .lookAhead = 0,
            .availableLeagues = 0,
            .availableLevels = 0,
            .availableTracks = { 0, 0, -1 },
            .selectedLevel = 0,
            .selectedTrack = 0,
            .selectedLeague = 0,
            .unknown2 = -127,
            .input = 0,
            .unknown3 = -127,
            .playerName = {}
        };
        strncpy(settings.playerName, defaultName, PLAYER_NAME_MAX);
        saveSettings(settings);
    }
}

SettingsManager::Settings SettingsManager::loadSettings()
{
    const std::filesystem::path saveFileName = GlobalSetting::SavesPath / GlobalSetting::SavesPrefix / GlobalSetting::GlobalSaveFileName;
    Log::write(Log::LogLevel::Info, "loadSettings %s\n", saveFileName.c_str());

    FileStream levelFileStream(saveFileName, std::ios::in | std::ios::binary);

    SettingsManager::SettingsConverter settingsConverter;
    levelFileStream.readVariable(&settingsConverter.bytes, false, sizeof(SettingsManager::Settings));

    return settingsConverter.settings;
}

void SettingsManager::saveSettings(SettingsManager::Settings settings)
{
    const std::filesystem::path saveFileName = GlobalSetting::SavesPath / GlobalSetting::SavesPrefix / GlobalSetting::GlobalSaveFileName;
    Log::write(Log::LogLevel::Info, "saveSettings %s\n", saveFileName.c_str());

    SettingsManager::SettingsConverter settingsConverter = { .settings = settings };

    FileStream levelFileStream(saveFileName, std::ios::out | std::ios::binary);
    levelFileStream.writeVariable(&settingsConverter.bytes, sizeof(SettingsManager::Settings));
}