#include "Micro.h"

#include "GameCanvas.h"
#include "GamePhysics.h"
#include "MenuManager.h"
#include "LevelLoader.h"
#include "utils/Time.h"
#include "lcdui/CanvasImpl.h"
#include "rms/RecordStore.h"

bool Micro::isGameLoopRunning = false;
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
    menuManager->addCommands();
}

void Micro::menuToGame()
{
    menuManager->removeCommands();
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
        levelLoader = new LevelLoader(mrgFilePath);
        break;
    case 2:
        gamePhysics = new GamePhysics(levelLoader);
        gameCanvas->init(gamePhysics);
        break;
    case 3:
        menuManager = new MenuManager(this);
        menuManager->initializePhase(1);
        break;
    case 4:
        menuManager->initializePhase(2);
        break;
    case 5:
        menuManager->initializePhase(3);
        break;
    case 6:
        menuManager->initializePhase(4);
        break;
    case 7:
        menuManager->initializePhase(5);
        break;
    case 8:
        menuManager->initializePhase(6);
        break;
    case 9:
        menuManager->initializePhase(7);
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
        // } catch (InterruptedException e) {
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

void Micro::restart(bool scheduleTimerTask)
{
    gamePhysics->resetPhysicsState(true);
    timeMs = 0;
    gameTimeMs = 0;
    crashRestartTimeMs = 0;
    if (scheduleTimerTask) {
        gameCanvas->scheduleGameTimerTask(levelLoader->getName(menuManager->getSelectedLevel(), menuManager->getSelectedTrack()), 3000);
    }

    gameCanvas->reset();
}

void Micro::destroyApp(bool var1) // TODO: unused parameter
{
    (void)var1;
    isGameLoopRunning = false;
    isAboutToExit = true;
    menuManager->saveStateAndCloseRecordStore();
}

void Micro::startApp(int argc, char** argv)
{
    if (argc > 1) {
        std::string argv1(argv[1]);

        if (argv1 == "-h" || argv1 == "--help") {
            showHelp(argv[0]);
            return;
        }

        this->mrgFilePath = argv1;
    }

    RecordStore::setRecordStoreDir(argv[0]);

    isGameLoopRunning = true;
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
    menuManager->runMenuLoop(0);
    if (menuManager->consumeShouldStartRaceFlag()) {
        restart(true);
    }

    int64_t lastMillis = 0L;

    while (isGameLoopRunning) {
        int physicsState;
        if (gamePhysics->getRenderModeIndex() != menuManager->getGraphicsFlags()) {
            physicsState = gameCanvas->loadSprites(menuManager->getGraphicsFlags());
            gamePhysics->setRenderFlags(physicsState);
            menuManager->applyGraphicsFlags(physicsState);
        }

        bool shouldContinueLoop;
        try {
            if (isInGameMenu) {
                menuManager->runMenuLoop(1);
                if (menuManager->consumeShouldStartRaceFlag()) {
                    restart(true);
                }
            }

            for (int i = numPhysicsLoops; i > 0; --i) {
                if (advanceGameTime) {
                    gameTimeMs += 20L;
                }

                if (timeMs == 0L) {
                    timeMs = Time::currentTimeMillis();
                }

                if ((physicsState = gamePhysics->updatePhysics()) == 3 && crashRestartTimeMs == 0L) {
                    crashRestartTimeMs = Time::currentTimeMillis() + 3000L;
                    gameCanvas->scheduleGameTimerTask("Crashed", 3000);
                    gameCanvas->repaint();
                    gameCanvas->serviceRepaints();
                }

                if (crashRestartTimeMs != 0L && crashRestartTimeMs < Time::currentTimeMillis()) {
                    restart(true);
                }

                if (physicsState == 5) {
                    gameCanvas->scheduleGameTimerTask("Crashed", 3000);
                    gameCanvas->repaint();
                    gameCanvas->serviceRepaints();

                    // try {
                    //     long var7 = 1000L;
                    //     if (this.crashRestartTimeMs > 0L) {
                    //         var7 = Math.min(this.crashRestartTimeMs - System.currentTimeMillis(), 1000L);
                    //     }

                    //     if (var7 > 0L) {
                    //         Thread.sleep(var7);
                    //     }
                    // } catch (InterruptedException e) {
                    // }
                    int64_t crashRestartDelay = 1000L;
                    if (crashRestartTimeMs > 0L) {
                        crashRestartDelay = std::min(crashRestartTimeMs - Time::currentTimeMillis(), static_cast<int64_t>(1000));
                    }

                    if (crashRestartDelay > 0L) {
                        Time::sleep(crashRestartDelay);
                    }

                    restart(true);
                } else if (physicsState == 4) {
                    timeMs = 0L;
                    gameTimeMs = 0L;
                } else if (physicsState == 1 || physicsState == 2) {
                    if (physicsState == 2) {
                        gameTimeMs -= 10L;
                    }

                    goalLoop();
                    menuManager->setFinishTime(gameTimeMs / 10L);
                    menuManager->runMenuLoop(2);
                    if (menuManager->consumeShouldStartRaceFlag()) {
                        restart(true);
                    }

                    if (!isGameLoopRunning) {
                        break;
                    }
                }

                advanceGameTime = physicsState != 4;
            }

            shouldContinueLoop = isGameLoopRunning;
        } catch (std::exception& e) {
            continue;
        }

        if (!shouldContinueLoop) {
            break;
        }

        try {
            gamePhysics->captureRenderSnapshot();
            int64_t curMillis;
            if ((curMillis = Time::currentTimeMillis()) - lastMillis < 30L) {
                // try {
                //     synchronized (this) {
                //         wait(Math.max(30L - (var1 - var3), 1L));
                //     }
                // } catch (InterruptedException e) {
                // }
                Time::sleep(std::max(30LL - (curMillis - lastMillis), 1LL));

                lastMillis = Time::currentTimeMillis();
            } else {
                lastMillis = curMillis;
            }

            gameCanvas->repaint();
        } catch (std::exception& e) {
        }
    }

    destroyApp(true);
}

void Micro::goalLoop()
{
    int64_t lastFrameTime = 0L;
    if (!gamePhysics->frontWheelContactLatch) {
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
                // } catch (InterruptedException e) {
                //     return;
                // }
                int64_t deltaTime;
                if ((deltaTime = timeMs - Time::currentTimeMillis()) > 0L) {
                    Time::sleep(deltaTime);
                }

                return;
            }
        }

        gamePhysics->captureRenderSnapshot();
        int64_t currentTime;
        if ((currentTime = Time::currentTimeMillis()) - lastFrameTime < 30L) {
            // try {
            //     synchronized (this) {
            //         wait(Math.max(30L - (currentTime - lastFrameTime), 1L));
            //     }
            // } catch (InterruptedException e) {
            // }
            Time::sleep(std::max(30LL - (currentTime - lastFrameTime), 1LL));

            lastFrameTime = Time::currentTimeMillis();
        } else {
            lastFrameTime = currentTime;
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