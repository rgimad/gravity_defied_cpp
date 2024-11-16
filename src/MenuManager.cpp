#include "MenuManager.h"

MenuManager::MenuManager(Micro* var1)
{
    micro = var1;
    field_376 = std::make_unique<TextRender>("", var1);
}

void MenuManager::initPart(int var1)
{
    switch (var1) {
    case 1: {
        recordManager = new RecordManager();
        trackTimeMs = 0;
        trackTimeFormatted.clear();

        return;
    }
    case 2: {
        settings = SettingsManager::loadSettings();

        if (settings.playerName[0] < 'A' || settings.playerName[0] > 'Z') {
            strncpy(settings.playerName, defaultName, PLAYER_NAME_MAX);
        }

        // cheat code: set name to RKE to enable all leagues ???
        if (strncmp(settings.playerName, cheatCode, PLAYER_NAME_MAX) == 0) {
            settings.availableLeagues = 3;
            settings.availableLevels = 2;
            settings.availableTracks[0] = (int8_t)(micro->levelLoader->getTracksCount(0));
            settings.availableTracks[1] = (int8_t)(micro->levelLoader->getTracksCount(1));
            settings.availableTracks[2] = (int8_t)(micro->levelLoader->getTracksCount(2));
            return;
        }

        return;
    }
    case 3: {
        field_354 = settings.selectedLevel;
        field_355 = settings.selectedTrack;

        try {
            field_345.at(settings.selectedLevel) = settings.selectedTrack;
        } catch (std::exception& var6) {
            settings.selectedLevel = 0;
            settings.selectedTrack = 0;
            field_345[settings.selectedLevel] = settings.selectedTrack;
        }

        LevelLoader::isEnabledPerspective = settings.perspective == 0;
        LevelLoader::isEnabledShadows = settings.shadows == 0;
        micro->gamePhysics->setEnableLookAhead(settings.lookAhead == 0);
        micro->gameCanvas->method_124(field_372 == 0);
        leagueNamesAll4 = { "100cc", "175cc", "220cc", "325cc" };

        if (settings.availableLeagues < 3) {
            this->leagueNames = { "100cc", "175cc", "220cc" };
        } else {
            this->leagueNames = leagueNamesAll4;
        }

        field_360 = settings.selectedLeague;
        return;
    }
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
        return;
    }
    case 5: {
        gameMenuIngame = new GameMenu("Ingame", micro, gameMenuPlay);
        gameMenuEnterName = new GameMenu("Enter Name", micro, gameMenuFinished, settings.playerName);
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
        settingStringLevel = new SettingsStringRender("Level", settings.selectedLevel, this, levelLabels, false, micro, gameMenuPlay, false);
        settingsStringTrack = new SettingsStringRender("Track", field_345[settings.selectedLevel], this, micro->levelLoader->GetTrackNames(settingStringLevel->getCurrentOptionPos()), false, micro, gameMenuPlay, false);
        settingsStringLeague = new SettingsStringRender("League", settings.selectedLeague, this, leagueNames, false, micro, gameMenuPlay, false);

        try {
            settingsStringTrack->setAvailableOptions(settings.availableTracks[settings.selectedLevel]);
        } catch (std::exception& var5) {
            settingsStringTrack->setAvailableOptions(0);
        }

        settingStringLevel->setAvailableOptions(settings.availableLevels);
        settingsStringLeague->setAvailableOptions(settings.availableLeagues);
        gameTimerTaskHighscore = new TimerOrMotoPartOrMenuElem("Highscore", gameMenuHighscore, this);
        gameMenuHighscore->addMenuElement(settingStringBack);
        taskStart = new SettingsStringRender("Start>", 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        gameMenuPlay->addMenuElement(taskStart);
        gameMenuPlay->addMenuElement(settingStringLevel);
        gameMenuPlay->addMenuElement(settingsStringTrack);
        gameMenuPlay->addMenuElement(settingsStringLeague);
        gameMenuPlay->addMenuElement(gameTimerTaskHighscore);
        gameMenuPlay->addMenuElement(settingStringGoToMain);

        perspectiveSetting = new SettingsStringRender("Perspective", settings.perspective, this, onOffLabels, true, micro, gameMenuOptions, false);
        shadowsSetting = new SettingsStringRender("Shadows", settings.shadows, this, onOffLabels, true, micro, gameMenuOptions, false);
        driverSpriteSetting = new SettingsStringRender("Driver sprite", settings.driverSprite, this, onOffLabels, true, micro, gameMenuOptions, false);
        bikeSpriteSetting = new SettingsStringRender("Bike sprite", settings.bikeSprite, this, onOffLabels, true, micro, gameMenuOptions, false);
        // inputSetting = new SettingsStringRender("Input", settings.input, this, inputLabels, false, micro, gameMenuOptions, false);
        lookAheadSetting = new SettingsStringRender("Look ahead", settings.lookAhead, this, onOffLabels, true, micro, gameMenuOptions, false);
        clearHighscoreSetting = new TimerOrMotoPartOrMenuElem("Clear highscore", gameMenuConfirmClear, this);
        return;
    }
    case 6: {
        gameMenuOptions->addMenuElement(perspectiveSetting);
        gameMenuOptions->addMenuElement(shadowsSetting);
        gameMenuOptions->addMenuElement(driverSpriteSetting);
        gameMenuOptions->addMenuElement(bikeSpriteSetting);
        // gameMenuOptions->addMenuElement(inputSetting);
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
        // field_319 = new GameMenu("Keys", micro, gameMenuHelp);
        // field_320 = new TimerOrMotoPartOrMenuElem("Keys", field_319, this);
        // addTextRender(field_319, "- " + inputLabels[0] + " -");
        // addTextRender(field_319, "UP accelerates, DOWN brakes, RIGHT leans forward and LEFT leans backward. 1 accelerates and leans backward. 3 accelerates and leans forward. 7 brakes and leans backward. 9 brakes and leans forward.");
        // field_319->addMenuElement(field_376.get());
        // addTextRender(field_319, "- " + inputLabels[1] + " -");
        // addTextRender(field_319, "1 accelerates, 4 brakes, 6 leans forward and 5 leans backward.");
        // field_319->addMenuElement(field_376.get());
        // addTextRender(field_319, "- " + inputLabels[2] + " -");
        // addTextRender(field_319, "3 accelerates, 6 brakes, 5 leans forward and 4 leans backward.");
        // field_319->addMenuElement(settingStringBack);
        // gameMenuHelp->addMenuElement(field_320);
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
    }
    case 7: {
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
        // gameMenuOptions2->addMenuElement(field_376.get());
        // addTextRender(gameMenuOptions2, "Input: Keyset 1,2,3 ");
        // addTextRender(gameMenuOptions2, "Default: <1>. Determines which type of input should be used when playing. See \"Keys\" in the help menu for more info.");
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
        field_336 = new SettingsStringRender("Name - " + std::string(settings.playerName, PLAYER_NAME_MAX), 0, this, std::vector<std::string>(), false, micro, gameMenuMain, true);
        commandOk = new Command("Ok", Command::Type::OK, 1);
        commandBack = new Command("Back", Command::Type::BACK, 1);
        method_1(gameMenuMain, false);

        rasterImage = std::make_unique<Image>("raster.png");
        return;
    }
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

bool MenuManager::isRestartNeeded()
{
    if (restartNeeded) {
        restartNeeded = false;
        return true;
    }

    return false;
}

void MenuManager::method_197()
{
    recordManager->loadRecordInfo(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
    recordManager->addNewRecord(settingsStringLeague->getCurrentOptionPos(), settings.playerName, trackTimeMs);
    recordManager->writeRecordInfo(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());

    currentLevelFinished = false;
    gameMenuFinished->clearVector();
    gameMenuFinished->addMenuElement(new TextRender("Time: " + trackTimeFormatted, micro));
    const std::array<std::string, RECORD_NO_MAX> recordDescriptions = recordManager->getRecordDescription(settingsStringLeague->getCurrentOptionPos());

    for (uint8_t i = 0; i < recordDescriptions.size(); ++i) {
        if (recordDescriptions[i].empty()) {
            continue;
        }

        std::stringstream ss;
        ss << (i + 1) << "." << recordDescriptions[i];
        gameMenuFinished->addMenuElement(new TextRender(ss.str(), micro));
    }

    int8_t availableLeagues = -1;

    if (settingsStringTrack->getMaxAvailableOptionPos() >= settingsStringTrack->getCurrentOptionPos()) {
        settingsStringTrack->setAvailableOptions(settingsStringTrack->getCurrentOptionPos() + 1 < settings.availableTracks[settingStringLevel->getCurrentOptionPos()] ? settings.availableTracks[settingStringLevel->getCurrentOptionPos()] : settingsStringTrack->getCurrentOptionPos() + 1);
        settings.availableTracks[settingStringLevel->getCurrentOptionPos()] = (int8_t)settingsStringTrack->getMaxAvailableOptionPos() < settings.availableTracks[settingStringLevel->getCurrentOptionPos()] ? settings.availableTracks[settingStringLevel->getCurrentOptionPos()] : (int8_t)settingsStringTrack->getMaxAvailableOptionPos();
    }

    if (settingsStringTrack->getCurrentOptionPos() == settingsStringTrack->getMaxOptionPos()) {
        currentLevelFinished = true;

        availableLeagues = settingStringLevel->getCurrentOptionPos() + 1;

        if (availableLeagues == 3) {
            settingsStringLeague->setOptionsList(leagueNamesAll4);
            leagueNames = leagueNamesAll4;
        }

        settingsStringLeague->setAvailableOptions(availableLeagues);
        settingStringLevel->setAvailableOptions(settingStringLevel->getMaxAvailableOptionPos() + 1);

        if (settings.availableTracks[settingStringLevel->getMaxAvailableOptionPos()] == -1) {
            settings.availableTracks[settingStringLevel->getMaxAvailableOptionPos()] = 0;
        }
    }

    const uint32_t numberOfRecords = RecordManager::getNumberOfRecordsForLevel(settingStringLevel->getCurrentOptionPos());
    addTextRender(gameMenuFinished, std::to_string(numberOfRecords) + " of " + std::to_string(micro->levelLoader->getTracksCount(settingStringLevel->getCurrentOptionPos())) + " tracks in " + levelLabels[settingStringLevel->getCurrentOptionPos()] + " completed.");

    if (!currentLevelFinished) {
        field_333->setText("Restart: " + micro->levelLoader->getName(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos()));
        field_334->setText("Next: " + micro->levelLoader->getName(field_354, field_355 + 1));
    } else {
        if (settingStringLevel->getCurrentOptionPos() < settingStringLevel->getMaxOptionPos()) {
            settingStringLevel->setCurentOptionPos(settingStringLevel->getCurrentOptionPos() + 1);
            settingsStringTrack->setCurentOptionPos(0);
            settingsStringTrack->setAvailableOptions(settings.availableTracks[settingStringLevel->getCurrentOptionPos()]);
        }

        if (availableLeagues != -1) {
            addTextRender(gameMenuFinished, "Congratulations! You have successfully unlocked a new league: " + leagueNames[availableLeagues]);
            if (availableLeagues == 3) {
                gameMenuFinished->addMenuElement(new TextRender("Enjoy...", micro));
            }

            showAlert("League unlocked", "You have successfully unlocked a new league: " + leagueNames[availableLeagues], nullptr);
        } else {
            bool var4 = true;

            for (int var5 = 0; var5 < 3; ++var5) {
                if (settings.availableTracks[var5] != micro->levelLoader->getTracksCount(var5)) {
                    var4 = false;
                }
            }

            if (!var4) {
                addTextRender(gameMenuFinished, "You have completed all tracks at this level.");
            }
        }
    }

    if (!currentLevelFinished) {
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
        restartNeeded = true;
        break;
    case 1:
        field_354 = settingStringLevel->getCurrentOptionPos();
        field_355 = settingsStringTrack->getCurrentOptionPos();
        field_333->setText("Restart: " + micro->levelLoader->getName(field_354, field_355));
        restartNeeded = false;
        method_1(gameMenuIngame, false);
        break;
    case 2: {
        gameMenuFinished->clearVector();
        field_354 = settingStringLevel->getCurrentOptionPos();
        field_355 = settingsStringTrack->getCurrentOptionPos();
        recordManager->loadRecordInfo(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
        const uint8_t var2 = recordManager->getPosOfNewRecord(settingsStringLeague->getCurrentOptionPos(), trackTimeMs);
        trackTimeFormatted = Time::timeToString(trackTimeMs);

        if (var2 < RECORD_NO_MAX) {
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

void MenuManager::processKeyCode(const Keys keyCode)
{
    if (currentGameMenu == nullptr) {
        return;
    }

    switch (keyCode) {
    case Keys::UP: {
        currentGameMenu->processGameActionUp();
        return;
    }
    case Keys::LEFT: {
        currentGameMenu->processGameActionUpd(3);

        if (currentGameMenu == gameMenuHighscore) {
            --field_360;
            if (field_360 < 0) {
                field_360 = 0;
            }

            method_207(field_360);
        }

        return;
    }
    case Keys::RIGHT: {
        currentGameMenu->processGameActionUpd(2);

        if (currentGameMenu == gameMenuHighscore) {
            ++field_360;
            if (field_360 > settingsStringLeague->getMaxAvailableOptionPos()) {
                field_360 = settingsStringLeague->getMaxAvailableOptionPos();
            }

            method_207(field_360);
            return;
        }

        return;
    }
    case Keys::DOWN: {
        currentGameMenu->processGameActionDown();
        return;
    }
    case Keys::FIRE: {
        currentGameMenu->processGameActionUpd(1);
        return;
    }
    default: {
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
        strncpy(settings.playerName, gameMenuEnterName->getStrArr(), PLAYER_NAME_MAX);
        field_336->setText("Name - " + std::string(settings.playerName, PLAYER_NAME_MAX));
    } else if (gm == gameMenuPlay) {
        settingsStringTrack->setOptionsList(micro->levelLoader->GetTrackNames(settingStringLevel->getCurrentOptionPos()));

        if (currentGameMenu == field_299) {
            field_345[settingStringLevel->getCurrentOptionPos()] = settingsStringTrack->getCurrentOptionPos();
        }

        settingsStringTrack->setAvailableOptions(settings.availableTracks[settingStringLevel->getCurrentOptionPos()]);
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
    recordManager->loadRecordInfo(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
    gameMenuHighscore->addMenuElement(new TextRender(micro->levelLoader->getName(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos()), micro));
    gameMenuHighscore->addMenuElement(new TextRender("LEAGUE: " + settingsStringLeague->getOptionsList()[var1], micro));
    const std::array<std::string, RECORD_NO_MAX> recordDescription = recordManager->getRecordDescription(var1);

    for (uint8_t i = 0; i < recordDescription.size(); ++i) {
        if (recordDescription[i].empty()) {
            continue;
        }

        std::stringstream ss;
        ss << (i + 1) << '.' << recordDescription[i];
        TextRender* var4 = new TextRender(ss.str(), micro);
        var4->setDx(GameCanvas::spriteSizeX[5] + 1);
        var4->setDrawSprite(true, 5 + i);
        gameMenuHighscore->addMenuElement(var4);
    }

    if (recordDescription[0].empty()) {
        gameMenuHighscore->addMenuElement(new TextRender("No Highscores", micro));
    }

    gameMenuHighscore->addMenuElement(settingStringBack);
}

void MenuManager::saveSmthToRecordStoreAndCloseIt()
{
    method_208();
    currentGameMenu = nullptr;
}

void MenuManager::method_208()
{
    settings.perspective = (int8_t)perspectiveSetting->getCurrentOptionPos();
    settings.shadows = (int8_t)shadowsSetting->getCurrentOptionPos();
    settings.driverSprite = (int8_t)driverSpriteSetting->getCurrentOptionPos();
    settings.bikeSprite = (int8_t)bikeSpriteSetting->getCurrentOptionPos();
    settings.lookAhead = (int8_t)lookAheadSetting->getCurrentOptionPos();
    settings.availableLeagues = (int8_t)settingsStringLeague->getMaxAvailableOptionPos();
    settings.availableLevels = (int8_t)settingStringLevel->getMaxAvailableOptionPos();
    // settings.availableEasyTracks =  (int8_t)availableTracks[0];
    // settings.availableMediumTracks =  (int8_t)availableTracks[1];
    // settings.availableHardTracks =  (int8_t)availableTracks[2];
    settings.selectedLevel = (int8_t)settingStringLevel->getMaxAvailableOptionPos();
    settings.selectedTrack = (int8_t)settingsStringTrack->getCurrentOptionPos();
    settings.selectedLeague = (int8_t)settingsStringLeague->getCurrentOptionPos();
    settings.unknown2 = -127;
    // settings.input = (int8_t)inputSetting->getCurrentOptionPos();
    settings.input = 0;
    settings.unknown3 = -127;

    SettingsManager::saveSettings(settings);
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
            micro->levelLoader->loadTrack(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
            micro->gamePhysics->setMotoLeague(settingsStringLeague->getCurrentOptionPos());
            restartNeeded = true;
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
            // if (menuElement == inputSetting) {
            //     if (inputSetting->method_114()) {
            //         inputSetting->setCurentOptionPos(inputSetting->getCurrentOptionPos() + 1);
            //     }

            //     micro->gameCanvas->method_163(inputSetting->getCurrentOptionPos());
            //     return;
            // }

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
                settingsStringTrack->setAvailableOptions(settings.availableTracks[field_354]);
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
                    settingsStringTrack->setAvailableOptions(settings.availableTracks[field_354]);
                    settingsStringTrack->setCurentOptionPos(field_355);
                    micro->gamePhysics->setMotoLeague(settingsStringLeague->getCurrentOptionPos());
                    restartNeeded = true;
                    micro->menuToGame();
                    return;
                }
            } else {
                if (menuElement == field_334) {
                    if (!currentLevelFinished) {
                        settingsStringTrack->menuElemMethod(2);
                    }

                    micro->levelLoader->loadTrack(settingStringLevel->getCurrentOptionPos(), settingsStringTrack->getCurrentOptionPos());
                    micro->gamePhysics->setMotoLeague(settingsStringLeague->getCurrentOptionPos());
                    method_208();
                    restartNeeded = true;
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
                        settingsStringTrack->setAvailableOptions(settings.availableTracks[settingStringLevel->getCurrentOptionPos()]);
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

                    settingsStringTrack->setOptionsList(micro->levelLoader->GetTrackNames(settingStringLevel->getCurrentOptionPos()));
                    settingsStringTrack->setAvailableOptions(settings.availableTracks[settingStringLevel->getCurrentOptionPos()]);
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

void MenuManager::setGameTimeMs(const uint64_t timeMs)
{
    trackTimeMs = timeMs;
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

    // playerName = defaultName;
    // availableTracks[0] = 0;
    // availableTracks[1] = 0;
    // availableTracks[2] = -1;
    // availableLeagues = 0;

    // inputSetting->setCurentOptionPos(0);
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
