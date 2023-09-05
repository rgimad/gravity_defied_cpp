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
    void method_164();
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
    int field_178 = 0;
    int field_179 = 0;
    Micro* micro = nullptr;
    std::shared_ptr<Font> font;
    bool timerTriggered = false;
    int field_184 = 1;
    std::unique_ptr<Image> splashImage;
    std::unique_ptr<Image> logoImage;
    std::unique_ptr<Image> bodyPartsImages[3];
    std::unique_ptr<Image> engineImage;
    std::unique_ptr<Image> fenderImage;
    std::unique_ptr<Image> onePixImage;
    std::unique_ptr<Image> spritesImage;
    int bodyPartsSpriteWidth[3] = { 0, 0, 0 };
    int bodyPartsSpriteHeight[3] = { 0, 0, 0 };
    inline static int defaultFontWidth00 = 25;
    bool field_205 = true;
    int field_206;
    std::unique_ptr<Image> screenBuffer;
    std::string timerMessage = "";
    int timerId = 0;
    std::vector<Timer> timers;
    Command* commandMenu;
    inline static std::string stringWithTime = "";
    std::vector<std::string> time10MsToStringCache = std::vector<std::string>(100);
    int timeInSeconds = -1;
    inline static int flagAnimationTime = 0;
    inline static int field_226 = 0;
    const int startFlagAnimationTimeToSpriteNo[4] = { 12, 10, 11, 10 };
    const int finishFlagAnumationTimeToSpriteNo[4] = { 14, 13, 15, 13 };
    int field_230[7][2] = { { 0, 0 }, { 1, 0 }, { 0, -1 }, { 0, 0 }, { 0, 0 }, { 0, 1 }, { -1, 0 } };
    int field_231[3][10][2] = { { { 0, 0 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, -1 }, { -1, 0 }, { 0, 1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } }, { { 0, 0 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, { -1, 0 }, { 0, -1 }, { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } };
    int field_232 = 2;
    std::vector<bool> activeActions = std::vector<bool>(7);
    std::vector<bool> activeKeys = std::vector<bool>(10);

    // int fps;

public:
    GameCanvas(Micro* micro);
    void drawSprite(Graphics* g, int spriteNo, int x, int y);
    void requestRepaint(int var1);
    void method_124(bool var1);
    void updateSizeAndRepaint();
    int loadSprites(int flags);
    void method_129();
    void setViewPosition(int dx, int dy);
    int getDx();
    int addDx(int x);
    int addDy(int y);
    void drawLine(int x, int y, int x2, int y2);
    void drawLineF16(int x, int y, int x2, int y2);
    void renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo);
    void renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo, int tF16);
    void method_142(int var1, int var2, int var3, int var4);
    void drawCircle(int x, int y, int size);
    void fillRect(int x, int y, int w, int h);
    void drawForthSpriteByCenter(int centerX, int centerY);
    void drawHelmet(int var1, int var2, int var3);
    void drawTime(int64_t time10Ms);
    void method_150(int var1);
    static void method_151();
    void renderStartFlag(int x, int y);
    void renderFinishFlag(int x, int y);
    void drawWheelTires(int x, int y, int wheelIsThin);
    int calcSpriteNo(int angleF16, int var2, int var3, int var4, bool var5);
    void renderEngine(int x, int y, int angleF16);
    void renderFender(int x, int y, int angleF16);
    void clearScreenWithWhite();
    void setColor(int red, int green, int blue);
    void drawGame(Graphics* g);
    void method_161(int var1, bool mode);
    void method_163(int var1);
    void paint(Graphics* g);
    void init(GamePhysics* gamePhysics);
    void processKeyPressed(int keyCode);
    void processKeyReleased(int keyCode);
    void scheduleGameTimerTask(std::string var1, int delayMs);
    void setMenuManager(MenuManager* menuManager);
    void method_168(Command* var1, Displayable* var2);
    void keyPressed(int var1);
    void keyReleased(int var1);
    void commandAction(Command* var1, Displayable* var2);
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