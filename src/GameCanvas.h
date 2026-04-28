#pragma once

#include <string>
#include <memory>
#include <vector>

#include "lcdui/Graphics.h"
#include "lcdui/Command.h"
#include "lcdui/Canvas.h"
#include "lcdui/CommandListener.h"

#include "Micro.h"
#include "Timer.h"

class GamePhysics;
class MenuManager;

class GameCanvas : public Canvas, public CommandListener {
private:
    void resetActiveKeys();
    void handleUpdatedInput();
    void processTimers();

    Graphics* graphics = nullptr;
    int dx;
    int dy;
    int engineSpriteWidth;
    int engineSpriteHeight;
    int fenderSpriteWidth;
    int fenderSpriteHeight;
    GamePhysics* gamePhysics = nullptr;
    MenuManager* menuManager = nullptr;
    // Additional offsets for camera view adjustment
    int cameraOffsetX = 0;
    int cameraOffsetY = 0;
    Micro* micro = nullptr;
    std::shared_ptr<Font> font;
    bool timerTriggered = false;
    // 0=gameplay, 1=logo screen, 2=splash screen
    int loadingScreenState = 1;
    std::unique_ptr<Image> splashImage;
    std::unique_ptr<Image> logoImage;
    std::unique_ptr<Image> bodyPartsImages[3];
    std::unique_ptr<Image> engineImage;
    std::unique_ptr<Image> fenderImage;
    std::unique_ptr<Image> onePixImage; // Unused
    std::unique_ptr<Image> spritesImage;
    int bodyPartsSpriteWidth[3] = { 0, 0, 0 };
    int bodyPartsSpriteHeight[3] = { 0, 0, 0 };
    inline static int defaultFontWidth00 = 25;
    bool unknown_bool = true; // Set by setInputConfigEnabled() but never read
    int unused_field; // Dead code - never used
    std::unique_ptr<Image> screenBuffer; // Unused
    std::string timerMessage = "";
    int timerId = 0;
    std::vector<Timer> timers;
    Command* commandMenu;
    inline static std::string stringWithTime = "";
    std::vector<std::string> time10MsToStringCache = std::vector<std::string>(100);
    int timeInSeconds = -1;
    inline static int flagAnimationTime = 0;
    // Fixed-point animation phase accumulator
    inline static int flagAnimationPhase = 0;
    const int startFlagAnimationTimeToSpriteNo[4] = { 12, 10, 11, 10 };
    const int finishFlagAnumationTimeToSpriteNo[4] = { 14, 13, 15, 13 };
    // Maps game actions to direction vectors
    int actionDirectionMap[7][2] = { { 0, 0 }, { 1, 0 }, { 0, -1 }, { 0, 0 }, { 0, 0 }, { 0, 1 }, { -1, 0 } };
    // Current input configuration (0, 1, or 2)
    int keyDirectionMap[3][10][2] = { { { 0, 0 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, -1 }, { -1, 0 }, { 0, 1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } }, { { 0, 0 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, { -1, 0 }, { 0, -1 }, { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } }; // Maps numeric keys to directions per input config
    int inputConfigIndex = 2;
    std::vector<bool> activeActions = std::vector<bool>(7);
    std::vector<bool> activeKeys = std::vector<bool>(10);

    // int fps;

public:
    GameCanvas(Micro* micro);
    void drawSprite(Graphics* g, int spriteNo, int x, int y);
    void requestRepaint(int state);
    void setInputConfigEnabled(bool enabled); // Sets unknown_bool (never read)
    void updateSizeAndRepaint();
    int loadSprites(int flags);
    void reset();
    void setViewPosition(int dx, int dy);
    int getDx();
    int addDx(int x);
    int addDy(int y);
    void drawLine(int x, int y, int x2, int y2);
    void drawLineF16(int x, int y, int x2, int y2);
    void renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo);
    void renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo, int tF16);
    void drawWheelHub(int centerX, int centerY, int radius, int angleF16);
    void drawCircle(int x, int y, int size);
    void fillRect(int x, int y, int w, int h);
    void drawAttachmentPointSprite(int x, int y);
    void drawHelmet(int x, int y, int angleF16);
    void drawTime(int64_t time10Ms);
    void triggerTimerIfMatching(int timerId);
    static void flagAnimation();
    void renderStartFlag(int x, int y);
    void renderFinishFlag(int x, int y);
    void drawWheelTires(int x, int y, int wheelIsThin);
    int calcSpriteNo(int angleF16, int angleOffset, int angleRange, int spriteCount, bool flipDirection);
    void renderEngine(int x, int y, int angleF16);
    void renderFender(int x, int y, int angleF16);
    void clearScreenWithWhite();
    void setColor(int red, int green, int blue);
    void drawGame(Graphics* g);
    void drawProgressBar(int progress, bool mode);
    void setInputConfigIndex(int configIndex);
    void paint(Graphics* g);
    void init(GamePhysics* gamePhysics);
    void processKeyPressed(int keyCode);
    void processKeyReleased(int keyCode);
    void scheduleGameTimerTask(const std::string& message, int delayMs);
    void setMenuManager(MenuManager* menuManager);
    void handleMenuCommand(Command* command, Displayable* displayable);
    void keyPressed(int keyCode);
    void keyReleased(int keyCode);
    void commandAction(Command* command, Displayable* displayable);
    void removeMenuCommand();
    void addMenuCommand();

    int width;
    int height2;
    int height;
    std::unique_ptr<Image> helmetImage;
    int helmetSpriteWidth;
    int helmetSpriteHeight;
    bool isDrawingTime = true;
    inline static const int spriteOffsetX[18] = { 0, 0, 15, 15, 15, 0, 6, 12, 18, 18, 25, 25, 25, 37, 37, 37, 15, 32 };
    inline static const int spriteOffsetY[18] = { 10, 25, 16, 20, 10, 0, 0, 0, 8, 0, 0, 6, 12, 0, 6, 12, 29, 18 };
    inline static const int spriteSizeX[18] = { 15, 15, 8, 8, 3, 6, 6, 6, 7, 7, 12, 12, 12, 12, 12, 12, 16, 17 };
    inline static const int spriteSizeY[18] = { 15, 15, 4, 4, 3, 10, 10, 10, 8, 8, 6, 6, 6, 6, 6, 6, 11, 22 };
};
