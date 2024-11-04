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
#include "config.h"

MenuManager::MenuManager(Micro* var1)
{
    micro = var1;
    field_376 = std::make_unique<TextRender>("", var1);
}

void MenuManager::initPart(int var1)
{
    int var4;
    switch (var1) {
    case 1:
        playerName = defaultName;
        field_374 = { "On", "Off" };
        field_375 = { "Keyset 1", "Keyset 2", "Keyset 3" };
        recordManager = new RecordManager();
        trackTimeMs = -1L;
        trackTimeFormatted.clear();
        isRecordStoreOpened = false;
        savedData = std::vector<int8_t>(19);

        for (int idx = 0; idx < 19; ++idx) {
            savedData[idx] = -127;
        }

        try {
            recordStore = RecordStore::openRecordStore(GlobalSetting::GlobalSaveFileName, true);
            isRecordStoreOpened = true;
            return;
        } catch (RecordStoreException& var9) {
            isRecordStoreOpened = false;
            return;
        }
    case 2: {
        recorcStoreRecordId = -1;

        RecordEnumeration* records;
        try {
            records = recordStore->enumerateRecords(nullptr, nullptr, false);
        } catch (RecordStoreNotOpenException& var8) {
            return;
        }

        std::vector<int8_t> var3;
        if (records->numRecords() > 0) {
            try {
                var3 = records->nextRecord();
                records->reset();
                recorcStoreRecordId = records->nextRecordId();
            } catch (RecordStoreException& var7) {
                return;
            }

            if (var3.size() <= 19) {
                for (std::size_t i = 0; i < var3.size(); ++i) {
                    savedData[i] = var3[i];
                }
            }

            // records->destroy();
        }

        var3 = method_216(16, (int8_t)-1);
        if (!var3.empty() && var3[0] != -1) {
            for (var4 = 0; var4 < 3; ++var4) {
                playerName[var4] = var3[var4];
            }
        }

        // cheat code: set name to RKE to enable all leagues ???
        if (playerName[0] == 82 && playerName[1] == 75 && playerName[2] == 69) {
            availableLeagues = 3;
            availableLevels = 2;
            availableTracks[0] = (int8_t)(micro->levelLoader->levelNames[0].size() - 1);
            availableTracks[1] = (int8_t)(micro->levelLoader->levelNames[1].size() - 1);
            availableTracks[2] = (int8_t)(micro->levelLoader->levelNames[2].size() - 1);
            return;
        }

        availableLeagues = 0;
        availableLevels = 0;
        availableTracks[0] = 0;
        availableTracks[1] = 0;
        availableTracks[2] = -1;
    }
        return;
    case 3:
        isDisablePerspective = getSettingOrDefault(0, isDisablePerspective);
        isDisabledShadows = getSettingOrDefault(1, isDisabledShadows);
        isDisabledDriverSprite = getSettingOrDefault(2, isDisabledDriverSprite);
        isDisabledBikeSprite = getSettingOrDefault(3, isDisabledBikeSprite);
        field_367 = getSettingOrDefault(14, field_367);
        isDisableLookAhead = getSettingOrDefault(4, isDisableLookAhead);
        field_369 = getSettingOrDefault(11, field_369);
        field_370 = getSettingOrDefault(10, field_370);
        field_371 = getSettingOrDefault(12, field_371);
        field_373 = getSettingOrDefault(15, field_373);
        field_354 = field_370;
        field_355 = field_369;

        if (playerName[0] != 82 || playerName[1] != 75 || playerName[2] != 69) {
            availableLeagues = getSettingOrDefault(5, availableLeagues);
            availableLevels = getSettingOrDefault(6, availableLevels);

            for (var4 = 0; var4 < 3; ++var4) {
                availableTracks[var4] = getSettingOrDefault(7 + var4, availableTracks[var4]);
            }
        }

        try {
            field_345.at(field_370) = field_369;
        } catch (std::exception& var6) {
            field_370 = 0;
            field_369 = 0;
            field_345[field_370] = field_369;
        }

        LevelLoader::isEnabledPerspective = isDisablePerspective == 0;
        LevelLoader::isEnabledShadows = isDisabledShadows == 0;
        micro->gamePhysics->setEnableLookAhead(isDisableLookAhead == 0);
        micro->gameCanvas->method_163(field_367);
        micro->gameCanvas->method_124(field_372 == 0);
        leagueNamesAll4 = { "100cc", "175cc", "220cc", "325cc" };
        levelNames = micro->levelLoader->levelNames;
        if (availableLeagues < 3) {
            this->leagueNames = { "100cc", "175cc", "220cc" };
        } else {
            this->leagueNames = leagueNamesAll4;
        }

        field_360 = field_371;
        return;
    case 4: {
        gameMenuMain = new GameMenu("Main", micro, nullptr);
        gameMenuPlay = new GameMenu("Play", micro, gameMenuMain);
        gameMenuOptions = new GameMenu("Options", micro, gameMenuMain);
        gameMenuAbout = new GameMenu("About", micro, gameMenuMain);
        gameMenuHelp = new GameMenu("Help", micro, gameMenuMain);
        settingStringBack = new SettingsStringRender("Back", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        settingStringGoToMain = new SettingsStringRender("Go to Main", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        settingStringContinue = new SettingsStringRender("Continue", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        settingStringPlayMenu = new SettingsStringRender("Play Menu", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);

        std::shared_ptr boldSmallFont = FontStorage::getFont(Font::STYLE_BOLD, Font::SIZE_SMALL);
        if (gameMenuAbout->xPos + boldSmallFont->stringWidth("http://www.codebrew.se/") >= getCanvasWidth()) {
            textRenderCodeBrewLink = new TextRender("www.codebrew.se", micro);
        } else {
            textRenderCodeBrewLink = new TextRender("http://www.codebrew.se/", micro);
        }

        textRenderCodeBrewLink->setFont(boldSmallFont);
        gameMenuHighscore = new GameMenu("Highscore", micro, gameMenuPlay);
        gameMenuFinished = new GameMenu("Finished!", micro, gameMenuPlay);
    }
        return;
    case 5:
        gameMenuIngame = new GameMenu("Ingame", micro, gameMenuPlay);
        gameMenuEnterName = new GameMenu("Enter Name", micro, gameMenuFinished, playerName);
        gameMenuConfirmClear = new GameMenu("Confirm Clear", micro, gameMenuOptions);
        gameMenuConfirmReset = new GameMenu("Confirm Reset", micro, gameMenuConfirmClear);
        taskPlayMenu = new TimerOrMotoPartOrMenuElem("Play Menu", gameMenuPlay, this);
        taskOptions = new TimerOrMotoPartOrMenuElem("Options", gameMenuOptions, this);
        taskHelp = new TimerOrMotoPartOrMenuElem("Help", gameMenuHelp, this);
        taskAbout = new TimerOrMotoPartOrMenuElem("About", gameMenuAbout, this);
        settingStringExitGame = new SettingsStringRender("Exit Game", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        gameMenuMain->addMenuElement(taskPlayMenu);
        gameMenuMain->addMenuElement(taskOptions);
        gameMenuMain->addMenuElement(taskHelp);
        gameMenuMain->addMenuElement(taskAbout);
        gameMenuMain->addMenuElement(settingStringExitGame);
        settingStringLevel = new SettingsStringRender("Level", field_370, this, field_361, false, micro, gameMenuPlay, false);
        settingsStringTrack = new SettingsStringRender("Track", field_345[field_370], this, levelNames[field_370], false, micro, gameMenuPlay, false);
        settingsStringLeague = new SettingsStringRender("League", field_371, this, leagueNames, false, micro, gameMenuPlay, false);

        try {
            settingsStringTrack->setAvailableOptions(availableTracks[field_370]);
        } catch (std::exception& var5) {
            settingsStringTrack->setAvailableOptions(0);
        }

        settingStringLevel->setAvailableOptions(availableLevels);
        settingsStringLeague->setAvailableOptions(availableLeagues);
        gameTimerTaskHighscore = new TimerOrMotoPartOrMenuElem("Highscore", gameMenuHighscore, this);
        gameMenuHighscore->addMenuElement(settingStringBack);
        taskStart = new SettingsStringRender("Start>", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        gameMenuPlay->addMenuElement(taskStart);
        gameMenuPlay->addMenuElement(settingStringLevel);
        gameMenuPlay->addMenuElement(settingsStringTrack);
        gameMenuPlay->addMenuElement(settingsStringLeague);
        gameMenuPlay->addMenuElement(gameTimerTaskHighscore);
        gameMenuPlay->addMenuElement(settingStringGoToMain);

        perspectiveSetting = new SettingsStringRender("Perspective", isDisablePerspective, this, field_374, true, micro, gameMenuOptions, false);
        shadowsSetting = new SettingsStringRender("Shadows", isDisabledShadows, this, field_374, true, micro, gameMenuOptions, false);
        driverSpriteSetting = new SettingsStringRender("Driver sprite", isDisabledDriverSprite, this, field_374, true, micro, gameMenuOptions, false);
        bikeSpriteSetting = new SettingsStringRender("Bike sprite", isDisabledBikeSprite, this, field_374, true, micro, gameMenuOptions, false);
        inputSetting = new SettingsStringRender("Input", field_367, this, field_375, false, micro, gameMenuOptions, false);
        lookAheadSetting = new SettingsStringRender("Look ahead", isDisableLookAhead, this, field_374, true, micro, gameMenuOptions, false);
        clearHighscoreSetting = new TimerOrMotoPartOrMenuElem("Clear highscore", gameMenuConfirmClear, this);
        return;
    case 6:
        gameMenuOptions->addMenuElement(perspectiveSetting);
        gameMenuOptions->addMenuElement(shadowsSetting);
        gameMenuOptions->addMenuElement(driverSpriteSetting);
        gameMenuOptions->addMenuElement(bikeSpriteSetting);
        gameMenuOptions->addMenuElement(inputSetting);
        gameMenuOptions->addMenuElement(lookAheadSetting);
        gameMenuOptions->addMenuElement(clearHighscoreSetting);
        gameMenuOptions->addMenuElement(settingStringBack);
        field_315 = new SettingsStringRender("No", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        field_314 = new SettingsStringRender("Yes", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        field_313 = new TimerOrMotoPartOrMenuElem("Full Reset", gameMenuConfirmReset, this);
        addTextRender(gameMenuConfirmClear, "Clearing the highscores can not be undone. It will remove all the registered times on all tracks.");
        addTextRender(gameMenuConfirmClear, "Would you like to clear the highscores?");
        gameMenuConfirmClear->addMenuElement(field_315);
        gameMenuConfirmClear->addMenuElement(field_314);
        gameMenuConfirmClear->addMenuElement(field_313);
        addTextRender(gameMenuConfirmReset, "A full reset can not be undone. It will relock all tracks and leagues and clear back all settings to default. A full reset will exit the application.");
        addTextRender(gameMenuConfirmReset, "Would you like to do a full reset?");
        gameMenuConfirmReset->addMenuElement(field_315);
        gameMenuConfirmReset->addMenuElement(field_314);
        field_317 = new GameMenu("Objective", micro, gameMenuHelp);
        field_318 = new TimerOrMotoPartOrMenuElem("Objective", field_317, this);
        addTextRender(field_317, "Race to the finish line as fast as you can without crashing. By leaning forward and backward you can adjust the rotation of your bike. By landing on both wheels after jumping, your bike won't crash as easily. Beware, the levels tend to get harder and harder...");
        field_317->addMenuElement(settingStringBack);
        gameMenuHelp->addMenuElement(field_318);
        field_319 = new GameMenu("Keys", micro, gameMenuHelp);
        field_320 = new TimerOrMotoPartOrMenuElem("Keys", field_319, this);
        addTextRender(field_319, "- " + field_375[0] + " -");
        addTextRender(field_319, "UP accelerates, DOWN brakes, RIGHT leans forward and LEFT leans backward. 1 accelerates and leans backward. 3 accelerates and leans forward. 7 brakes and leans backward. 9 brakes and leans forward.");
        field_319->addMenuElement(field_376.get());
        addTextRender(field_319, "- " + field_375[1] + " -");
        addTextRender(field_319, "1 accelerates, 4 brakes, 6 leans forward and 5 leans backward.");
        field_319->addMenuElement(field_376.get());
        addTextRender(field_319, "- " + field_375[2] + " -");
        addTextRender(field_319, "3 accelerates, 6 brakes, 5 leans forward and 4 leans backward.");
        field_319->addMenuElement(settingStringBack);
        gameMenuHelp->addMenuElement(field_320);
        field_321 = new GameMenu("Unlocking", micro, gameMenuHelp);
        field_322 = new TimerOrMotoPartOrMenuElem("Unlocking", field_321, this);
        addTextRender(field_321, "By completing the easier levels, new levels will be unlocked. You will also gain access to higher leagues where more advanced bikes with different characteristics are available.");
        field_321->addMenuElement(settingStringBack);
        gameMenuHelp->addMenuElement(field_322);
        gameMenuOptionsHighscoreDescription = new GameMenu("Highscore", micro, gameMenuHelp);
        taskHighscore = new TimerOrMotoPartOrMenuElem("Highscore", gameMenuOptionsHighscoreDescription, this);
        addTextRender(gameMenuOptionsHighscoreDescription, "The three best times on every track are saved for each league. When beating a time on a track you will be asked to enter your name. The highscores can be viewed from the Play Menu. By pressing left and right in the highscore view you can view the highscore for a specific league. The highscore can be cleared from the options menu.");
        gameMenuOptionsHighscoreDescription->addMenuElement(settingStringBack);
        gameMenuHelp->addMenuElement(taskHighscore);
        return;
    case 7:
        gameMenuOptions2 = new GameMenu("Options", micro, gameMenuHelp);
        field_326 = new TimerOrMotoPartOrMenuElem("Options", gameMenuOptions2, this);

        addTextRender(gameMenuOptions2, "Perspective: On/Off");
        addTextRender(gameMenuOptions2, "Default: <On>. Turns on and off the perspective view of the tracks.");
        gameMenuOptions2->addMenuElement(field_376.get());
        addTextRender(gameMenuOptions2, "Shadows: On/Off");
        addTextRender(gameMenuOptions2, "Default: <On>. Turns on and off the shadows.");
        gameMenuOptions2->addMenuElement(field_376.get());
        addTextRender(gameMenuOptions2, "Driver Sprite: On / Off");
        addTextRender(gameMenuOptions2, "Default: <On>. <On> uses a texture for the driver. <Off> uses line graphics.");
        gameMenuOptions2->addMenuElement(field_376.get());
        addTextRender(gameMenuOptions2, "Bike Sprite: On / Off");
        addTextRender(gameMenuOptions2, "Default: <On>. <On> uses a texture for the bike. <Off> uses line graphics.");
        gameMenuOptions2->addMenuElement(field_376.get());
        addTextRender(gameMenuOptions2, "Input: Keyset 1,2,3 ");
        addTextRender(gameMenuOptions2, "Default: <1>. Determines which type of input should be used when playing. See \"Keys\" in the help menu for more info.");
        gameMenuOptions2->addMenuElement(field_376.get());
        addTextRender(gameMenuOptions2, "Look ahead: On/Off");
        addTextRender(gameMenuOptions2, "Default: <On>. Turns on and off smart camera movement.");
        gameMenuOptions2->addMenuElement(field_376.get());
        addTextRender(gameMenuOptions2, "Clear highscore");
        addTextRender(gameMenuOptions2, "Lets you clear the highscores. Here you can also do a \"Full Reset\" which will reset the game to original state (clear settings, highscores, unlocked levels and leagues).");
        gameMenuOptions2->addMenuElement(field_376.get());
        gameMenuOptions2->addMenuElement(settingStringBack);
        gameMenuHelp->addMenuElement(field_326);
        gameMenuHelp->addMenuElement(settingStringBack);
        addTextRender(gameMenuAbout, "\"Gravity Defied - Trial Racing\" v1.0 by Codebrew Software © 2004.");
        addTextRender(gameMenuAbout, "brought 2 you by pascha.                For information visit:");
        gameMenuAbout->addMenuElement(textRenderCodeBrewLink);
        gameMenuAbout->addMenuElement(settingStringBack);
        field_334 = new SettingsStringRender("Track: " + micro->levelLoader->getName(0, 1), 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        field_333 = new SettingsStringRender("Restart: " + micro->levelLoader->getName(0, 0), 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        gameMenuIngame->addMenuElement(settingStringContinue);
        gameMenuIngame->addMenuElement(field_333);
        gameMenuIngame->addMenuElement(taskOptions);
        gameMenuIngame->addMenuElement(taskHelp);
        gameMenuIngame->addMenuElement(settingStringPlayMenu);
        field_335 = new SettingsStringRender("Ok", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        field_336 = new SettingsStringRender("Name - " + std::string(playerName), 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        commandOk = new Command("Ok", 4, 1);
        commandBack = new Command("Back", 2, 1);
        method_1(gameMenuMain, false);

        rasterImage = std::make_unique<Image>("raster.png");

    default:
        break;
    }
}

void MenuManager::addTextRender(GameMenu* gameMenu, std::string text)
{
    std::vector<TextRender*> var3 = TextRender::makeMultilineTextRenders(text, micro);

    for (std::size_t var4 = 0; var4 < var3.size(); ++var4) {
        gameMenu->addMenuElement(var3[var4]);
    }
}

int MenuManager::getCurrentLevel()
{
    return settingStringLevel->getCurrentOptionPos();
}

int MenuManager::getCurrentTrack()
{
    return settingsStringTrack->getCurrentOptionPos();
}

bool MenuManager::method_196()
{
    if (field_357) {
        field_357 = false;
        return true;
    } else {
        return false;
    }
}

void MenuManager::method_197()
{
    std::cout << "method_197 " << playerName << " " << trackTimeMs << std::endl;
    recordManager->method_17(settingsStringLeague->getCurrentOptionPos(), playerName, trackTimeMs);
    recordManager->writeRecordInfo();
    field_356 = false;
    gameMenuFinished->clearVector();
    gameMenuFinished->addMenuElement(new TextRender("Time: " + trackTimeFormatted, micro));
    std::vector<std::string> var1 = recordManager->getRecordDescription(settingsStringLeague->getCurrentOptionPos());

    for (std::size_t var2 = 0; var2 < var1.size(); ++var2) {
        if (var1[var2] != "") {
            gameMenuFinished->addMenuElement(new TextRender(std::to_string(var2 + 1) + "." + var1[var2], micro));
        }
    }

    recordManager->closeRecordStore();
    int8_t availableLeagues = -1;
    if (settingsStringTrack->getMaxAvailableOptionPos() >= settingsStringTrack->getCurrentOptionPos()) {
        settingsStringTrack->setAvailableOptions(settingsStringTrack->getCurrentOptionPos() + 1 < availableTracks[settingStringLevel->getCurrentOptionPos()] ? availableTracks[settingStringLevel->getCurrentOptionPos()] : settingsStringTrack->getCurrentOptionPos() + 1);
        availableTracks[settingStringLevel->getCurrentOptionPos()] = (int8_t)settingsStringTrack->getMaxAvailableOptionPos() < availableTracks[settingStringLevel->getCurrentOptionPos()] ? availableTracks[settingStringLevel->getCurrentOptionPos()] : (int8_t)settingsStringTrack->getMaxAvailableOptionPos();
    }

    if (settingsStringTrack->getCurrentOptionPos() == settingsStringTrack->getMaxOptionPos()) {
        field_356 = true;
        switch (settingStringLevel->getCurrentOptionPos()) {
        case 0:
            if (availableLeagues < 1) {
                availableLeagues = 1;
                settingsStringLeague->setAvailableOptions(availableLeagues);
            }
            break;
        case 1:
            if (availableLeagues < 2) {
                availableLeagues = 2;
                settingsStringLeague->setAvailableOptions(availableLeagues);
            }
            break;
        case 2:
            if (availableLeagues < 3) {
                availableLeagues = 3;
                settingsStringLeague->setOptionsList(leagueNamesAll4);
                leagueNames = leagueNamesAll4;
                settingsStringLeague->setAvailableOptions(availableLeagues);
            }
        }

        settingStringLevel->setAvailableOptions(settingStringLevel->getMaxAvailableOptionPos() + 1);
        if (availableTracks[settingStringLevel->getMaxAvailableOptionPos()] == -1) {
            availableTracks[settingStringLevel->getMaxAvailableOptionPos()] = 0;
        }
    }

    int var3 = getCountOfRecordStoresWithPrefix(settingStringLevel->getCurrentOptionPos());
    addTextRender(gameMenuFinished, var3 + " of " + std::to_string(levelNames[settingStringLevel->getCurrentOptionPos()].size()) + " tracks in " + field_361[settingStringLevel->getCurrentOptionPos()] + " completed.");
    if (!field_356) {
        field_333->setText("Restart: " + micro->levelLoader->getName(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos()));
        field_334->setText("Next: " + micro->levelLoader->getName(field_354, field_355 + 1));
    } else {
        if (settingStringLevel->getCurrentOptionPos() < settingStringLevel->getMaxOptionPos()) {
            settingStringLevel->setCurentOptionPos(settingStringLevel->getCurrentOptionPos() + 1);
            settingsStringTrack->setCurentOptionPos(0);
            settingsStringTrack->setAvailableOptions(availableTracks[settingStringLevel->getCurrentOptionPos()]);
        }

        if (availableLeagues != -1) {
            addTextRender(gameMenuFinished, "Congratultions! You have successfully unlocked a new league: " + leagueNames[availableLeagues]);
            if (availableLeagues == 3) {
                gameMenuFinished->addMenuElement(new TextRender("Enjoy...", micro));
            }

            showAlert("League unlocked", "You have successfully unlocked a new league: " + leagueNames[availableLeagues], nullptr);
        } else {
            bool var4 = true;

            for (int var5 = 0; var5 < 3; ++var5) {
                if (availableTracks[var5] != static_cast<int>(micro->levelLoader->levelNames[var5].size() - 1)) {
                    var4 = false;
                }
            }

            if (!var4) {
                addTextRender(gameMenuFinished, "You have completed all tracks at this level.");
            }
        }
    }

    if (!field_356) {
        gameMenuFinished->addMenuElement(field_334);
    }

    field_333->setText("Restart: " + micro->levelLoader->getName(field_354, field_355));
    gameMenuFinished->addMenuElement(field_333);
    gameMenuFinished->addMenuElement(settingStringPlayMenu);
    method_1(gameMenuFinished, false);
}

void MenuManager::repaint()
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

void MenuManager::method_201(int var1)
{
    field_377 = false;
    switch (var1) {
    case 0:
        method_1(gameMenuMain, false);
        micro->gamePhysics->enableGenerateInputAI();
        field_357 = true;
        break;
    case 1:
        field_354 = settingStringLevel->getCurrentOptionPos();
        field_355 = settingsStringTrack->getCurrentOptionPos();
        field_333->setText("Restart: " + micro->levelLoader->getName(field_354, field_355));
        field_357 = false;
        method_1(gameMenuIngame, false);
        break;
    case 2: {
        field_362 = Time::currentTimeMillis();
        gameMenuFinished->clearVector();
        field_354 = settingStringLevel->getCurrentOptionPos();
        field_355 = settingsStringTrack->getCurrentOptionPos();
        recordManager->method_8(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
        const uint8_t var2 = recordManager->getPosOfNewRecord(settingsStringLeague->getCurrentOptionPos(), trackTimeMs);
        trackTimeFormatted = Time::timeToString(trackTimeMs);

        if (var2 >= 0 && var2 <= 2) {
            TextRender* var3 = new TextRender("", micro);
            var3->setDx(GameCanvas::spriteSizeX[5] + 1);
            switch (var2) {
            case 0:
                var3->setText("First place!");
                var3->setDrawSprite(true, 5);
                break;
            case 1:
                var3->setText("Second place!");
                var3->setDrawSprite(true, 6);
                break;
            case 2:
                var3->setText("Third place!");
                var3->setDrawSprite(true, 7);
            }

            gameMenuFinished->addMenuElement(var3);
            TextRender* var4 = new TextRender(trackTimeFormatted, micro);
            var4->setDx(GameCanvas::spriteSizeX[5] + 1);
            gameMenuFinished->addMenuElement(var4);
            gameMenuFinished->addMenuElement(field_335);
            gameMenuFinished->addMenuElement(field_336);
            method_1(gameMenuFinished, false);
            field_377 = false;
        } else {
            method_197();
        }
    } break;
    default:
        method_1(gameMenuMain, false);
        break;
    }

    int64_t currentTimeMillis = Time::currentTimeMillis();
    micro->gameCanvas->isDrawingTime = false;
    int64_t var6 = 0L;
    int8_t var8 = 50;
    micro->gamePhysics->method_53();
    micro->gameToMenu();

    while (Micro::isInGameMenu && Micro::gameStarted && currentGameMenu != nullptr) {
        int64_t var20;
        if (micro->gamePhysics->isGenerateInputAI) {
            int var9;
            if ((var9 = micro->gamePhysics->updatePhysics()) != 0 && var9 != 4) {
                micro->gamePhysics->resetSmth(true);
            }

            micro->gamePhysics->method_53();
            repaint();
            if ((var20 = Time::currentTimeMillis()) - var6 < (int64_t)var8) {
                // try {
                //     synchronized (field_359) {
                //         field_359.wait((int64_t) var8 - (var20 - var6) < 1L ? 1L : (int64_t) var8 - (var20 - var6));
                //     }
                // } catch (InterruptedException var16) {
                // }
                Time::sleep((int64_t)var8 - (var20 - var6) < 1L ? 1L : (int64_t)var8 - (var20 - var6));

                var6 = Time::currentTimeMillis();
            } else {
                var6 = var20;
            }
        } else {
            var8 = 50;
            if ((var20 = Time::currentTimeMillis()) - var6 < (int64_t)var8) {
                // try {
                //     Object var21;
                //     synchronized (var21 = new Object()) {
                //         var21.wait((int64_t) var8 - (var20 - var6) < 1L ? 1L : (int64_t) var8 - (var20 - var6));
                //     }
                // } catch (InterruptedException var14) {
                // }
                Time::sleep((int64_t)var8 - (var20 - var6) < 1L ? 1L : (int64_t)var8 - (var20 - var6));

                var6 = Time::currentTimeMillis();
            } else {
                var6 = var20;
            }

            if (Micro::isInGameMenu) {
                repaint();
            }
        }
    }

    micro->timeMs += Time::currentTimeMillis() - currentTimeMillis;
    micro->gameCanvas->isDrawingTime = true;
    if (currentGameMenu == nullptr) {
        Micro::gameStarted = false;
    }
}

void MenuManager::method_202(Graphics* var1)
{
    if (currentGameMenu != nullptr && !field_377) {
        micro->gameCanvas->drawGame(var1);
        fillCanvasWithImage(var1);
        currentGameMenu->render_76(var1);
    }
}

void MenuManager::fillCanvasWithImage(Graphics* graphics)
{
    for (int y = 0; y < getCanvasHeight(); y += rasterImage->getHeight()) {
        for (int x = 0; x < getCanvasWidth(); x += rasterImage->getWidth()) {
            graphics->drawImage(rasterImage.get(), x, y, 20);
        }
    }
}

void MenuManager::processNonFireKeyCode(int keyCode)
{
    if (micro->gameCanvas->getGameAction(keyCode) != 8) {
        // if not fire
        processKeyCode(keyCode);
    }
}

void MenuManager::processKeyCode(int keyCode)
{
    if (currentGameMenu != nullptr) {
        switch (micro->gameCanvas->getGameAction(keyCode)) {
        case Canvas::Keys::UP: // UP
            currentGameMenu->processGameActionUp();
            return;
        case Canvas::Keys::LEFT: // LEFT
            currentGameMenu->processGameActionUpd(3);
            if (currentGameMenu == gameMenuHighscore) {
                --field_360;
                if (field_360 < 0) {
                    field_360 = 0;
                }

                method_207(field_360);
            }
        case 3:
        case 4:
        case 7:
        default:
            break;
        case Canvas::Keys::RIGHT: // RIGHT
            currentGameMenu->processGameActionUpd(2);
            if (currentGameMenu == gameMenuHighscore) {
                ++field_360;
                if (field_360 > settingsStringLeague->getMaxAvailableOptionPos()) {
                    field_360 = settingsStringLeague->getMaxAvailableOptionPos();
                }

                method_207(field_360);
                return;
            }
            break;
        case Canvas::Keys::DOWN: // DOWN
            currentGameMenu->processGameActionDown();
            return;
        case Canvas::Keys::FIRE: // FIRE
            currentGameMenu->processGameActionUpd(1);
            return;
        }
    }
}

void MenuManager::method_206(Command* var1, Displayable* var2)
{
    (void)var2;
    if (var1 == commandOk) {
        if (currentGameMenu != nullptr) {
            currentGameMenu->processGameActionUpd(1);
            return;
        }
    } else if (var1 == commandBack && currentGameMenu != nullptr) {
        if (currentGameMenu == gameMenuIngame) {
            micro->menuToGame();
            return;
        }

        method_1(currentGameMenu->getGameMenu(), true);
    }
}

GameMenu* MenuManager::getGameMenu()
{
    return currentGameMenu;
}

void MenuManager::method_1(GameMenu* gm, bool var2)
{
    micro->gameCanvas->removeCommand(commandBack);
    if (gm != gameMenuMain && gm != gameMenuFinished && gm != nullptr) {
        micro->gameCanvas->addCommand(commandBack);
    }

    if (gm == gameMenuHighscore) {
        field_360 = settingsStringLeague->getCurrentOptionPos();
        method_207(field_360);
    } else if (gm == gameMenuFinished) {
        playerName = gameMenuEnterName->getStrArr();
        field_336->setText("Name - " + std::string(playerName));
    } else if (gm == gameMenuPlay) {
        settingsStringTrack->setOptionsList(micro->levelLoader->levelNames[settingStringLevel->getCurrentOptionPos()]);
        if (currentGameMenu == field_299) {
            field_345[settingStringLevel->getCurrentOptionPos()] = settingsStringTrack->getCurrentOptionPos();
        }

        settingsStringTrack->setAvailableOptions(availableTracks[settingStringLevel->getCurrentOptionPos()]);
        settingsStringTrack->setCurentOptionPos(field_345[settingStringLevel->getCurrentOptionPos()]);
    }

    if (gm == gameMenuMain || gm == gameMenuPlay) {
        micro->gamePhysics->enableGenerateInputAI();
    }

    currentGameMenu = gm;
    if (currentGameMenu != nullptr && !var2) {
        currentGameMenu->method_70();
    }

    field_377 = false;
}

void MenuManager::method_207(int var1)
{
    gameMenuHighscore->clearVector();
    recordManager->method_8(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
    gameMenuHighscore->addMenuElement(new TextRender(micro->levelLoader->getName(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos()), micro));
    gameMenuHighscore->addMenuElement(new TextRender("LEAGUE: " + settingsStringLeague->getOptionsList()[var1], micro));
    std::vector<std::string> var2 = recordManager->getRecordDescription(var1);

    for (std::size_t var3 = 0; var3 < var2.size(); ++var3) {
        if (var2[var3] != "") {
            TextRender* var4 = new TextRender(std::to_string(var3 + 1) + "." + var2[var3], micro);
            var4->setDx(GameCanvas::spriteSizeX[5] + 1);
            if (var3 == 0) {
                var4->setDrawSprite(true, 5);
            } else if (var3 == 1) {
                var4->setDrawSprite(true, 6);
            } else if (var3 == 2) {
                var4->setDrawSprite(true, 7);
            }

            gameMenuHighscore->addMenuElement(var4);
        }
    }

    recordManager->closeRecordStore();
    if (var2[0] == "") {
        gameMenuHighscore->addMenuElement(new TextRender("No Highscores", micro));
    }

    gameMenuHighscore->addMenuElement(settingStringBack);
}

void MenuManager::saveSmthToRecordStoreAndCloseIt()
{
    if (isRecordStoreOpened) {
        method_208();

        try {
            recordStore->closeRecordStore();
            isRecordStoreOpened = false;
        } catch (RecordStoreException& var1) {
            std::cout << var1.what() << std::endl;
        }
    }

    currentGameMenu = nullptr;
}

void MenuManager::method_208()
{
    // copyThreeBytesFromArr(16, playerName);

    setValue(0, (int8_t)perspectiveSetting->getCurrentOptionPos());
    setValue(1, (int8_t)shadowsSetting->getCurrentOptionPos());
    setValue(2, (int8_t)driverSpriteSetting->getCurrentOptionPos());
    setValue(3, (int8_t)bikeSpriteSetting->getCurrentOptionPos());
    setValue(14, (int8_t)inputSetting->getCurrentOptionPos());
    setValue(4, (int8_t)lookAheadSetting->getCurrentOptionPos());
    setValue(5, (int8_t)settingsStringLeague->getMaxAvailableOptionPos());
    setValue(6, (int8_t)settingStringLevel->getMaxAvailableOptionPos());
    setValue(10, (int8_t)settingStringLevel->getCurrentOptionPos());
    setValue(11, (int8_t)settingsStringTrack->getCurrentOptionPos());
    setValue(12, (int8_t)settingsStringLeague->getCurrentOptionPos());

    setValue(16, (int8_t)playerName[0]);
    setValue(17, (int8_t)playerName[1]);
    setValue(18, (int8_t)playerName[2]);

    for (int i = 0; i < 3; ++i) {
        setValue(7 + i, availableTracks[i]);
    }

    if (recorcStoreRecordId == -1) {
        try {
            recorcStoreRecordId = recordStore->addRecord(savedData, 0, 19);
        } catch (RecordStoreNotOpenException& var2) {
            std::cout << var2.what() << std::endl;
        } catch (RecordStoreException& var3) {
            std::cout << var3.what() << std::endl;
        }
    } else {
        try {
            recordStore->setRecord(recorcStoreRecordId, savedData, 0, 19);
        } catch (RecordStoreNotOpenException& var4) {
            std::cout << var4.what() << std::endl;
        } catch (RecordStoreException& var5) {
            std::cout << var5.what() << std::endl;
        }
    }
}

void MenuManager::run()
{
    // TODO
    // if (alert != nullptr) {
    //     Display.getDisplay(micro).setCurrent(alert);
    // }
}

void MenuManager::showAlert(std::string title, std::string alertText, Image* image)
{
    (void)title;
    (void)alertText;
    (void)image;
    // TODO
    // alert = new Alert(title, alertText, image, AlertType.INFO);
    // alert.setTimeout(4000);
    // (new Thread(this)).start();
}

void MenuManager::processMenu(IGameMenuElement* menuElement)
{
    if (menuElement == taskStart) {
        if (settingStringLevel->getCurrentOptionPos() <= settingStringLevel->getMaxAvailableOptionPos() && settingsStringTrack->getCurrentOptionPos() <= settingsStringTrack->getMaxAvailableOptionPos() && settingsStringLeague->getCurrentOptionPos() <= settingsStringLeague->getMaxAvailableOptionPos()) {
            micro->gamePhysics->disableGenerateInputAI();
            micro->levelLoader->method_88(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
            micro->gamePhysics->setMotoLeague(settingsStringLeague->getCurrentOptionPos());
            field_357 = true;
            micro->menuToGame();
        } else {
            showAlert("GDTR", "Complete more tracks to unlock this track/league combo.", nullptr);
        }
    } else if (menuElement == perspectiveSetting) {
        micro->gamePhysics->method_26(perspectiveSetting->getCurrentOptionPos() == 0);
        LevelLoader::isEnabledPerspective = perspectiveSetting->getCurrentOptionPos() == 0;
    } else if (menuElement == shadowsSetting) {
        LevelLoader::isEnabledShadows = shadowsSetting->getCurrentOptionPos() == 0;
    } else {
        if (menuElement == driverSpriteSetting) {
            if (driverSpriteSetting->method_114()) {
                driverSpriteSetting->setCurentOptionPos(driverSpriteSetting->getCurrentOptionPos() + 1);
                return;
            }
        } else if (menuElement == bikeSpriteSetting) {
            if (bikeSpriteSetting->method_114()) {
                bikeSpriteSetting->setCurentOptionPos(bikeSpriteSetting->getCurrentOptionPos() + 1);
                return;
            }
        } else {
            if (menuElement == inputSetting) {
                if (inputSetting->method_114()) {
                    inputSetting->setCurentOptionPos(inputSetting->getCurrentOptionPos() + 1);
                }

                micro->gameCanvas->method_163(inputSetting->getCurrentOptionPos());
                return;
            }

            if (menuElement == lookAheadSetting) {
                micro->gamePhysics->setEnableLookAhead(lookAheadSetting->getCurrentOptionPos() == 0);
                return;
            }

            if (menuElement == field_314) {
                if (currentGameMenu == gameMenuConfirmClear) {
                    recordManager->deleteRecordStores();
                    showAlert("Cleared", "Highscores have been cleared", nullptr);
                } else if (currentGameMenu == gameMenuConfirmReset) {
                    exit();
                    showAlert("Reset", "Master reset. Application will be closed.", nullptr);
                }

                method_1(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (menuElement == field_315) {
                method_1(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (menuElement == settingStringBack) {
                method_1(currentGameMenu->getGameMenu(), true);
                return;
            }

            if (menuElement == settingStringPlayMenu) {
                settingStringLevel->setCurentOptionPos(field_354);
                settingsStringTrack->setAvailableOptions(availableTracks[field_354]);
                settingsStringTrack->setCurentOptionPos(field_355);
                method_1(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (menuElement == settingStringGoToMain) {
                method_1(gameMenuMain, false);
                return;
            }

            if (menuElement == settingStringExitGame) {
                method_1(currentGameMenu->getGameMenu(), false);
                return;
            }

            if (menuElement == field_333) {
                if (settingsStringLeague->getCurrentOptionPos() <= settingsStringLeague->getMaxAvailableOptionPos()) {
                    settingStringLevel->setCurentOptionPos(field_354);
                    settingsStringTrack->setAvailableOptions(availableTracks[field_354]);
                    settingsStringTrack->setCurentOptionPos(field_355);
                    micro->gamePhysics->setMotoLeague(settingsStringLeague->getCurrentOptionPos());
                    field_357 = true;
                    micro->menuToGame();
                    return;
                }
            } else {
                if (menuElement == field_334) {
                    if (!field_356) {
                        settingsStringTrack->menuElemMethod(2);
                    }

                    micro->levelLoader->method_88(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
                    micro->gamePhysics->setMotoLeague(settingsStringLeague->getCurrentOptionPos());
                    method_208();
                    field_357 = true;
                    micro->menuToGame();
                    return;
                }

                if (menuElement == settingStringContinue) {
                    repaint();
                    micro->menuToGame();
                    return;
                }

                if (menuElement == field_336) {
                    gameMenuEnterName->method_70();
                    method_1(gameMenuEnterName, false);
                    return;
                }

                if (menuElement == field_335) {
                    method_197();
                    return;
                }

                if (menuElement == settingsStringTrack) {
                    if (settingsStringTrack->method_114()) {
                        settingsStringTrack->setAvailableOptions(availableTracks[settingStringLevel->getCurrentOptionPos()]);
                        settingsStringTrack->init();
                        field_299 = settingsStringTrack->getGameMenu();
                        method_1(field_299, false);
                        field_299->method_83(settingsStringTrack->getCurrentOptionPos());
                    }

                    field_345[settingStringLevel->getCurrentOptionPos()] = settingsStringTrack->getCurrentOptionPos();
                    return;
                }

                if (menuElement == settingStringLevel) {
                    if (settingStringLevel->method_114()) {
                        gameMenuStringLevel = settingStringLevel->getGameMenu();
                        method_1(gameMenuStringLevel, false);
                        gameMenuStringLevel->method_83(settingStringLevel->getCurrentOptionPos());
                    }

                    settingsStringTrack->setOptionsList(micro->levelLoader->levelNames[settingStringLevel->getCurrentOptionPos()]);
                    settingsStringTrack->setAvailableOptions(availableTracks[settingStringLevel->getCurrentOptionPos()]);
                    settingsStringTrack->setCurentOptionPos(field_345[settingStringLevel->getCurrentOptionPos()]);
                    settingsStringTrack->init();
                    return;
                }

                if (menuElement == settingsStringLeague && settingsStringLeague->method_114()) {
                    gameMenuLeague = settingsStringLeague->getGameMenu();
                    settingsStringLeague->setParentGameMenu(currentGameMenu);
                    method_1(gameMenuLeague, false);
                    gameMenuLeague->method_83(settingsStringLeague->getCurrentOptionPos());
                }
            }
        }
    }
}

int MenuManager::method_210()
{
    int var1 = 0;
    if (driverSpriteSetting->getCurrentOptionPos() == 0) {
        var1 |= 2;
    }

    if (bikeSpriteSetting->getCurrentOptionPos() == 0) {
        var1 |= 1;
    }

    return var1;
}

void MenuManager::method_211(int var1)
{
    bikeSpriteSetting->setCurentOptionPos(1);
    driverSpriteSetting->setCurentOptionPos(1);
    if ((var1 & 1) > 0) {
        bikeSpriteSetting->setCurentOptionPos(0);
    }

    if ((var1 & 2) > 0) {
        driverSpriteSetting->setCurentOptionPos(0);
    }
}

int MenuManager::method_212()
{
    return settingStringLevel->getCurrentOptionPos();
}

int MenuManager::method_213()
{
    return settingsStringTrack->getCurrentOptionPos();
}

int MenuManager::method_214()
{
    return settingsStringLeague->getCurrentOptionPos();
}

void MenuManager::method_215(int64_t var1)
{
    trackTimeMs = var1;
}

std::vector<int8_t> MenuManager::method_216(int var1, int8_t defaultValue)
{
    switch (var1) {
    case 16: {
        std::vector<int8_t> difficultySetting = std::vector<int8_t>(3);

        for (int idx = 0; idx < 3; ++idx) {
            difficultySetting[idx] = savedData[16 + idx];
        }

        if (difficultySetting[0] == -127) {
            difficultySetting[0] = defaultValue;
        }
        return difficultySetting;
    }
    default:
        return std::vector<int8_t>();
    }
}

int8_t MenuManager::getSettingOrDefault(int idx, int8_t defaultValue)
{
    return savedData[idx] == -127 ? defaultValue : savedData[idx];
}

// void MenuManager::copyThreeBytesFromArr(int var1, char* var2)
// {
//     if (isRecordStoreOpened && var1 == 16) {
//         for (int i = 0; i < 3; ++i) {
//             savedData[16 + i] = var2[i];
//         }
//     }
// }

void MenuManager::setValue(int pos, int8_t value)
{
    if (isRecordStoreOpened) {
        savedData[pos] = value;
    }
}

void MenuManager::exit()
{
    perspectiveSetting->setCurentOptionPos(0);
    shadowsSetting->setCurentOptionPos(0);
    driverSpriteSetting->setCurentOptionPos(0);
    bikeSpriteSetting->setCurentOptionPos(0);
    lookAheadSetting->setCurentOptionPos(0);
    settingsStringLeague->setCurentOptionPos(0);
    settingsStringLeague->setAvailableOptions(0);
    settingStringLevel->setCurentOptionPos(0);
    settingStringLevel->setAvailableOptions(1);
    settingsStringTrack->setCurentOptionPos(0);

    playerName = defaultName;
    inputSetting->setCurentOptionPos(0);
    availableTracks[0] = 0;
    availableTracks[1] = 0;
    availableTracks[2] = -1;
    availableLeagues = 0;
    method_208();
    recordManager->deleteRecordStores();
}

void MenuManager::removeOkAndBackCommands()
{
    micro->gameCanvas->removeCommand(commandOk);
    micro->gameCanvas->removeCommand(commandBack);
}

void MenuManager::addOkAndBackCommands()
{
    if (currentGameMenu != gameMenuMain && currentGameMenu != gameMenuFinished && currentGameMenu != nullptr) {
        micro->gameCanvas->addCommand(commandBack);
    }

    micro->gameCanvas->addCommand(commandOk);
}

int MenuManager::getCountOfRecordStoresWithPrefix(int prefixNumber)
{
    std::vector<std::string> storeNames = RecordStore::listRecordStores();
    if (recordManager != nullptr && !storeNames.empty()) {
        int count = 0;

        for (std::size_t i = 0; i < storeNames.size(); ++i) {
            if (storeNames[i].find(std::to_string(prefixNumber), 0) == 0) {
                ++count;
            }
        }

        return count;
    } else {
        return 0;
    }
}
