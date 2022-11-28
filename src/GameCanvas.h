#pragma once

#include <string.h>
#include <memory>
#include <vector>

#include "lcdui/Graphics.h"
#include "lcdui/Command.h"

#include "Micro.h"

class GamePhysics;

class GameCanvas { // extends Canvas implements CommandListener
private:
    void repaint();
    void serviceRepaints();
    bool hasPointerEvents();

    void method_164();
    void handleUpdatedInput();

    Graphics *graphics = nullptr;
    int dx;
    int dy;
    int engineSpriteWidth;
    int engineSpriteHeight;
    int fenderSpriteWidth;
    int fenderSpriteHeight;
    // GamePhysics *gamePhysics = nullptr;
    // MenuManager *menuManager = nullptr;
    int field_178 = 0;
    int field_179 = 0;
    Micro *micro = nullptr;
    Font *font = nullptr;
    bool field_182 = false;
    bool isDrawingTime = true;
    int field_184 = 1;
    Image *splashImage = nullptr;
    Image *logoImage = nullptr;

    Image *bodyPartsImages[3] = {nullptr, nullptr, nullptr}; // TODO: memory leak
    // std::vector<std::unique_ptr<Image>> v = std::vector<std::unique_ptr<Image>>(3);

    Image *engineImage = nullptr;
    Image *fenderImage = nullptr;
    // Image[] unusedImagesArr = null;
    // Image[] unusedImagesArr2 = null;
    // Image[][] imagesArr = new Image[3][];
    Image *onePixImage = Image::createImage(1, 1);
    inline static Image *spritesImage = nullptr;
    int bodyPartsSpriteWidth[3] = {0, 0, 0};
    int bodyPartsSpriteHeight[3] = {0, 0, 0};
    inline static int defaultFontWidth00 = 25;
    bool field_205 = true;
    int field_206;
    Image *screenBuffer = nullptr;
    // Graphics screenBufferGraphics = null;
    // String field_210 = null;
    int countOfScheduledTimers = 0;
    // Timer timer = new Timer();
    Command *commandMenu;
    // Object objectForSyncronization = new Object();
    int pointerX = 0;
    int pointerY = 0;
    inline static std::string stringWithTime = "";
    std::vector<std::string> time10MsToStringCache = std::vector<std::string>(100);
    int timeInSeconds = -1;
    inline static int flagAnimationTime = 0;
    inline static int field_226 = 0;
    // final int field_227 = 262144;
    const int startFlagAnimationTimeToSpriteNo[4] = {12, 10, 11, 10};
    const int finishFlagAnumationTimeToSpriteNo[4] = {14, 13, 15, 13};
    int field_230[7][2] = {{0, 0}, {1, 0}, {0, -1}, {0, 0}, {0, 0}, {0, 1}, {-1, 0}};
    int field_231[3][10][2] = {{{0, 0}, {1, -1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1}}, {{0, 0}, {1, 0}, {0, 0}, {0, 0}, {-1, 0}, {0, -1}, {0, 1}, {0, 0}, {0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, 0}, {0, 0}, {0, 0}, {0, 0}}};
    int field_232 = 2;
    // final int field_233 = 7;
    // final int field_234 = 10;
    std::vector<bool> activeActions = std::vector<bool>(7);
    std::vector<bool> activeKeys = std::vector<bool>(10);

public:
    GameCanvas(Micro *micro);

    void drawSprite(Graphics *g, int spriteNo, int x, int y);
    void requestRepaint(int var1);
    void method_124(bool var1);
    void updateSizeAndRepaint();
    int loadSprites(int var1);
    void method_129();
    // void init(GamePhysics *gamePhysics);
    Image* loadImage(std::string imgName);
    void setViewPosition(int dx, int dy);
    int getDx();
    int addDx(int x);
    int addDy(int y);
    void renderGreyCircles();
    void drawLine(int x, int y, int x2, int y2);
    void drawLineF16(int x, int y, int x2, int y2);
    void renderBodyPart(int var1, int var2, int var3, int var4, int var5);
    void renderBodyPart(int var1, int var2, int var3, int var4, int bodyPartNo, int var6);
    void method_142(int var1, int var2, int var3, int var4);
    void drawCircle(int x, int y, int size);
    void fillRect(int x, int y, int w, int h);
    void drawForthSpriteByCenter(int centerX, int centerY);
    void method_146(int var1, int var2, int var3);
    void drawTime(long time10Ms);
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
    void render_160(Graphics *g); // TODO: rename to drawGame()
    void method_161(int var1, bool mode);
    void method_163(int var1);

    static const int field_163 = 0;
    static const int field_164 = 1;
    static const int field_165 = 2;
    int width;
    int height2;
    int height;

    Image *helmetImage = nullptr;
    // Image[] field_195 = null;
    int helmetSpriteWidth;
    int helmetSpriteHeight;
    bool isHasPinterEvents = false;
    // long field_214 = -1L;
    const int spriteOffsetX[18] = {0, 0, 15, 15, 15, 0, 6, 12, 18, 18, 25, 25, 25, 37, 37, 37, 15, 32};
    const int spriteOffsetY[18] = {10, 25, 16, 20, 10, 0, 0, 0, 8, 0, 0, 6, 12, 0, 6, 12, 29, 18};
    const int spriteSizeX[18] = {15, 15, 8, 8, 3, 6, 6, 6, 7, 7, 12, 12, 12, 12, 12, 12, 16, 17};
    const int spriteSizeY[18] = {15, 15, 4, 4, 3, 10, 10, 10, 8, 8, 6, 6, 6, 6, 6, 6, 11, 22};

protected:
    void processKeyPressed(int keyCode);
    void processKeyReleased(int keyCode);
    int getGameAction(int key);
    void hideNotify();
    void showNotify();
};