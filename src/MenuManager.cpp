#include "MenuManager.h"
#include "rms/RecordStoreException.h"
#include "rms/RecordStoreNotOpenException.h"
#include "rms/RecordStore.h"
#include "lcdui/FontStorage.h"
#include "TextRender.h"
#include "RecordManager.h"
#include "Micro.h"
#include "LevelLoader.h"
#include "GameMenu.h"
#include "SettingsStringRender.h"
#include "utils/Time.h"

MenuManager::MenuManager(Micro* micro)
{
    this->micro = micro;
    helpSeparatorText = std::make_unique<TextRender>("", micro); // Empty separator for help menus
}

void MenuManager::initializePhase(int phase)
{
    int levelIndex;
    switch (phase) {
    case 1:
        // Initialize basic state and open record store
        playerName = defaultPlayerName;
        onOffOptions = { "On", "Off" };
        inputMethodNames = { "Keyset 1", "Keyset 2", "Keyset 3" };
        recordManager = new RecordManager();
        finishTime = -1L;
        finishTimeSeconds = -1;
        finishTimeCentiseconds = -1;
        finishTimeFormatted.clear();
        isRecordStoreOpened = false;
        savedStateBuffer = std::vector<int8_t>(19);

        // Initialize buffer to -127 (uninitialized marker)
        for (int i = 0; i < 19; ++i) {
            savedStateBuffer[i] = -127;
        }

        try {
            recordStore = RecordStore::openRecordStore("GDTRStates", true);
            isRecordStoreOpened = true;
            return;
        } catch (RecordStoreException& e) {
            isRecordStoreOpened = false;
            return;
        }
    case 2: {
        // Load saved state from record store
        savedStateRecordId = -1;

        RecordEnumeration* records;
        try {
            records = recordStore->enumerateRecords(nullptr, nullptr, false);
        } catch (RecordStoreNotOpenException& e) {
            return;
        }

        std::vector<int8_t> recordData;
        if (records->numRecords() > 0) {
            try {
                recordData = records->nextRecord();
                records->reset();
                savedStateRecordId = records->nextRecordId();
            } catch (RecordStoreException& e) {
                return;
            }

            if (recordData.size() <= 19) {
                for (std::size_t i = 0; i < recordData.size(); ++i) {
                    savedStateBuffer[i] = recordData[i];
                }
            }

            records->destroy();
        }

        // Load player name from buffer (indices 16-18)
        recordData = loadPlayerName(16, (int8_t)-1);
        if (!recordData.empty() && recordData[0] != -1) {
            for (levelIndex = 0; levelIndex < 3; ++levelIndex) {
                playerName[levelIndex] = recordData[levelIndex];
            }
        }

        // Cheat code detection: "RKE" unlocks all
        if (playerName[0] == 82 && playerName[1] == 75 && playerName[2] == 69) {
            maxAvailableLeagues = 3; // All 4 leagues
            maxAvailableLevels = 2; // Cheat mode (unlocks all levels)
            maxUnlockedTracksPerLevel[0] = (int8_t)(micro->levelLoader->trackNames[0].size() - 1);
            maxUnlockedTracksPerLevel[1] = (int8_t)(micro->levelLoader->trackNames[1].size() - 1);
            maxUnlockedTracksPerLevel[2] = (int8_t)(micro->levelLoader->trackNames[2].size() - 1);
            return;
        }

        // Default progression state
        maxAvailableLeagues = 0;
        maxAvailableLevels = 1;
        maxUnlockedTracksPerLevel[0] = 0;
        maxUnlockedTracksPerLevel[1] = 0;
        maxUnlockedTracksPerLevel[2] = -1;
    }
        return;
    case 3:
        // Apply settings from saved state
        perspectiveDisabled = loadSavedStateValue(0, perspectiveDisabled);
        shadowsDisabled = loadSavedStateValue(1, shadowsDisabled);
        driverSpriteDisabled = loadSavedStateValue(2, driverSpriteDisabled);
        bikeSpriteDisabled = loadSavedStateValue(3, bikeSpriteDisabled);
        inputMethod = loadSavedStateValue(14, inputMethod);
        lookAheadDisabled = loadSavedStateValue(4, lookAheadDisabled);
        lastSelectedTrack = loadSavedStateValue(11, lastSelectedTrack);
        lastSelectedLevel = loadSavedStateValue(10, lastSelectedLevel);
        lastSelectedLeague = loadSavedStateValue(12, lastSelectedLeague);
        unknownSetting15 = loadSavedStateValue(15, unknownSetting15);
        savedLevelBeforeMenu = lastSelectedLevel;
        savedTrackBeforeMenu = lastSelectedTrack;

        // Skip progression loading for cheat code
        if (playerName[0] != 82 || playerName[1] != 75 || playerName[2] != 69) {
            maxAvailableLeagues = loadSavedStateValue(5, maxAvailableLeagues);
            maxAvailableLevels = loadSavedStateValue(6, maxAvailableLevels);

            for (levelIndex = 0; levelIndex < 3; ++levelIndex) {
                maxUnlockedTracksPerLevel[levelIndex] = loadSavedStateValue(7 + levelIndex, maxUnlockedTracksPerLevel[levelIndex]);
            }
        }

        try {
            lastSelectedTrackPerLevel.at(lastSelectedLevel) = lastSelectedTrack;
        } catch (std::exception& e) {
            lastSelectedLevel = 0;
            lastSelectedTrack = 0;
            lastSelectedTrackPerLevel[lastSelectedLevel] = lastSelectedTrack;
        }

        // Apply graphics and physics settings
        LevelLoader::isEnabledPerspective = perspectiveDisabled == 0;
        LevelLoader::isEnabledShadows = shadowsDisabled == 0;
        micro->gamePhysics->setEnableLookAhead(lookAheadDisabled == 0);
        micro->gameCanvas->setInputConfigIndex(inputMethod);
        micro->gameCanvas->setInputConfigEnabled(unknownGraphicsSetting == 0); // unknownGraphicsSetting never read
        allLeagueNames = { "100cc", "175cc", "220cc", "325cc" };
        trackNamesByLevel = micro->levelLoader->trackNames;
        if (maxAvailableLeagues < 3) {
            this->leagueNames = { "100cc", "175cc", "220cc" };
        } else {
            this->leagueNames = allLeagueNames;
        }

        highscoreLeagueViewIndex = lastSelectedLeague;
        return;
    case 4: {
        // Create main menu screens
        mainMenu = new GameMenu("Main", micro, nullptr);
        playMenu = new GameMenu("Play", micro, mainMenu);
        optionsMenu = new GameMenu("Options", micro, mainMenu);
        aboutMenu = new GameMenu("About", micro, mainMenu);
        helpMenu = new GameMenu("Help", micro, mainMenu);
        backSetting = new SettingsStringRender("Back", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        goToMainSetting = new SettingsStringRender("Go to Main", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        continueSetting = new SettingsStringRender("Continue", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        playMenuSetting = new SettingsStringRender("Play Menu", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);

        std::shared_ptr boldSmallFont = FontStorage::getFont(Font::STYLE_BOLD, Font::SIZE_SMALL);
        if (aboutMenu->xPos + boldSmallFont->stringWidth("http://www.codebrew.se/") >= getCanvasWidth()) {
            codebrewLinkText = new TextRender("www.codebrew.se", micro);
        } else {
            codebrewLinkText = new TextRender("http://www.codebrew.se/", micro);
        }

        codebrewLinkText->setFont(boldSmallFont);
        highscoreMenu = new GameMenu("Highscore", micro, playMenu);
        finishedTrackMenu = new GameMenu("Finished!", micro, playMenu);
    }
        return;
    case 5:
        // Create ingame and confirmation menus
        ingameMenu = new GameMenu("Ingame", micro, playMenu);
        enterNameMenu = new GameMenu("Enter Name", micro, finishedTrackMenu, playerName);
        confirmClearHighscoresMenu = new GameMenu("Confirm Clear", micro, optionsMenu);
        confirmFullResetMenu = new GameMenu("Confirm Reset", micro, confirmClearHighscoresMenu);
        playMenuTask = new PhysicsElemOrMenuItem("Play Menu", playMenu, this);
        optionsTask = new PhysicsElemOrMenuItem("Options", optionsMenu, this);
        helpTask = new PhysicsElemOrMenuItem("Help", helpMenu, this);
        aboutTask = new PhysicsElemOrMenuItem("About", aboutMenu, this);
        exitGameSetting = new SettingsStringRender("Exit Game", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        mainMenu->addMenuElement(playMenuTask);
        mainMenu->addMenuElement(optionsTask);
        mainMenu->addMenuElement(helpTask);
        mainMenu->addMenuElement(aboutTask);
        mainMenu->addMenuElement(exitGameSetting);
        levelSetting = new SettingsStringRender("Level", lastSelectedLevel, this, levelNames, false, micro, playMenu, false);
        trackSetting = new SettingsStringRender("Track", lastSelectedTrackPerLevel[lastSelectedLevel], this, trackNamesByLevel[lastSelectedLevel], false, micro, playMenu, false);
        leagueSetting = new SettingsStringRender("League", lastSelectedLeague, this, leagueNames, false, micro, playMenu, false);

        try {
            trackSetting->setAvailableOptions(maxUnlockedTracksPerLevel[lastSelectedLevel]);
        } catch (std::exception& e) {
            trackSetting->setAvailableOptions(0);
        }

        levelSetting->setAvailableOptions(maxAvailableLevels);
        leagueSetting->setAvailableOptions(maxAvailableLeagues);
        highscoreTask = new PhysicsElemOrMenuItem("Highscore", highscoreMenu, this);
        highscoreMenu->addMenuElement(backSetting);
        startTask = new SettingsStringRender("Start>", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        playMenu->addMenuElement(startTask);
        playMenu->addMenuElement(levelSetting);
        playMenu->addMenuElement(trackSetting);
        playMenu->addMenuElement(leagueSetting);
        playMenu->addMenuElement(highscoreTask);
        playMenu->addMenuElement(goToMainSetting);

        perspectiveSetting = new SettingsStringRender("Perspective", perspectiveDisabled, this, onOffOptions, true, micro, optionsMenu, false);
        shadowsSetting = new SettingsStringRender("Shadows", shadowsDisabled, this, onOffOptions, true, micro, optionsMenu, false);
        driverSpriteSetting = new SettingsStringRender("Driver sprite", driverSpriteDisabled, this, onOffOptions, true, micro, optionsMenu, false);
        bikeSpriteSetting = new SettingsStringRender("Bike sprite", bikeSpriteDisabled, this, onOffOptions, true, micro, optionsMenu, false);
        inputSetting = new SettingsStringRender("Input", inputMethod, this, inputMethodNames, false, micro, optionsMenu, false);
        lookAheadSetting = new SettingsStringRender("Look ahead", lookAheadDisabled, this, onOffOptions, true, micro, optionsMenu, false);
        clearHighscoreTask = new PhysicsElemOrMenuItem("Clear highscore", confirmClearHighscoresMenu, this);
        return;
    case 6:
        // Populate options menu and create help menus
        optionsMenu->addMenuElement(perspectiveSetting);
        optionsMenu->addMenuElement(shadowsSetting);
        optionsMenu->addMenuElement(driverSpriteSetting);
        optionsMenu->addMenuElement(bikeSpriteSetting);
        optionsMenu->addMenuElement(inputSetting);
        optionsMenu->addMenuElement(lookAheadSetting);
        optionsMenu->addMenuElement(clearHighscoreTask);
        optionsMenu->addMenuElement(backSetting);
        confirmNoSetting = new SettingsStringRender("No", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        confirmYesSetting = new SettingsStringRender("Yes", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        fullResetTask = new PhysicsElemOrMenuItem("Full Reset", confirmFullResetMenu, this);
        addMultilineTextToMenu(confirmClearHighscoresMenu, "Clearing the highscores can not be undone. It will remove all the registered times on all tracks.");
        addMultilineTextToMenu(confirmClearHighscoresMenu, "Would you like to clear the highscores?");
        confirmClearHighscoresMenu->addMenuElement(confirmNoSetting);
        confirmClearHighscoresMenu->addMenuElement(confirmYesSetting);
        confirmClearHighscoresMenu->addMenuElement(fullResetTask);
        addMultilineTextToMenu(confirmFullResetMenu, "A full reset can not be undone. It will relock all tracks and leagues and clear back all settings to default. A full reset will exit the application.");
        addMultilineTextToMenu(confirmFullResetMenu, "Would you like to do a full reset?");
        confirmFullResetMenu->addMenuElement(confirmNoSetting);
        confirmFullResetMenu->addMenuElement(confirmYesSetting);
        helpObjectiveMenu = new GameMenu("Objective", micro, helpMenu);
        helpObjectiveTask = new PhysicsElemOrMenuItem("Objective", helpObjectiveMenu, this);
        addMultilineTextToMenu(helpObjectiveMenu, "Race to the finish line as fast as you can without crashing. By leaning forward and backward you can adjust the rotation of your bike. By landing on both wheels after jumping, your bike won't crash as easily. Beware, the levels tend to get harder and harder...");
        helpObjectiveMenu->addMenuElement(backSetting);
        helpMenu->addMenuElement(helpObjectiveTask);
        helpKeysMenu = new GameMenu("Keys", micro, helpMenu);
        helpKeysTask = new PhysicsElemOrMenuItem("Keys", helpKeysMenu, this);
        addMultilineTextToMenu(helpKeysMenu, "- " + inputMethodNames[0] + " -");
        addMultilineTextToMenu(helpKeysMenu, "UP accelerates, DOWN brakes, RIGHT leans forward and LEFT leans backward. 1 accelerates and leans backward. 3 accelerates and leans forward. 7 brakes and leans backward. 9 brakes and leans forward.");
        helpKeysMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpKeysMenu, "- " + inputMethodNames[1] + " -");
        addMultilineTextToMenu(helpKeysMenu, "1 accelerates, 4 brakes, 6 leans forward and 5 leans backward.");
        helpKeysMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpKeysMenu, "- " + inputMethodNames[2] + " -");
        addMultilineTextToMenu(helpKeysMenu, "3 accelerates, 6 brakes, 5 leans forward and 4 leans backward.");
        helpKeysMenu->addMenuElement(backSetting);
        helpMenu->addMenuElement(helpKeysTask);
        helpUnlockingMenu = new GameMenu("Unlocking", micro, helpMenu);
        helpUnlockingTask = new PhysicsElemOrMenuItem("Unlocking", helpUnlockingMenu, this);
        addMultilineTextToMenu(helpUnlockingMenu, "By completing the easier levels, new levels will be unlocked. You will also gain access to higher leagues where more advanced bikes with different characteristics are available.");
        helpUnlockingMenu->addMenuElement(backSetting);
        helpMenu->addMenuElement(helpUnlockingTask);
        helpHighscoreDescriptionMenu = new GameMenu("Highscore", micro, helpMenu);
        helpHighscoreTask = new PhysicsElemOrMenuItem("Highscore", helpHighscoreDescriptionMenu, this);
        addMultilineTextToMenu(helpHighscoreDescriptionMenu, "The three best times on every track are saved for each league. When beating a time on a track you will be asked to enter your name. The highscores can be viewed from the Play Menu. By pressing left and right in the highscore view you can view the highscore for a specific league. The highscore can be cleared from the options menu.");
        helpHighscoreDescriptionMenu->addMenuElement(backSetting);
        helpMenu->addMenuElement(helpHighscoreTask);
        return;
    case 7:
        // Create help options description menu
        helpOptionsDescriptionMenu = new GameMenu("Options", micro, helpMenu);
        helpOptionsTask = new PhysicsElemOrMenuItem("Options", helpOptionsDescriptionMenu, this);

        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Perspective: On/Off");
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Default: <On>. Turns on and off the perspective view of the tracks.");
        helpOptionsDescriptionMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Shadows: On/Off");
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Default: <On>. Turns on and off the shadows.");
        helpOptionsDescriptionMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Driver Sprite: On / Off");
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Default: <On>. <On> uses a texture for the driver. <Off> uses line graphics.");
        helpOptionsDescriptionMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Bike Sprite: On / Off");
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Default: <On>. <On> uses a texture for the bike. <Off> uses line graphics.");
        helpOptionsDescriptionMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Input: Keyset 1,2,3 ");
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Default: <1>. Determines which type of input should be used when playing. See \"Keys\" in the help menu for more info.");
        helpOptionsDescriptionMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Look ahead: On/Off");
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Default: <On>. Turns on and off smart camera movement.");
        helpOptionsDescriptionMenu->addMenuElement(helpSeparatorText.get());
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Clear highscore");
        addMultilineTextToMenu(helpOptionsDescriptionMenu, "Lets you clear the highscores. Here you can also do a \"Full Reset\" which will reset the game to original state (clear settings, highscores, unlocked levels and leagues).");
        helpOptionsDescriptionMenu->addMenuElement(helpSeparatorText.get());
        helpOptionsDescriptionMenu->addMenuElement(backSetting);
        helpMenu->addMenuElement(helpOptionsTask);
        helpMenu->addMenuElement(backSetting);
        addMultilineTextToMenu(aboutMenu, "\"Gravity Defied - Trial Racing\" v1.0 by Codebrew Software © 2004.");
        addMultilineTextToMenu(aboutMenu, "brought 2 you by pascha.                For information visit:");
        aboutMenu->addMenuElement(codebrewLinkText);
        aboutMenu->addMenuElement(backSetting);
        nextTrackSetting = new SettingsStringRender("Track: " + micro->levelLoader->getName(0, 1), 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        restartTrackSetting = new SettingsStringRender("Restart: " + micro->levelLoader->getName(0, 0), 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        ingameMenu->addMenuElement(continueSetting);
        ingameMenu->addMenuElement(restartTrackSetting);
        ingameMenu->addMenuElement(optionsTask);
        ingameMenu->addMenuElement(helpTask);
        ingameMenu->addMenuElement(playMenuSetting);
        okSetting = new SettingsStringRender("Ok", 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        nameEntrySetting = new SettingsStringRender("Name - " + std::string(playerName), 0, this, std::vector<std::string>(), false, micro, mainMenu, true);
        okCommand = new Command("Ok", 4, 1);
        backCommand = new Command("Back", 2, 1);
        switchToMenu(mainMenu, false);

        menuBackgroundImage = std::make_unique<Image>("raster.png");

    default:
        break;
    }
}

void MenuManager::addMultilineTextToMenu(GameMenu* menu, const std::string& text)
{
    std::vector<TextRender*> textRenders = TextRender::makeMultilineTextRenders(text, micro);

    for (std::size_t i = 0; i < textRenders.size(); ++i) {
        menu->addMenuElement(textRenders[i]);
    }
}

bool MenuManager::consumeShouldStartRaceFlag()
{
    if (shouldStartRaceImmediately) {
        shouldStartRaceImmediately = false;
        return true;
    } else {
        return false;
    }
}

void MenuManager::saveHighscoreAndShowResults()
{
    // Save the record and update progression
    recordManager->addRecord(leagueSetting->getCurrentOptionPos(), playerName, finishTime);
    recordManager->writeRecordInfo();
    isAllTracksCompletedAtLevel = false;
    finishedTrackMenu->clearVector();
    finishedTrackMenu->addMenuElement(new TextRender("Time: " + finishTimeFormatted, micro));
    std::vector<std::string> recordDescriptions = recordManager->getRecordDescription(leagueSetting->getCurrentOptionPos());

    for (std::size_t i = 0; i < recordDescriptions.size(); ++i) {
        if (recordDescriptions[i] != "") {
            finishedTrackMenu->addMenuElement(new TextRender(std::to_string(i + 1) + "." + recordDescriptions[i], micro));
        }
    }

    recordManager->closeRecordStore();
    int8_t newlyUnlockedLeague = -1;
    if (trackSetting->getMaxAvailableOptionPos() >= trackSetting->getCurrentOptionPos()) {
        trackSetting->setAvailableOptions(trackSetting->getCurrentOptionPos() + 1 < maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()] ? maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()] : trackSetting->getCurrentOptionPos() + 1);
        maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()] = (int8_t)trackSetting->getMaxAvailableOptionPos() < maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()] ? maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()] : (int8_t)trackSetting->getMaxAvailableOptionPos();
    }

    // Check if all tracks at this level are completed
    if (trackSetting->getCurrentOptionPos() == trackSetting->getMaxOptionPos()) {
        isAllTracksCompletedAtLevel = true;
        switch (levelSetting->getCurrentOptionPos()) {
        case 0:
            if (newlyUnlockedLeague < 1) {
                newlyUnlockedLeague = 1;
                newlyUnlockedLeague = 1;
                leagueSetting->setAvailableOptions(newlyUnlockedLeague);
            }
            break;
        case 1:
            if (newlyUnlockedLeague < 2) {
                newlyUnlockedLeague = 2;
                newlyUnlockedLeague = 2;
                leagueSetting->setAvailableOptions(newlyUnlockedLeague);
            }
            break;
        case 2:
            if (newlyUnlockedLeague < 3) {
                newlyUnlockedLeague = 3;
                newlyUnlockedLeague = 3;
                leagueSetting->setOptionsList(allLeagueNames);
                leagueNames = allLeagueNames;
                leagueSetting->setAvailableOptions(newlyUnlockedLeague);
            }
        }

        levelSetting->setAvailableOptions(levelSetting->getMaxAvailableOptionPos() + 1);
        if (maxUnlockedTracksPerLevel[levelSetting->getMaxAvailableOptionPos()] == -1) {
            maxUnlockedTracksPerLevel[levelSetting->getMaxAvailableOptionPos()] = 0;
        }
    }

    int completedTrackCount = countRecordStoresForLevel(levelSetting->getCurrentOptionPos());
    addMultilineTextToMenu(finishedTrackMenu, completedTrackCount + " of " + std::to_string(trackNamesByLevel[levelSetting->getCurrentOptionPos()].size()) + " tracks in " + levelNames[levelSetting->getCurrentOptionPos()] + " completed.");
    if (!isAllTracksCompletedAtLevel) {
        restartTrackSetting->setText("Restart: " + micro->levelLoader->getName(levelSetting->getCurrentOptionPos(), trackSetting->getCurrentOptionPos()));
        nextTrackSetting->setText("Next: " + micro->levelLoader->getName(savedLevelBeforeMenu, savedTrackBeforeMenu + 1));
    } else {
        if (levelSetting->getCurrentOptionPos() < levelSetting->getMaxOptionPos()) {
            levelSetting->setCurentOptionPos(levelSetting->getCurrentOptionPos() + 1);
            trackSetting->setCurentOptionPos(0);
            trackSetting->setAvailableOptions(maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()]);
        }

        if (newlyUnlockedLeague != -1) {
            addMultilineTextToMenu(finishedTrackMenu, "Congratultions! You have successfully unlocked a new league: " + leagueNames[newlyUnlockedLeague]);
            if (newlyUnlockedLeague == 3) {
                finishedTrackMenu->addMenuElement(new TextRender("Enjoy...", micro));
            }

            showAlert("League unlocked", "You have successfully unlocked a new league: " + leagueNames[newlyUnlockedLeague], nullptr);
        } else {
            bool allTracksCompleted = true;

            for (int levelIndex = 0; levelIndex < 3; ++levelIndex) {
                if (maxUnlockedTracksPerLevel[levelIndex] != static_cast<int>(micro->levelLoader->trackNames[levelIndex].size() - 1)) {
                    allTracksCompleted = false;
                }
            }

            if (!allTracksCompleted) {
                addMultilineTextToMenu(finishedTrackMenu, "You have completed all tracks at this level.");
            }
        }
    }

    if (!isAllTracksCompletedAtLevel) {
        finishedTrackMenu->addMenuElement(nextTrackSetting);
    }

    restartTrackSetting->setText("Restart: " + micro->levelLoader->getName(savedLevelBeforeMenu, savedTrackBeforeMenu));
    finishedTrackMenu->addMenuElement(restartTrackSetting);
    finishedTrackMenu->addMenuElement(playMenuSetting);
    switchToMenu(finishedTrackMenu, false);
}

void MenuManager::requestRepaint()
{
    micro->gameCanvas->repaint();
}

int MenuManager::getCanvasHeight()
{
    return micro->gameCanvas->getHeight();
}

int MenuManager::getCanvasWidth()
{
    return micro->gameCanvas->getWidth();
}

void MenuManager::runMenuLoop(int menuType)
{
    isMenuRenderingBlocked = false;
    switch (menuType) {
    // main menu
    case 0:
        switchToMenu(mainMenu, false);
        micro->gamePhysics->enableGenerateInputAI();
        shouldStartRaceImmediately = true;
        break;
    // in-game menu
    case 1:
        savedLevelBeforeMenu = levelSetting->getCurrentOptionPos();
        savedTrackBeforeMenu = trackSetting->getCurrentOptionPos();
        restartTrackSetting->setText("Restart: " + micro->levelLoader->getName(savedLevelBeforeMenu, savedTrackBeforeMenu));
        shouldStartRaceImmediately = false;
        switchToMenu(ingameMenu, false);
        break;
    // finished track menu
    case 2: {
        menuLoopStartTime = Time::currentTimeMillis();
        finishedTrackMenu->clearVector();
        savedLevelBeforeMenu = levelSetting->getCurrentOptionPos();
        savedTrackBeforeMenu = trackSetting->getCurrentOptionPos();
        recordManager->openRecordStore(levelSetting->getCurrentOptionPos(), trackSetting->getCurrentOptionPos());
        int newRecordPosition = recordManager->getPosOfNewRecord(leagueSetting->getCurrentOptionPos(), finishTime);
        finishTimeFormatted = formatTime(finishTime);
        if (newRecordPosition >= 0 && newRecordPosition <= 2) {
            // New record is in top 3 - show name entry
            TextRender* firstPlaceText = new TextRender("", micro);
            firstPlaceText->setDx(GameCanvas::spriteSizeX[5] + 1);
            switch (newRecordPosition) {
            case 0:
                firstPlaceText->setText("First place!");
                firstPlaceText->setDrawSprite(true, 5);
                break;
            case 1:
                firstPlaceText->setText("Second place!");
                firstPlaceText->setDrawSprite(true, 6);
                break;
            case 2:
                firstPlaceText->setText("Third place!");
                firstPlaceText->setDrawSprite(true, 7);
            }

            finishedTrackMenu->addMenuElement(firstPlaceText);
            TextRender* timeText = new TextRender("" + finishTimeFormatted, micro);
            timeText->setDx(GameCanvas::spriteSizeX[5] + 1);
            finishedTrackMenu->addMenuElement(timeText);
            finishedTrackMenu->addMenuElement(okSetting);
            finishedTrackMenu->addMenuElement(nameEntrySetting);
            switchToMenu(finishedTrackMenu, false);
            isMenuRenderingBlocked = false;
        } else {
            saveHighscoreAndShowResults();
        }
    } break;
    default:
        switchToMenu(mainMenu, false);
        break;
    }

    int64_t loopStartTime = Time::currentTimeMillis();
    micro->gameCanvas->isDrawingTime = false;
    int64_t lastFrameTime = 0L;
    int8_t targetFrameTimeMs = 50; // 20 FPS target
    micro->gamePhysics->captureRenderSnapshot();
    micro->gameToMenu();

    while (Micro::isInGameMenu && Micro::isGameLoopRunning && currentGameMenu != nullptr) {
        int64_t currentTime;
        if (micro->gamePhysics->isGenerateInputAI) {
            int physicsResult;
            if ((physicsResult = micro->gamePhysics->updatePhysics()) != 0 && physicsResult != 4) {
                micro->gamePhysics->resetPhysicsState(true);
            }

            micro->gamePhysics->captureRenderSnapshot();
            requestRepaint();
            if ((currentTime = Time::currentTimeMillis()) - lastFrameTime < (int64_t)targetFrameTimeMs) {
                // Frame limiting - sleep to maintain 20 FPS
                Time::sleep((int64_t)targetFrameTimeMs - (currentTime - lastFrameTime) < 1L ? 1L : (int64_t)targetFrameTimeMs - (currentTime - lastFrameTime));

                lastFrameTime = Time::currentTimeMillis();
            } else {
                lastFrameTime = currentTime;
            }
        } else {
            targetFrameTimeMs = 50;
            if ((currentTime = Time::currentTimeMillis()) - lastFrameTime < (int64_t)targetFrameTimeMs) {
                Time::sleep((int64_t)targetFrameTimeMs - (currentTime - lastFrameTime) < 1L ? 1L : (int64_t)targetFrameTimeMs - (currentTime - lastFrameTime));

                lastFrameTime = Time::currentTimeMillis();
            } else {
                lastFrameTime = currentTime;
            }

            if (Micro::isInGameMenu) {
                requestRepaint();
            }
        }
    }

    micro->timeMs += Time::currentTimeMillis() - loopStartTime;
    micro->gameCanvas->isDrawingTime = true;
    if (currentGameMenu == nullptr) {
        Micro::isGameLoopRunning = false;
    }
}

/* synchronized */ void MenuManager::renderMenuOverGame(Graphics* graphics)
{
    // Render menu on top of game (for pause menu)
    if (currentGameMenu != nullptr && !isMenuRenderingBlocked) {
        micro->gameCanvas->drawGame(graphics);
        tileBackgroundImage(graphics);
        currentGameMenu->render(graphics);
    }
}

void MenuManager::tileBackgroundImage(Graphics* graphics)
{
    // Tile the raster background image across the entire canvas
    for (int y = 0; y < getCanvasHeight(); y += menuBackgroundImage->getHeight()) {
        for (int x = 0; x < getCanvasWidth(); x += menuBackgroundImage->getWidth()) {
            graphics->drawImage(menuBackgroundImage.get(), x, y, 20);
        }
    }
}

void MenuManager::processNonFireKey(int keyCode)
{
    if (micro->gameCanvas->getGameAction(keyCode) != 8) {
        // if not fire
        processKey(keyCode);
    }
}

void MenuManager::processKey(int keyCode)
{
    if (currentGameMenu != nullptr) {
        switch (micro->gameCanvas->getGameAction(keyCode)) {
        case 1: // UP
            currentGameMenu->processGameActionUp();
            return;
        case 2: // LEFT
            currentGameMenu->processGameActionUpd(3);
            if (currentGameMenu == highscoreMenu) {
                --highscoreLeagueViewIndex;
                if (highscoreLeagueViewIndex < 0) {
                    highscoreLeagueViewIndex = 0;
                }

                refreshHighscoreDisplay(highscoreLeagueViewIndex);
            }
        case 3:
        case 4:
        case 7:
        default:
            break;
        case 5: // RIGHT
            currentGameMenu->processGameActionUpd(2);
            if (currentGameMenu == highscoreMenu) {
                ++highscoreLeagueViewIndex;
                if (highscoreLeagueViewIndex > leagueSetting->getMaxAvailableOptionPos()) {
                    highscoreLeagueViewIndex = leagueSetting->getMaxAvailableOptionPos();
                }

                refreshHighscoreDisplay(highscoreLeagueViewIndex);
                return;
            }
            break;
        case 6: // DOWN
            currentGameMenu->processGameActionDown();
            return;
        case 8: // FIRE
            currentGameMenu->processGameActionUpd(1);
            return;
        }
    }
}

void MenuManager::handleCommand(Command* command, Displayable* displayable)
{
    (void)displayable;
    if (command == okCommand) {
        if (currentGameMenu != nullptr) {
            currentGameMenu->processGameActionUpd(1);
            return;
        }
    } else if (command == backCommand && currentGameMenu != nullptr) {
        if (currentGameMenu == ingameMenu) {
            micro->menuToGame();
            return;
        }

        switchToMenu(currentGameMenu->getGameMenu(), true);
    }
}

GameMenu* MenuManager::getCurrentMenu()
{
    return currentGameMenu;
}

void MenuManager::switchToMenu(GameMenu* menu, bool skipSelectionReset)
{
    micro->gameCanvas->removeCommand(backCommand);
    if (menu != mainMenu && menu != finishedTrackMenu && menu != nullptr) {
        micro->gameCanvas->addCommand(backCommand);
    }

    if (menu == highscoreMenu) {
        highscoreLeagueViewIndex = leagueSetting->getCurrentOptionPos();
        refreshHighscoreDisplay(highscoreLeagueViewIndex);
    } else if (menu == finishedTrackMenu) {
        playerName = enterNameMenu->getStrArr();
        nameEntrySetting->setText("Name - " + std::string(playerName));
    } else if (menu == playMenu) {
        trackSetting->setOptionsList(micro->levelLoader->trackNames[levelSetting->getCurrentOptionPos()]);
        if (currentGameMenu == trackSelectionParentMenu) {
            lastSelectedTrackPerLevel[levelSetting->getCurrentOptionPos()] = trackSetting->getCurrentOptionPos();
        }

        trackSetting->setAvailableOptions(maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()]);
        trackSetting->setCurentOptionPos(lastSelectedTrackPerLevel[levelSetting->getCurrentOptionPos()]);
    }

    if (menu == mainMenu || menu == playMenu) {
        micro->gamePhysics->enableGenerateInputAI();
    }

    currentGameMenu = menu;
    if (currentGameMenu != nullptr && !skipSelectionReset) {
        currentGameMenu->resetToFirstItem();
    }

    isMenuRenderingBlocked = false;
}

void MenuManager::refreshHighscoreDisplay(int leagueIndex)
{
    highscoreMenu->clearVector();
    recordManager->openRecordStore(levelSetting->getCurrentOptionPos(), trackSetting->getCurrentOptionPos());
    highscoreMenu->addMenuElement(new TextRender(micro->levelLoader->getName(levelSetting->getCurrentOptionPos(), trackSetting->getCurrentOptionPos()), micro));
    highscoreMenu->addMenuElement(new TextRender("LEAGUE: " + leagueSetting->getOptionsList()[leagueIndex], micro));
    std::vector<std::string> recordDescriptions = recordManager->getRecordDescription(leagueIndex);

    for (std::size_t i = 0; i < recordDescriptions.size(); ++i) {
        if (recordDescriptions[i] != "") {
            TextRender* recordText = new TextRender(std::to_string(i + 1) + "." + recordDescriptions[i], micro);
            recordText->setDx(GameCanvas::spriteSizeX[5] + 1);
            if (i == 0) {
                recordText->setDrawSprite(true, 5);
            } else if (i == 1) {
                recordText->setDrawSprite(true, 6);
            } else if (i == 2) {
                recordText->setDrawSprite(true, 7);
            }

            highscoreMenu->addMenuElement(recordText);
        }
    }

    recordManager->closeRecordStore();
    if (recordDescriptions[0] == "") {
        highscoreMenu->addMenuElement(new TextRender("No Highscores", micro));
    }

    highscoreMenu->addMenuElement(backSetting);
}

/* synchronized */ void MenuManager::saveStateAndCloseRecordStore()
{
    if (isRecordStoreOpened) {
        saveSettingsToBuffer();

        try {
            recordStore->closeRecordStore();
            isRecordStoreOpened = false;
        } catch (RecordStoreException& e) {
        }
    }

    currentGameMenu = nullptr;
}

void MenuManager::saveSettingsToBuffer()
{
    // Copy player name to buffer (indices 16-18)
    savePlayerNameToBuffer(16, playerName);

    // Save all settings to the 19-byte buffer
    setSavedStateValue(0, (int8_t)perspectiveSetting->getCurrentOptionPos());
    setSavedStateValue(1, (int8_t)shadowsSetting->getCurrentOptionPos());
    setSavedStateValue(2, (int8_t)driverSpriteSetting->getCurrentOptionPos());
    setSavedStateValue(3, (int8_t)bikeSpriteSetting->getCurrentOptionPos());
    setSavedStateValue(14, (int8_t)inputSetting->getCurrentOptionPos());
    setSavedStateValue(4, (int8_t)lookAheadSetting->getCurrentOptionPos());
    setSavedStateValue(5, (int8_t)leagueSetting->getMaxAvailableOptionPos());
    setSavedStateValue(6, (int8_t)levelSetting->getMaxAvailableOptionPos());
    setSavedStateValue(10, (int8_t)levelSetting->getCurrentOptionPos());
    setSavedStateValue(11, (int8_t)trackSetting->getCurrentOptionPos());
    setSavedStateValue(12, (int8_t)leagueSetting->getCurrentOptionPos());

    for (int i = 0; i < 3; ++i) {
        setSavedStateValue(7 + i, maxUnlockedTracksPerLevel[i]);
    }

    if (savedStateRecordId == -1) {
        try {
            savedStateRecordId = recordStore->addRecord(savedStateBuffer, 0, 19);
        } catch (RecordStoreNotOpenException& e) {
        } catch (RecordStoreException& e) {
        }
    } else {
        try {
            recordStore->setRecord(savedStateRecordId, savedStateBuffer, 0, 19);
        } catch (RecordStoreNotOpenException& e) {
        } catch (RecordStoreException& e) {
        }
    }
}

void MenuManager::runAlertThread()
{
    // TODO: Thread entry point for alert display
    // if (alert != nullptr) {
    //     Display.getDisplay(micro).setCurrent(alert);
    // }
}

void MenuManager::showAlert(const std::string& title, const std::string& message, Image* image)
{
    (void)title;
    (void)message;
    (void)image;
    // TODO: Display alert dialog
    // alert = new Alert(title, message, image, AlertType.INFO);
    // alert.setTimeout(4000);
    // (new Thread(this)).start();
}

void MenuManager::handleMenuSelection(IGameMenuElement* element)
{
    if (element == startTask) {
        if (levelSetting->getCurrentOptionPos() <= levelSetting->getMaxAvailableOptionPos() && trackSetting->getCurrentOptionPos() <= trackSetting->getMaxAvailableOptionPos() && leagueSetting->getCurrentOptionPos() <= leagueSetting->getMaxAvailableOptionPos()) {
            micro->gamePhysics->disableGenerateInputAI();
            micro->levelLoader->loadTrack(levelSetting->getCurrentOptionPos(), trackSetting->getCurrentOptionPos());
            micro->gamePhysics->setMotoLeague(leagueSetting->getCurrentOptionPos());
            shouldStartRaceImmediately = true;
            micro->menuToGame();
        } else {
            showAlert("GDTR", "Complete more tracks to unlock this track/league combo.", nullptr);
        }
    } else if (element == perspectiveSetting) {
        micro->gamePhysics->invertYPositions(perspectiveSetting->getCurrentOptionPos() == 0);
        LevelLoader::isEnabledPerspective = perspectiveSetting->getCurrentOptionPos() == 0;
    } else if (element == shadowsSetting) {
        LevelLoader::isEnabledShadows = shadowsSetting->getCurrentOptionPos() == 0;
    } else {
        if (element == driverSpriteSetting) {
            if (driverSpriteSetting->checkAndClearSelectionFlag()) {
                driverSpriteSetting->setCurentOptionPos(driverSpriteSetting->getCurrentOptionPos() + 1);
                return;
            }
        } else if (element == bikeSpriteSetting) {
            if (bikeSpriteSetting->checkAndClearSelectionFlag()) {
                bikeSpriteSetting->setCurentOptionPos(bikeSpriteSetting->getCurrentOptionPos() + 1);
                return;
            }
        } else {
            if (element == inputSetting) {
                if (inputSetting->checkAndClearSelectionFlag()) {
                    inputSetting->setCurentOptionPos(inputSetting->getCurrentOptionPos() + 1);
                }

                micro->gameCanvas->setInputConfigIndex(inputSetting->getCurrentOptionPos());
                return;
            }

            if (element == lookAheadSetting) {
                micro->gamePhysics->setEnableLookAhead(lookAheadSetting->getCurrentOptionPos() == 0);
                return;
            }

            if (element == confirmYesSetting) {
                if (currentGameMenu == confirmClearHighscoresMenu) {
                    recordManager->deleteRecordStores();
                    showAlert("Cleared", "Highscores have been cleared", nullptr);
                } else if (currentGameMenu == confirmFullResetMenu) {
                    performFullReset();
                    showAlert("Reset", "Master reset. Application will be closed.", nullptr);
                }

                switchToMenu(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (element == confirmNoSetting) {
                switchToMenu(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (element == backSetting) {
                switchToMenu(currentGameMenu->getGameMenu(), true);
                return;
            }

            if (element == playMenuSetting) {
                levelSetting->setCurentOptionPos(savedLevelBeforeMenu);
                trackSetting->setAvailableOptions(maxUnlockedTracksPerLevel[savedLevelBeforeMenu]);
                trackSetting->setCurentOptionPos(savedTrackBeforeMenu);
                switchToMenu(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (element == goToMainSetting) {
                switchToMenu(mainMenu, false);
                return;
            }

            if (element == exitGameSetting) {
                switchToMenu(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (element == restartTrackSetting) {
                if (leagueSetting->getCurrentOptionPos() <= leagueSetting->getMaxAvailableOptionPos()) {
                    levelSetting->setCurentOptionPos(savedLevelBeforeMenu);
                    trackSetting->setAvailableOptions(maxUnlockedTracksPerLevel[savedLevelBeforeMenu]);
                    trackSetting->setCurentOptionPos(savedTrackBeforeMenu);
                    micro->gamePhysics->setMotoLeague(leagueSetting->getCurrentOptionPos());
                    shouldStartRaceImmediately = true;
                    micro->menuToGame();
                    return;
                }
            } else {
                if (element == nextTrackSetting) {
                    if (!isAllTracksCompletedAtLevel) {
                        trackSetting->menuElemMethod(2);
                    }

                    micro->levelLoader->loadTrack(levelSetting->getCurrentOptionPos(), trackSetting->getCurrentOptionPos());
                    micro->gamePhysics->setMotoLeague(leagueSetting->getCurrentOptionPos());
                    saveSettingsToBuffer();
                    shouldStartRaceImmediately = true;
                    micro->menuToGame();
                    return;
                }

                if (element == continueSetting) {
                    requestRepaint();
                    micro->menuToGame();
                    return;
                }

                if (element == nameEntrySetting) {
                    enterNameMenu->resetToFirstItem();
                    switchToMenu(enterNameMenu, false);
                    return;
                }

                if (element == okSetting) {
                    saveHighscoreAndShowResults();
                    return;
                }

                if (element == trackSetting) {
                    if (trackSetting->checkAndClearSelectionFlag()) {
                        trackSetting->setAvailableOptions(maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()]);
                        trackSetting->init();
                        trackSelectionParentMenu = trackSetting->getParentGameMenu();
                        switchToMenu(trackSelectionParentMenu, false);
                        trackSelectionParentMenu->navigateToItem(trackSetting->getCurrentOptionPos());
                    }

                    lastSelectedTrackPerLevel[levelSetting->getCurrentOptionPos()] = trackSetting->getCurrentOptionPos();
                    return;
                }

                if (element == levelSetting) {
                    if (levelSetting->checkAndClearSelectionFlag()) {
                        levelSelectionMenu = levelSetting->getParentGameMenu();
                        switchToMenu(levelSelectionMenu, false);
                        levelSelectionMenu->navigateToItem(levelSetting->getCurrentOptionPos());
                    }

                    trackSetting->setOptionsList(micro->levelLoader->trackNames[levelSetting->getCurrentOptionPos()]);
                    trackSetting->setAvailableOptions(maxUnlockedTracksPerLevel[levelSetting->getCurrentOptionPos()]);
                    trackSetting->setCurentOptionPos(lastSelectedTrackPerLevel[levelSetting->getCurrentOptionPos()]);
                    trackSetting->init();
                    return;
                }

                if (element == leagueSetting && leagueSetting->checkAndClearSelectionFlag()) {
                    leagueSelectionMenu = leagueSetting->getParentGameMenu();
                    leagueSetting->setParentGameMenu(currentGameMenu);
                    switchToMenu(leagueSelectionMenu, false);
                    leagueSelectionMenu->navigateToItem(leagueSetting->getCurrentOptionPos());
                }
            }
        }
    }
}

int MenuManager::getGraphicsFlags()
{
    // Returns bitfield: bit 0 = bike sprite enabled, bit 1 = driver sprite enabled
    int flags = 0;
    if (driverSpriteSetting->getCurrentOptionPos() == 0) {
        flags |= 2;
    }

    if (bikeSpriteSetting->getCurrentOptionPos() == 0) {
        flags |= 1;
    }

    return flags;
}

void MenuManager::applyGraphicsFlags(int flags)
{
    bikeSpriteSetting->setCurentOptionPos(1);
    driverSpriteSetting->setCurentOptionPos(1);
    if ((flags & 1) > 0) {
        bikeSpriteSetting->setCurentOptionPos(0);
    }

    if ((flags & 2) > 0) {
        driverSpriteSetting->setCurentOptionPos(0);
    }
}

int MenuManager::getSelectedLevel()
{
    return levelSetting->getCurrentOptionPos();
}

int MenuManager::getSelectedTrack()
{
    return trackSetting->getCurrentOptionPos();
}

int MenuManager::getSelectedLeague()
{
    return leagueSetting->getCurrentOptionPos();
}

void MenuManager::setFinishTime(int64_t timeMs)
{
    finishTime = timeMs;
}

std::vector<int8_t> MenuManager::loadPlayerName(int index, int8_t defaultValue)
{
    // Load 3 bytes from saved state buffer starting at index
    switch (index) {
    case 16: {
        std::vector<int8_t> result = std::vector<int8_t>(3);

        for (int i = 0; i < 3; ++i) {
            result[i] = savedStateBuffer[16 + i];
        }

        if (result[0] == -127) {
            result[0] = defaultValue;
        }
        return result;
    }
    default:
        return std::vector<int8_t>();
    }
}

int8_t MenuManager::loadSavedStateValue(int index, int8_t defaultValue)
{
    // Load single byte from saved state, return default if uninitialized (-127)
    return savedStateBuffer[index] == -127 ? defaultValue : savedStateBuffer[index];
}

void MenuManager::savePlayerNameToBuffer(int index, char* name)
{
    // Copy 3-character player name to buffer at specified index
    if (isRecordStoreOpened && index == 16) {
        for (int i = 0; i < 3; ++i) {
            savedStateBuffer[16 + i] = name[i];
        }
    }
}

std::string MenuManager::formatTime(int64_t timeMs)
{
    // Format milliseconds to MM:SS.cc string
    finishTimeSeconds = (int)(timeMs / 100L);
    finishTimeCentiseconds = (int)(timeMs % 100L);
    std::string formattedTime;
    if (finishTimeSeconds / 60 < 10) {
        formattedTime = " 0" + std::to_string(finishTimeSeconds / 60);
    } else {
        formattedTime = " " + std::to_string(finishTimeSeconds / 60);
    }

    if (finishTimeSeconds % 60 < 10) {
        formattedTime = formattedTime + ":0" + std::to_string(finishTimeSeconds % 60);
    } else {
        formattedTime = formattedTime + ":" + std::to_string(finishTimeSeconds % 60);
    }

    if (finishTimeCentiseconds < 10) {
        formattedTime = formattedTime + ".0" + std::to_string(finishTimeCentiseconds);
    } else {
        formattedTime = formattedTime + "." + std::to_string(finishTimeCentiseconds);
    }

    return formattedTime;
}

void MenuManager::setSavedStateValue(int index, int8_t value)
{
    if (isRecordStoreOpened) {
        savedStateBuffer[index] = value;
    }
}

void MenuManager::performFullReset()
{
    // Reset all settings to default values
    perspectiveSetting->setCurentOptionPos(0);
    shadowsSetting->setCurentOptionPos(0);
    driverSpriteSetting->setCurentOptionPos(0);
    bikeSpriteSetting->setCurentOptionPos(0);
    lookAheadSetting->setCurentOptionPos(0);
    leagueSetting->setCurentOptionPos(0);
    leagueSetting->setAvailableOptions(0);
    levelSetting->setCurentOptionPos(0);
    levelSetting->setAvailableOptions(1);
    trackSetting->setCurentOptionPos(0);

    playerName[0] = 65; // 'A'
    playerName[1] = 65;
    playerName[2] = 65;
    inputSetting->setCurentOptionPos(0);
    maxUnlockedTracksPerLevel[0] = 0;
    maxUnlockedTracksPerLevel[1] = 0;
    maxUnlockedTracksPerLevel[2] = -1;
    maxAvailableLeagues = 0;
    saveSettingsToBuffer();
    recordManager->deleteRecordStores();
}

void MenuManager::removeCommands()
{
    micro->gameCanvas->removeCommand(okCommand);
    micro->gameCanvas->removeCommand(backCommand);
}

void MenuManager::addCommands()
{
    if (currentGameMenu != mainMenu && currentGameMenu != finishedTrackMenu && currentGameMenu != nullptr) {
        micro->gameCanvas->addCommand(backCommand);
    }

    micro->gameCanvas->addCommand(okCommand);
}

int MenuManager::countRecordStoresForLevel(int levelIndex)
{
    std::vector<std::string> storeNames = RecordStore::listRecordStores();
    if (recordManager != nullptr && !storeNames.empty()) {
        int count = 0;

        for (std::size_t i = 0; i < storeNames.size(); ++i) {
            if (storeNames[i].find(std::to_string(levelIndex), 0) == 0) {
                ++count;
            }
        }

        return count;
    } else {
        return 0;
    }
}
