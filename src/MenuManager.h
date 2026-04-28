#pragma once

#include <vector>
#include <string>
#include <memory>

#include "IMenuManager.h"

class Micro;
class RecordManager;
class Command;
class GameMenu;
class PhysicsElemOrMenuItem;
class SettingsStringRender;
class RecordStore;
class Image;
class TextRender;
class Graphics;
class Displayable;
class IGameMenuElement;

class MenuManager : public IMenuManager {
private:
    // Saved state buffer (19 bytes) - see initPart case 2 for layout
    std::vector<int8_t> savedStateBuffer;
    Micro* micro;
    RecordManager* recordManager;
    Command* okCommand;
    Command* backCommand;
    // Main menu screens
    GameMenu* mainMenu;
    GameMenu* playMenu;
    GameMenu* optionsMenu;
    GameMenu* aboutMenu;
    GameMenu* helpMenu;
    GameMenu* confirmClearHighscoresMenu;
    GameMenu* confirmFullResetMenu;
    GameMenu* finishedTrackMenu;
    GameMenu* ingameMenu;
    // Menu navigation elements
    PhysicsElemOrMenuItem* playMenuTask;
    PhysicsElemOrMenuItem* optionsTask;
    PhysicsElemOrMenuItem* helpTask;
    SettingsStringRender* levelSetting;
    GameMenu* levelSelectionMenu;
    SettingsStringRender* trackSetting;
    // Parent menu when track selection is active
    GameMenu* trackSelectionParentMenu;
    SettingsStringRender* leagueSetting;
    GameMenu* leagueSelectionMenu;
    GameMenu* highscoreMenu;
    PhysicsElemOrMenuItem* highscoreTask;
    SettingsStringRender* startTask;
    SettingsStringRender* perspectiveSetting;
    SettingsStringRender* shadowsSetting;
    SettingsStringRender* driverSpriteSetting;
    SettingsStringRender* bikeSpriteSetting;
    SettingsStringRender* inputSetting;
    SettingsStringRender* lookAheadSetting;
    PhysicsElemOrMenuItem* clearHighscoreTask;
    PhysicsElemOrMenuItem* fullResetTask;
    SettingsStringRender* confirmYesSetting;
    SettingsStringRender* confirmNoSetting;
    PhysicsElemOrMenuItem* aboutTask;
    GameMenu* helpObjectiveMenu;
    PhysicsElemOrMenuItem* helpObjectiveTask;
    GameMenu* helpKeysMenu;
    PhysicsElemOrMenuItem* helpKeysTask;
    GameMenu* helpUnlockingMenu;
    PhysicsElemOrMenuItem* helpUnlockingTask;
    GameMenu* helpHighscoreDescriptionMenu;
    PhysicsElemOrMenuItem* helpHighscoreTask;
    GameMenu* helpOptionsDescriptionMenu;
    PhysicsElemOrMenuItem* helpOptionsTask;
    GameMenu* enterNameMenu;
    SettingsStringRender* backSetting;
    SettingsStringRender* playMenuSetting;
    SettingsStringRender* continueSetting;
    SettingsStringRender* goToMainSetting;
    SettingsStringRender* exitGameSetting;
    // "Restart: [track name]"
    SettingsStringRender* restartTrackSetting;
    // "Next: [track name]"
    SettingsStringRender* nextTrackSetting;
    // "Ok" for highscore entry
    SettingsStringRender* okSetting;
    // "Name - [player name]"
    SettingsStringRender* nameEntrySetting;
    int64_t finishTime;
    // For display formatting
    int finishTimeSeconds;
    // For display formatting
    int finishTimeCentiseconds;
    // Formatted time string (MM:SS.cc)
    std::string finishTimeFormatted;
    // 3-character player name for highscores
    char* playerName;
    // Array[3]: max unlocked track per level
    char maxUnlockedTracksPerLevel[4];
    char defaultPlayerName[4] = "AAA";
    // Number of unlocked leagues (0-3)
    int8_t maxAvailableLeagues = 0;
    // Number of unlocked levels (1-3)
    int8_t maxAvailableLevels = 0;
    // Last selected track for each level
    std::vector<int> lastSelectedTrackPerLevel = { 0, 0, 0 };
    // Track names by level
    std::vector<std::vector<std::string>> trackNamesByLevel;
    // Current league names
    std::vector<std::string> leagueNames = std::vector<std::string>(3);
    // All 4 league names (including 325cc)
    std::vector<std::string> allLeagueNames;
    RecordStore* recordStore;
    // Record ID for saved state (typo in original: "recorc")
    int savedStateRecordId = -1;
    bool isRecordStoreOpened;
    // raster.png
    std::unique_ptr<Image> menuBackgroundImage;
    // Clickable link to codebrew.se
    TextRender* codebrewLinkText;
    int savedLevelBeforeMenu = 0;
    int savedTrackBeforeMenu = 0;
    bool isAllTracksCompletedAtLevel = false;
    // Flag to start race without menu interaction
    bool shouldStartRaceImmediately = false;
    // Level display names
    std::vector<std::string> levelNames = { "Easy", "Medium", "Pro" };
    int64_t menuLoopStartTime = 0L;
    int8_t perspectiveDisabled = 0;
    int8_t shadowsDisabled = 0;
    int8_t driverSpriteDisabled = 0;
    int8_t bikeSpriteDisabled = 0;
    // Selected input method (0-2)
    int8_t inputMethod = 0;
    int8_t lookAheadDisabled = 0;
    int8_t lastSelectedTrack = 0;
    int8_t lastSelectedLevel = 0;
    int8_t lastSelectedLeague = 0;
    int8_t unknownGraphicsSetting = 0; // Graphics setting passed to setInputConfigEnabled
    int8_t unknownSetting15 = 0; // Unknown setting from save index 15
    std::vector<std::string> onOffOptions; // {"On", "Off"}
    // {"Keyset 1", "Keyset 2", "Keyset 3"}
    std::vector<std::string> inputMethodNames;
    // Empty text for help menu separators
    std::unique_ptr<TextRender> helpSeparatorText;
    // Alert alert = nullptr; // TODO

    void addMultilineTextToMenu(GameMenu* menu, const std::string& text);
    void saveHighscoreAndShowResults();
    void tileBackgroundImage(Graphics* graphics);
    void processNonFireKey(int keyCode);
    std::vector<int8_t> loadPlayerName(int index, int8_t defaultValue);
    int8_t loadSavedStateValue(int index, int8_t defaultValue);
    void savePlayerNameToBuffer(int index, char* name);
    std::string formatTime(int64_t timeMs);
    void setSavedStateValue(int index, int8_t value);
    void performFullReset();
    int countRecordStoresForLevel(int levelIndex);

public:
    GameMenu* currentGameMenu;
    // Currently viewed league in highscore screen
    int highscoreLeagueViewIndex = 0;
    // Prevents menu rendering over game
    bool isMenuRenderingBlocked = false;

    MenuManager(Micro* micro);
    // Initialize in phases 1-7
    void initializePhase(int phase);
    // Returns and clears shouldStartRaceImmediately
    bool consumeShouldStartRaceFlag();
    void requestRepaint();
    int getCanvasHeight();
    int getCanvasWidth();
    // 0=main, 1=ingame, 2=finished
    void runMenuLoop(int menuType);
    void handleCommand(Command* command, Displayable* displayable);
    GameMenu* getCurrentMenu();
    void switchToMenu(GameMenu* menu, bool skipSelectionReset);
    void refreshHighscoreDisplay(int leagueIndex);
    /* synchronized */ void saveStateAndCloseRecordStore();
    void saveSettingsToBuffer();
    void runAlertThread(); // TODO: Thread entry point for alert display
    void showAlert(const std::string& title, const std::string& message, Image* image);
    void handleMenuSelection(IGameMenuElement* element);
    // Bitfield: bit 0=bike sprite, bit 1=driver sprite
    int getGraphicsFlags();
    void applyGraphicsFlags(int flags);
    int getSelectedLevel();
    int getSelectedTrack();
    int getSelectedLeague();
    void setFinishTime(int64_t timeMs);
    void removeCommands();
    void addCommands();
    /* synchronized */ void renderMenuOverGame(Graphics* graphics);
    void processKey(int keyCode);
};
