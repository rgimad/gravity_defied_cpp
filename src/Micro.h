#pragma once
#include <cstdint>
#include <filesystem>
#include <string>

class GameCanvas;
class GamePhysics;
class MenuManager;
class LevelLoader;

class Micro {
private:
    int64_t goLoadingStep();
    void destroyApp(bool var1);

public:
    inline static bool isInGameMenu;
    static bool gameStarted;
    static int gameLoadingStateStage;

    bool gameDestroyed = false;
    bool isInited = false;
    bool field_248 = false;
    int numPhysicsLoops = 2;
    int64_t timeMs = 0;
    int64_t gameTimeMs = 0;
    int64_t field_246 = 0;
    GameCanvas* gameCanvas;
    LevelLoader* levelLoader;
    GamePhysics* gamePhysics;
    MenuManager* menuManager;

    Micro();
    ~Micro();

    void startApp(int argc, char** argv);

    void gameToMenu();
    void menuToGame();
    void init();
    void restart(bool var1);
    void run();
    void goalLoop();
    void setNumPhysicsLoops(int value);
    void setMode(int mode);
    void showHelp(const char* progName);
};
