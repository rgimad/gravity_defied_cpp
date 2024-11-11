#pragma once

#include <vector>
#include <string>
#include <memory>

#include "lcdui/FontStorage.h"
#include "TextRender.h"
#include "RecordManager.h"
#include "Micro.h"
#include "LevelLoader.h"
#include "GameMenu.h"
#include "SettingsStringRender.h"
#include "utils/Time.h"
#include "config.h"
#include "IMenuManager.h"
#include "SettingsManager.h"

class MenuManager : public IMenuManager {
private:
    SettingsManager::Settings settings;

    Micro* micro;
    RecordManager* recordManager;
    Command* commandOk;
    Command* commandBack;
    GameMenu* gameMenuMain;
    GameMenu* gameMenuPlay;
    GameMenu* gameMenuOptions;
    GameMenu* gameMenuAbout;
    GameMenu* gameMenuHelp;
    GameMenu* gameMenuConfirmClear;
    GameMenu* gameMenuConfirmReset;
    GameMenu* gameMenuFinished;
    GameMenu* gameMenuIngame;
    TimerOrMotoPartOrMenuElem* taskPlayMenu;
    TimerOrMotoPartOrMenuElem* taskOptions;
    TimerOrMotoPartOrMenuElem* taskHelp;
    SettingsStringRender* settingStringLevel;
    GameMenu* gameMenuStringLevel;
    SettingsStringRender* settingsStringTrack;
    GameMenu* field_299;
    SettingsStringRender* settingsStringLeague;
    GameMenu* gameMenuLeague;
    GameMenu* gameMenuHighscore;
    TimerOrMotoPartOrMenuElem* gameTimerTaskHighscore;
    SettingsStringRender* taskStart;
    SettingsStringRender* perspectiveSetting;
    SettingsStringRender* shadowsSetting;
    SettingsStringRender* driverSpriteSetting;
    SettingsStringRender* bikeSpriteSetting;
    // SettingsStringRender* inputSetting;
    SettingsStringRender* lookAheadSetting;
    TimerOrMotoPartOrMenuElem* clearHighscoreSetting;
    TimerOrMotoPartOrMenuElem* field_313;
    SettingsStringRender* field_314;
    SettingsStringRender* field_315;
    TimerOrMotoPartOrMenuElem* taskAbout;
    GameMenu* field_317;
    TimerOrMotoPartOrMenuElem* field_318;
    GameMenu* field_319;
    TimerOrMotoPartOrMenuElem* field_320;
    GameMenu* field_321;
    TimerOrMotoPartOrMenuElem* field_322;
    GameMenu* gameMenuOptionsHighscoreDescription;
    TimerOrMotoPartOrMenuElem* taskHighscore;
    GameMenu* gameMenuOptions2;
    TimerOrMotoPartOrMenuElem* field_326;
    GameMenu* gameMenuEnterName;
    SettingsStringRender* settingStringBack;
    SettingsStringRender* settingStringPlayMenu;
    SettingsStringRender* settingStringContinue;
    SettingsStringRender* settingStringGoToMain;
    SettingsStringRender* settingStringExitGame;
    SettingsStringRender* field_333;
    SettingsStringRender* field_334;
    SettingsStringRender* field_335;
    SettingsStringRender* field_336;
    int64_t trackTimeMs;
    std::string trackTimeFormatted;
    std::vector<int> field_345 = { 0, 0, 0 };
    std::vector<std::string> leagueNames = std::vector<std::string>(3);
    std::vector<std::string> leagueNamesAll4;
    std::unique_ptr<Image> rasterImage;
    TextRender* textRenderCodeBrewLink;
    int field_354 = 0;
    int field_355 = 0;
    bool currentLevelFinished = false;
    bool field_357 = false;
    int8_t field_372 = 0;
    std::unique_ptr<TextRender> field_376;

    // Alert alert = nullptr; // TODO

    void addTextRender(GameMenu* gameMenu, std::string text);
    void method_197();
    void fillCanvasWithImage(Graphics* graphics);
    void exit();
    int getCountOfRecordStoresWithPrefix(int prefixNumber);

public:
    GameMenu* currentGameMenu;
    int field_360 = 0;
    bool field_377 = false;

    MenuManager(Micro* var1);
    void initPart(int var1);
    int getCurrentLevel();
    int getCurrentTrack();
    bool method_196();
    void repaint();
    int getCanvasHeight();
    int getCanvasWidth();
    void method_201(int var1);
    void method_206(Command* var1, Displayable* var2);
    GameMenu* getGameMenu();
    void method_1(GameMenu* gm, bool var2);
    void method_207(int var1);
    /* synchronized */ void saveSmthToRecordStoreAndCloseIt();
    void method_208();
    void run();
    void showAlert(std::string title, std::string alertText, Image* image);
    void processMenu(IGameMenuElement* menuElement);
    int method_210();
    void method_211(int var1);
    void method_215(int64_t var1);
    void removeOkAndBackCommands();
    void addOkAndBackCommands();
    /* synchronized */ void method_202(Graphics* var1);
    void processKeyCode(const Keys keyCode);
};
