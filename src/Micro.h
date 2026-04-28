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
    void destroyApp(bool var1); // TODO: unused parameter
    std::filesystem::path mrgFilePath;

public:
    GameCanvas* gameCanvas;
    LevelLoader* levelLoader;
    GamePhysics* gamePhysics;
    MenuManager* menuManager;
    bool isAboutToExit = false;
    int numPhysicsLoops = 2;
    int64_t timeMs = 0;
    int64_t gameTimeMs = 0;
    int64_t crashRestartTimeMs = 0;
    bool isInited = false;
    bool advanceGameTime = false;
    static bool isGameLoopRunning;
    inline static bool isInGameMenu;
    static int gameLoadingStateStage;

    Micro();
    ~Micro();

    void startApp(int argc, char** argv);

    void gameToMenu();
    void menuToGame();
    void init();
    void restart(bool scheduleTimerTask);
    void run();
    void goalLoop();
    void setNumPhysicsLoops(int value);
    void setMode(int mode);
    void showHelp(const char* progName);
};
