#include "Micro.h"

#include "GameCanvas.h"
#include "GamePhysics.h"
#include "MenuManager.h"
#include "LevelLoader.h"
#include "utils/Time.h"
#include "utils/Hashing.h"
#include "lcdui/CanvasImpl.h"
#include "rms/RecordStore.h"
#include "config.h"

bool Micro::gameStarted = false;
int Micro::gameLoadingStateStage = 0;

Micro::Micro()
{
}

Micro::~Micro()
{
}

void Micro::setNumPhysicsLoops(int value)
{
    numPhysicsLoops = value;
}

void Micro::gameToMenu()
{
    gameCanvas->removeMenuCommand();
    isInGameMenu = true;
    menuManager->addOkAndBackCommands();
}

void Micro::menuToGame()
{
    menuManager->removeOkAndBackCommands();
    isInGameMenu = false;
    gameCanvas->addMenuCommand();
}

int64_t Micro::goLoadingStep()
{
    ++gameLoadingStateStage;
    gameCanvas->repaint();
    int64_t startTimeMillis = Time::currentTimeMillis();
    switch (gameLoadingStateStage) {
    case 1:
        levelLoader = new LevelLoader(GlobalSetting::MrgFilePath);
        break;
    case 2:
        gamePhysics = new GamePhysics(levelLoader);
        gameCanvas->init(gamePhysics);
        break;
    case 3:
        menuManager = new MenuManager(this);
        menuManager->initPart(1);
        break;
    case 4:
        menuManager->initPart(2);
        break;
    case 5:
        menuManager->initPart(3);
        break;
    case 6:
        menuManager->initPart(4);
        break;
    case 7:
        menuManager->initPart(5);
        break;
    case 8:
        menuManager->initPart(6);
        break;
    case 9:
        menuManager->initPart(7);
        break;
    case 10:
        gameCanvas->setMenuManager(menuManager);
        gameCanvas->setViewPosition(-50, 150);
        setMode(1);
        break;
    default:
        --gameLoadingStateStage;

        // try {
        //     Thread.sleep(100L);
        // } catch (InterruptedException var3) {
        // }
        Time::sleep(100LL);
    }

    return Time::currentTimeMillis() - startTimeMillis;
}

void Micro::init()
{
    int64_t timeToLoading = 3000L;
    // Thread.yield();
    gameCanvas = new GameCanvas(this);
    gameCanvas->requestRepaint(1);

    while (!gameCanvas->isShown()) {
        goLoadingStep();
    }

    int64_t deltaTimeMs;
    while (timeToLoading > 0L) {
        deltaTimeMs = goLoadingStep();
        timeToLoading -= deltaTimeMs;
    }

    gameCanvas->requestRepaint(2);

    for (timeToLoading = 3000L; timeToLoading > 0L; timeToLoading -= deltaTimeMs) {
        deltaTimeMs = goLoadingStep();
    }

    while (gameLoadingStateStage < 10) {
        goLoadingStep();
    }

    gameCanvas->requestRepaint(0);
    isInited = true;
}

void Micro::restart(bool var1)
{
    gamePhysics->resetSmth(true);
    timeMs = 0;
    gameTimeMs = 0;
    field_246 = 0;
    if (var1) {
        gameCanvas->scheduleGameTimerTask(levelLoader->getName(menuManager->getCurrentLevel(), menuManager->getCurrentTrack()), 3000);
    }

    gameCanvas->method_129();
}

void Micro::destroyApp(bool var1)
{
    (void)var1;
    gameStarted = false;
    gameDestroyed = true;
    menuManager->saveSmthToRecordStoreAndCloseIt();
}

void Micro::startApp(int argc, char** argv)
{
    if (argc > 1) {
        std::string argv1(argv[1]);

        if (argv1 == "-h" || argv1 == "--help") {
            showHelp(argv[0]);
            return;
        }

        GlobalSetting::MrgFilePath = argv1;
    }

    std::cout << "path: " << GlobalSetting::MrgFilePath << std::endl;
    GlobalSetting::SavesPrefix = Hashing::HashFileMD5(GlobalSetting::MrgFilePath.string());
    std::cout << "hash: " << GlobalSetting::SavesPrefix << std::endl;

    RecordStore::setRecordStoreDir(argv[0]);

    gameStarted = true;
    // if (thread == null) {
    //     thread = new Thread(this);
    //     thread.start();
    // }
    run();
}

// original method
void Micro::run()
{
    if (!isInited) {
        init();
    }

    gameCanvas->setCommandListener(gameCanvas);
    restart(false);
    menuManager->method_201(0);

    if (menuManager->method_196()) {
        restart(true);
    }

    int64_t var3 = 0L;

    while (gameStarted) {
        int var5;

        if (gamePhysics->method_21() != menuManager->method_210()) {
            var5 = gameCanvas->loadSprites(menuManager->method_210());
            gamePhysics->method_22(var5);
            menuManager->method_211(var5);
        }

        bool var10000;
        try {
            if (isInGameMenu) {
                menuManager->method_201(1);

                if (menuManager->method_196()) {
                    restart(true);
                }
            }

            for (int i = numPhysicsLoops; i > 0; --i) {
                if (field_248) {
                    gameTimeMs += 20L;
                }

                if (timeMs == 0L) {
                    timeMs = Time::currentTimeMillis();
                }

                if ((var5 = gamePhysics->updatePhysics()) == 3 && field_246 == 0L) {
                    field_246 = Time::currentTimeMillis() + 3000L;
                    gameCanvas->scheduleGameTimerTask("Crashed", 3000);
                    gameCanvas->repaint();
                    gameCanvas->serviceRepaints();
                }

                if (field_246 != 0L && field_246 < Time::currentTimeMillis()) {
                    restart(true);
                }

                if (var5 == 5) {
                    gameCanvas->scheduleGameTimerTask("Crashed", 3000);
                    gameCanvas->repaint();
                    gameCanvas->serviceRepaints();

                    // try {
                    //     long var7 = 1000L;
                    //     if (this.field_246 > 0L) {
                    //         var7 = Math.min(this.field_246 - System.currentTimeMillis(), 1000L);
                    //     }

                    //     if (var7 > 0L) {
                    //         Thread.sleep(var7);
                    //     }
                    // } catch (InterruptedException var12) {
                    // }
                    int64_t var7 = 1000L;

                    if (field_246 > 0L) {
                        var7 = std::min(field_246 - Time::currentTimeMillis(), static_cast<int64_t>(1000));
                    }

                    if (var7 > 0L) {
                        Time::sleep(var7);
                    }

                    restart(true);
                } else if (var5 == 4) {
                    timeMs = 0L;
                    gameTimeMs = 0L;
                } else if (var5 == 1 || var5 == 2) {
                    if (var5 == 2) {
                        gameTimeMs -= 10L;
                    }

                    goalLoop();
                    menuManager->method_215(gameTimeMs / 10L);
                    menuManager->method_201(2);

                    if (menuManager->method_196()) {
                        restart(true);
                    }

                    if (!gameStarted) {
                        break;
                    }
                }

                field_248 = var5 != 4;
            }

            var10000 = gameStarted;
        } catch (std::exception& var15) {
            continue;
        }

        if (!var10000) {
            break;
        }

        try {
            gamePhysics->method_53();
            int64_t var1;

            if ((var1 = Time::currentTimeMillis()) - var3 < 30L) {
                // try {
                //     synchronized (this) {
                //         wait(Math.max(30L - (var1 - var3), 1L));
                //     }
                // } catch (InterruptedException var11) {
                // }
                Time::sleep(std::max(30LL - (var1 - var3), 1LL));

                var3 = Time::currentTimeMillis();
            } else {
                var3 = var1;
            }

            gameCanvas->repaint();
        } catch (std::exception& var14) {
        }
    }

    destroyApp(true);
}

void Micro::goalLoop()
{
    int64_t var4 = 0L;

    if (!gamePhysics->field_69) {
        gameCanvas->scheduleGameTimerTask("Wheelie!", 1000);
    } else {
        gameCanvas->scheduleGameTimerTask("Finished", 1000);
    }

    for (int64_t timeMs = Time::currentTimeMillis() + 1000L; timeMs > Time::currentTimeMillis(); gameCanvas->repaint()) {
        if (isInGameMenu) {
            gameCanvas->repaint();
            return;
        }

        for (int i = numPhysicsLoops; i > 0; --i) {
            if (gamePhysics->updatePhysics() == 5) {
                // try {
                //     long deltaTime;
                //     if ((deltaTime = timeMs - System.currentTimeMillis()) > 0L) {
                //         Thread.sleep(deltaTime);
                //     }

                //     return;
                // } catch (InterruptedException var12) {
                //     return;
                // }
                int64_t deltaTime;

                if ((deltaTime = timeMs - Time::currentTimeMillis()) > 0L) {
                    Time::sleep(deltaTime);
                }

                return;
            }
        }

        gamePhysics->method_53();
        int64_t var2;

        if ((var2 = Time::currentTimeMillis()) - var4 < 30L) {
            // try {
            //     synchronized (this) {
            //         wait(Math.max(30L - (var2 - var4), 1L));
            //     }
            // } catch (InterruptedException var14) {
            // }
            Time::sleep(std::max(30LL - (var2 - var4), 1LL));

            var4 = Time::currentTimeMillis();
        } else {
            var4 = var2;
        }
    }
}

void Micro::setMode(int mode)
{
    gamePhysics->setMode(mode);
}

void Micro::showHelp(const char* progName)
{
    std::cout << "Usage: " << progName << " <FILE>" << std::endl
              << "Example:" << std::endl
              << "  " << progName << " levels.mrg  # A path to a custom levels file could be specified" << std::endl
              << "  " << progName << "             # When no path is specified, the built-in levels file will be used" << std::endl
              << std::endl;
}