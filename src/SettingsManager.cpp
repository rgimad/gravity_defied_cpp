#include "SettingsManager.h"

SettingsManager::Settings SettingsManager::loadSettings()
{
    std::string saveFileName = "./saves/" + GlobalSetting::SavesPrefix + "_" + GlobalSetting::GlobalSaveFileName;
    std::cout << "loadSettings " << saveFileName << std::endl;

    FileStream levelFileStream(saveFileName, std::ios::in | std::ios::binary);

    SettingsManager::SettingsConverter settingsConverter;
    levelFileStream.readVariable(&settingsConverter.bytes, false, sizeof(SettingsManager::Settings));

    return settingsConverter.settings;
}

void SettingsManager::saveSettings(SettingsManager::Settings settings)
{
    std::string saveFileName = "./saves/" + GlobalSetting::SavesPrefix + "_" + GlobalSetting::GlobalSaveFileName;
    std::cout << "saveSettings " << saveFileName << std::endl;

    SettingsManager::SettingsConverter settingsConverter = { .settings = settings };

    FileStream levelFileStream(saveFileName, std::ios::out | std::ios::binary);
    levelFileStream.writeVariable(&settingsConverter.bytes, sizeof(SettingsManager::Settings));
}