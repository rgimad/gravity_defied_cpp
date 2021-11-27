#pragma once
#include <cstdint>

class Game {
private:
    // TODO: static uint8_t singleByteArr[1]; ?
    int64_t goLoadingStep();
    void destroyApp(bool var1);
    void startApp();
    void pauseApp();

public:
    GameCanvas *gameCanvas;
    LevelLoader *levelLoader;
    GamePhysics *gamePhysics;
    MenuManager *menuManager;
    bool field_242;
    int numPhysicsLoops;
    int64_t timeMs;
    int64_t gameTimeMs;
    int64_t field_246;
    bool isInited;
    bool field_248;
    static bool field_249;
    static bool isPaused;;
    static bool isInGameMenu;
    static int gameLoadingStateStage;

    Game();
    ~Game();

    void gameToMenu();
    void menuToGame();
    void init();
    // NOTE: methods readBigFile, readByte, readFile skipped because unused
    void restart(bool var1);
    void run();
    void goalLoop();
    void setNumPhysicsLoops(int value);
    void setMode(int mode);
};