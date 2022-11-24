#pragma once

#include "lcdui/Graphics.h"
#include "lcdui/Command.h"
#include "GamePhysics.h"

class GameCanvas { // extends Canvas implements CommandListener
private:
    Graphics *graphics = nullptr;
    int dx;
    int dy;
    int engineSpriteWidth;
    int engineSpriteHeight;
    int fenderSpriteWidth;
    int fenderSpriteHeight;
    GamePhysics *gamePhysics = nullptr;
    MenuManager *menuManager = nullptr;
    int field_178 = 0;
    int field_179 = 0;
    Micro *micro = nullptr;
    Font *font = nullptr;
    bool field_182 = false;
    bool isDrawingTime = true;
    int field_184 = 1;
    Image *splashImage = nullptr;
    Image *logoImage = nullptr;

    // Image[] bodyPartsImages = new Image[3];
    Image *engineImage = nullptr;
    Image *fenderImage = nullptr;
    // Image[] unusedImagesArr = null;
    // Image[] unusedImagesArr2 = null;
    // Image[][] imagesArr = new Image[3][];
    // Image onePixImage = Image.createImage(1, 1);
    static Image *spritesImage = nullptr;
    // int[] bodyPartsSpriteWidth = new int[3];
    // int[] bodyPartsSpriteHeight = new int[3];
    static int defaultFontWidth00 = 25;
    bool field_205 = true;
    // int field_206;
    // Image screenBuffer = null;
    // Graphics screenBufferGraphics = null;
    // boolean useScreenBuffer = false;
    // String field_210 = null;
    // int countOfScheduledTimers = 0;
    // Timer timer = new Timer();
    Command *commandMenu;
    // Object objectForSyncronization = new Object();
    // int pointerX = 0;
    // int pointerY = 0;
    // static String stringWithTime = "";
    // static String[] time10MsToStringCache = new String[100];
    // int timeInSeconds = -1;
    // static int flagAnimationTime = 0;
    // static int field_226 = 0;
    // final int field_227 = 262144;
    // final int[] startFlagAnimationTimeToSpriteNo = new int[]{12, 10, 11, 10};
    // final int[] finishFlagAnumationTimeToSpriteNo = new int[]{14, 13, 15, 13};
    // byte[][] field_230 = new byte[][]{{0, 0}, {1, 0}, {0, -1}, {0, 0}, {0, 0}, {0, 1}, {-1, 0}};
    // byte[][][] field_231 = new byte[][][]{{{0, 0}, {1, -1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1}}, {{0, 0}, {1, 0}, {0, 0}, {0, 0}, {-1, 0}, {0, -1}, {0, 1}, {0, 0}, {0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, 0}, {0, 0}, {0, 0}, {0, 0}}};
    // int field_232 = 2;
    // final int field_233 = 7;
    // final int field_234 = 10;
    // boolean[] activeActions = new boolean[7];
    // boolean[] activeKeys = new boolean[10];

public:
    GameCanvas(Micro *micro);

    void drawSprite(Graphics *g, int spriteNo, int x, int y);
    void requestRepaint(int var1);
    void method_124(bool var1);
    void updateSizeAndRepaint();
    int loadSprites(int var1);

    static const int field_163 = 0;
    static const int field_164 = 1;
    static const int field_165 = 2;
    int width;
    int height2;
    int height;

    // Image helmetImage = null;
    // Image[] field_195 = null;
    int helmetSpriteWidth;
    int helmetSpriteHeight;
    bool isHasPinterEvents = false;
    // long field_214 = -1L;
    const int spriteOffsetX[] = {0, 0, 15, 15, 15, 0, 6, 12, 18, 18, 25, 25, 25, 37, 37, 37, 15, 32};
    const int spriteOffsetY[] = {10, 25, 16, 20, 10, 0, 0, 0, 8, 0, 0, 6, 12, 0, 6, 12, 29, 18};
    const int spriteSizeX[] = {15, 15, 8, 8, 3, 6, 6, 6, 7, 7, 12, 12, 12, 12, 12, 12, 16, 17};
    const int spriteSizeY[] = {15, 15, 4, 4, 3, 10, 10, 10, 8, 8, 6, 6, 6, 6, 6, 6, 11, 22};
};