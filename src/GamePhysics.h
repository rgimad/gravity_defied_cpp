#pragma once

#include <vector>

#include "class_10.h"
#include "LevelLoader.h"


class GamePhysics {
private:
    int index01 = 0;
    int index10 = 1;
    int field_28 = -1;
    std::vector<TimerOrMotoPartOrMenuElem> field_30, motoComponents;
    int field_31 = 0;
    LevelLoader *levelLoader;
    int field_33 = 0;
    int field_34 = 0;
    bool field_35 = false;
    bool field_36 = false;
    int field_37 = 32768;
    int field_38 = 3276;
    int field_39 = 0;
    bool isGenerateInputAI = false;
    bool field_42 = false;
    int field_44 = 0;
    bool isInputAcceleration = false;
    bool isInputBreak = false;
    bool isInputBack = false;
    bool isInputForward = false;
    bool isInputUp = false;
    bool isInputDown = false;
    bool isInputLeft = false;
    bool isInputRight = false;
    bool field_68 = false;
    bool isEnableLookAhead = true;
    int camShiftX = 0;
    int camShiftY = 0;
    int field_73 = 655360;
    const int hardcodedArr1[8][2] = {{183500, -52428}, {262144, -163840}, {406323, -65536}, {445644, -39321}, {235929, 39321}, {16384, -144179}, {13107, -78643}, {288358, 81920}};
    const int hardcodedArr2[8][2] = {{190054, -111411}, {308019, -235929}, {334233, -114688}, {393216, -58982}, {262144, 98304}, {65536, -124518}, {13107, -78643}, {288358, 81920}};
    const int hardcodedArr3[8][2] = {{157286, 13107}, {294912, -13107}, {367001, 91750}, {406323, 190054}, {347340, 72089}, {39321, -98304}, {13107, -52428}, {294912, 81920}};
    const int hardcodedArr4[8][2] = {{183500, -39321}, {262144, -131072}, {393216, -65536}, {458752, -39321}, {294912, 6553}, {16384, -144179}, {13107, -78643}, {288358, 85196}};
    const int hardcodedArr5[8][2] = {{190054, -91750}, {255590, -235929}, {334233, -114688}, {393216, -42598}, {301465, 6553}, {65536, -78643}, {13107, -78643}, {288358, 85196}};
    const int hardcodedArr6[8][2] = {{157286, 13107}, {294912, -13107}, {367001, 104857}, {406323, 176947}, {347340, 72089}, {39321, -98304}, {13107, -52428}, {288358, 85196}};
    int field_80[3] = {45875, 32768, 52428};

    void method_27(int var1, int var2); // TOCHECK
    void setInputFromAI(); // TODO
    void method_35(); // TODO
    int method_39(int var1); // TODO
    void method_40(int var1); // TODO
    void method_42(class_10 &var1, TimerOrMotoPartOrMenuElem &var2, class_10 var3, int var4, int var5); // TODO
    void method_43(int var1, int var2, int var3); // TODO
    void method_44(int var1, int var2, int var3); // TODO
    void method_45(int var1); // TODO
    int method_46(int var1); // TODO
    void method_47(int var1); // TODO
    void renderEngine(GameCanvas &gameCanvas, int var2, int var3); // TODO
    void renderMotoFork(GameCanvas &gameCanvas); // TODO
    void renderWheelTires(GameCanvas &gameCanvas); // TODO
    void renderWheelSpokes(GameCanvas &gameCanvas); // TODO
    void renderSmth(GameCanvas &gameCanvas, int var2, int var3, int var4, int var5); // TODO
    void renderMotoAsLines(GameCanvas &gameCanvas, int var2, int var3, int var4, int var5); // TODO


public:
    static int field_7;
    static int field_8;
    static int field_9;
    static int field_10;
    static int field_11;
    static int motoParam1;
    static int motoParam2;
    static int field_14;
    static int motoParam10;
    static int field_16;
    static const int const175_1_half[3];
    static int motoParam3;
    static int field_19;

    static int motoParam4;
    static int motoParam5;
    static int motoParam6;
    static int motoParam7;
    static int motoParam8;
    static int motoParam9;

    std::vector<class_10> field_29;
    bool field_41 = false;
    int field_45 = 0;
    bool field_46 = 0;
    bool isRenderMotoWithSprites = false;

    static const int field_48 = 1;
    static const int field_49 = 0;
    static const int field_50 = 1;
    static const int field_51 = 2;
    static const int field_52 = 3;
    static int curentMotoLeague;
    static const int field_62 = 0;
    static const int field_63 = 1;
    static const int field_64 = 2;
    static const int field_65 = 3;
    static const int field_66 = 4;
    static const int field_67 = 5;
    bool field_69 = false;

    GamePhysics(LevelLoader *levelLoader);
    ~GamePhysics();

    int method_21();
    void method_22(int var1);
    void setMode(int mode);
    void setMotoLeague(int league);
    void resetSmth(bool unused);
    void method_26(bool var1); // TODO
    void setRenderMinMaxX(int minX, int maxX); // TODO
    void processPointerReleased(); // TODO
    void method_30(int var1, int var2); // TODO
    void enableGenerateInputAI(); // TODO
    void disableGenerateInputAI(); // TODO
    bool getIsGenerateInputAI();
    int updatePhysics(); // TODO
    bool isTrackStarted(); // TODO
    bool method_38(); // TODO
    static int getSmthLikeMaxAbs(int xF16, int yF16); // TODO
    void setEnableLookAhead(bool value); // TODO
    void setMinimalScreenWH(int minWH); // TODO
    int getCamPosX(); // TODO
    int getCamPosY(); // TODO
    int method_52(); // TODO
    void method_53(); // TODO
    void setMotoComponents(); // TODO
    void renderGame(GameCanvas *gameCanvas); // TODO

};