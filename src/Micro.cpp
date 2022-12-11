#include "Micro.h"

#include "GameCanvas.h"
#include "GamePhysics.h"
#include "MenuManager.h"
#include "LevelLoader.h"
#include "Helpers.h"
#include "lcdui/CanvasImpl.h"

bool Micro::field_249 = false;
bool Micro::isPaused = true;
int Micro::gameLoadingStateStage = 0;

Micro::Micro() {

}

Micro::~Micro() {
    // TODO
}

void Micro::setNumPhysicsLoops(int value) {
    numPhysicsLoops = value;
}

void Micro::gameToMenu() {
    gameCanvas->removeMenuCommand();
    isInGameMenu = true;
    menuManager->addOkAndBackCommands();
}

void Micro::menuToGame() {
    menuManager->removeOkAndBackCommands();
    isInGameMenu = false;
    gameCanvas->addMenuCommand();
}

int64_t Micro::goLoadingStep() {
    ++gameLoadingStateStage;
    gameCanvas->repaint();
    int64_t startTimeMillis = Helpers::currentTimeMillis();
    switch (gameLoadingStateStage) {
        case 1:
            levelLoader = new LevelLoader();
            break;
        case 2:
            gamePhysics = new GamePhysics(levelLoader);
            gameCanvas->init(gamePhysics);
            break;
        case 3:
            menuManager = new MenuManager(this); // TODO memory leak
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

            // try { // TODO check if it's ok
                Helpers::sleep(100LL);
            // } catch (InterruptedException var3) {
            // }
    }

    return Helpers::currentTimeMillis() - startTimeMillis;
}

void Micro::init() {
    int64_t timeToLoading = 3000L;
    // Thread.yield(); // TODO
    gameCanvas = new GameCanvas(this);
    // Display.getDisplay(this).setCurrent(gameCanvas); // TODO
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

/*

public static byte[] readBigFile(String var0, int var1) {
    byte[] result = null;

    try {
        InputStream var3 = new Object().getClass().getResourceAsStream("/" + var0);
        result = new byte[var1];
        var3.read(result, 0, var1);
    } catch (IOException var4) {
    }

    return result;
}

public static byte readByte(InputStream var0) {
    try {
        var0.read(singleByteArr, 0, 1);
    } catch (IOException var1) {
    }

    return singleByteArr[0];
}

public static InputStream readFile(String var0) {
    InputStream var1;
    if (var0.charAt(0) == '/') {
        var1 = var0.getClass().getResourceAsStream(var0);
    } else {
        var1 = var0.getClass().getResourceAsStream("/" + var0);
    }

    return var1;
}

*/

void Micro::restart(bool var1) {
    gamePhysics->resetSmth(true);
    timeMs = 0;
    gameTimeMs = 0;
    field_246 = 0;
    if (var1) {
        // gameCanvas->scheduleGameTimerTask(levelLoader.getName(menuManager->getCurrentLevel(), menuManager->getCurrentTrack()), 3000); // TODO
    }

    gameCanvas->method_129();
}

void Micro::destroyApp(bool var1) {
    (void)var1;
    field_249 = false;
    field_242 = true;
    menuManager->saveSmthToRecordStoreAndCloseIt();
    // notifyDestroyed(); // TODO
}



void Micro::startApp() {
    field_249 = true;
    isPaused = false;
    // if (thread == null) {
    //     thread = new Thread(this);
    //     thread.start();
    // }
    run();
}

/*

protected void pauseApp() {
    isPaused = true;
    if (!isInGameMenu) {
        gameToMenu();
    }

    System.gc();
}

*/

// original method
void Micro::run() {
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

    while (field_249) {
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
                    timeMs = Helpers::currentTimeMillis();
                }

                if ((var5 = gamePhysics->updatePhysics()) == 3 && field_246 == 0L) {
                    field_246 = Helpers::currentTimeMillis() + 3000L;
                    // gameCanvas->scheduleGameTimerTask("Crashed", 3000); // TODO
                    gameCanvas->repaint();
                    gameCanvas->serviceRepaints();
                }

                if (field_246 != 0L && field_246 < Helpers::currentTimeMillis()) {
                    restart(true);
                }

                if (var5 == 5) {
                    // gameCanvas->scheduleGameTimerTask("Crashed", 3000); // TODO
                    gameCanvas->repaint();
                    gameCanvas->serviceRepaints();

                    // try {
                        int64_t var7 = 1000L;
                        if (field_246 > 0L) {
                            var7 = std::min(field_246 - Helpers::currentTimeMillis(), 1000LL);
                        }

                        if (var7 > 0L) {
                            Helpers::sleep(var7);
                        }
                    // } catch (InterruptedException var12) { // TODO check if it's ok
                    // }

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

                    if (!field_249) {
                        break;
                    }
                }

                field_248 = var5 != 4;
            }

            var10000 = field_249;
        } catch (std::exception &var15) {
            continue;
        }

        if (!var10000) {
            break;
        }

        try {
            gamePhysics->method_53();
            int64_t var1;
            if ((var1 = Helpers::currentTimeMillis()) - var3 < 30L) {
                // TODO check if this replacement is ok
                // try {
                //     synchronized (this) {
                //         wait(Math.max(30L - (var1 - var3), 1L));
                //     }
                // } catch (InterruptedException var11) {
                // }
                Helpers::sleep(std::max(30LL - (var1 - var3), 1LL));

                var3 = Helpers::currentTimeMillis();
            } else {
                var3 = var1;
            }

            gameCanvas->repaint();
        } catch (std::exception &var14) {
        }
    }

    destroyApp(true);
}

void Micro::goalLoop() {
    int64_t var4 = 0L;
    if (!gamePhysics->field_69) {
        // gameCanvas->scheduleGameTimerTask("Wheelie!", 1000); // TODO
    } else {
        // gameCanvas->scheduleGameTimerTask("Finished", 1000); // TODO
    }

    for (int64_t timeMs = Helpers::currentTimeMillis() + 1000L; timeMs > Helpers::currentTimeMillis(); gameCanvas->repaint()) {
        if (isInGameMenu) {
            gameCanvas->repaint();
            return;
        }

        for (int i = numPhysicsLoops; i > 0; --i) {
            if (gamePhysics->updatePhysics() == 5) {
                // try { // TODO check if it's ok
                    int64_t deltaTime;
                    if ((deltaTime = timeMs - Helpers::currentTimeMillis()) > 0L) {
                        Helpers::sleep(deltaTime);
                    }

                    return;
                // } catch (InterruptedException var12) {
                    // return;
                // }
            }
        }

        gamePhysics->method_53();
        int64_t var2;
        if ((var2 = Helpers::currentTimeMillis()) - var4 < 30L) {
            // TODO check if it's ok
            // try {
            //     synchronized (this) {
            //         wait(Math.max(30L - (var2 - var4), 1L));
            //     }
            // } catch (InterruptedException var14) {
            // }
            Helpers::sleep(std::max(30L - (var2 - var4), 1LL));

            var4 = Helpers::currentTimeMillis();
        } else {
            var4 = var2;
        }
    }
}

void Micro::setMode(int mode) {
    gamePhysics->setMode(mode);
}