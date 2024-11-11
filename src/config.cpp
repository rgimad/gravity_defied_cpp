#include "config.h"

bool GlobalSetting::WindowFullscreen = false;
uint8_t GlobalSetting::LogoMultiplier = 3;
uint8_t GlobalSetting::SplashMultiplier = 3;
uint8_t GlobalSetting::TimerFpsTextOffset = 14;
uint8_t GlobalSetting::LoadingBarPadding = 2;
uint8_t GlobalSetting::BarScreenOffset = 2;
uint8_t GlobalSetting::BarH = 10;
uint16_t GlobalSetting::DefaultScreenWidth = 640;
uint16_t GlobalSetting::DefaultScreenHeight = 480;
std::string GlobalSetting::SavesPrefix = "";
std::filesystem::path GlobalSetting::SavesPath = "./saves";
std::filesystem::path GlobalSetting::GlobalSaveFileName = "GDTRStates.dat";
std::filesystem::path GlobalSetting::MrgFilePath = "levels.mrg";