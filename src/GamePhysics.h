#pragma once
#include "class_10.h"

/*
class GamePhysics {
private:
    int index01 = 0;
    int index10 = 1;
    int field_28 = -1;
    TimerOrMotoPartOrMenuElem[] field_30;
    TimerOrMotoPartOrMenuElem[] motoComponents = new TimerOrMotoPartOrMenuElem[6];
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
    int field_44;
    bool isInputAcceleration;
    bool isInputBreak;
    bool isInputBack;
    bool isInputForward;
    bool isInputUp;
    bool isInputDown;
    bool isInputLeft;
    bool isInputRight;
    bool field_68;
    bool isEnableLookAhead;
    int camShiftX;
    int camShiftY;
    int field_73;
    const int[][] hardcodedArr1;
    const int[][] hardcodedArr2;
    const int[][] hardcodedArr3;
    const int[][] hardcodedArr4;
    const int[][] hardcodedArr5;
    const int[][] hardcodedArr6;
    int[][] field_80;

    void method_27(int var1, int var2);
    void setInputFromAI();
    void method_35();
    int method_39(int var1);
    void method_40(int var1);
    void method_42(class_10 *var1, TimerOrMotoPartOrMenuElem *var2, class_10 var3, int var4, int var5);
    void method_43(int var1, int var2, int var3);
    void method_44(int var1, int var2, int var3);
    void method_45(int var1);
    int method_46(int var1);
    void method_47(int var1);
    void renderEngine(GameCanvas *gameCanvas, int var2, int var3);
    void renderMotoFork(GameCanvas *gameCanvas);
    void renderWheelTires(GameCanvas *gameCanvas);
    void renderWheelSpokes(GameCanvas *gameCanvas);
    void renderSmth(GameCanvas *gameCanvas, int var2, int var3, int var4, int var5);
    void renderMotoAsLines(GameCanvas *gameCanvas, int var2, int var3, int var4, int var5);


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
    static const int const175_1_half[] = {114688, 65536, 32768};
    static int motoParam3;
    static int field_19;

    static int motoParam4;
    static int motoParam5;
    static int motoParam6;
    static int motoParam7;
    static int motoParam8;
    static int motoParam9;

    class_10[] field_29;
    bool field_41 = false;
    int field_45;
    bool field_46;
    bool isRenderMotoWithSprites;

    static const int field_48 = 1;
    static const int field_49 = 0;
    static const int field_50 = 1;
    static const int field_51 = 2;
    static const int field_52 = 3;
    static int curentMotoLeague = 0;
    static const int field_62 = 0;
    static const int field_63 = 1;
    static const int field_64 = 2;
    static const int field_65 = 3;
    static const int field_66 = 4;
    static const int field_67 = 5;
    bool field_69;

    GamePhysics(LevelLoader *levelLoader);
    ~GamePhysics();

    int method_21();
    void method_22(int var1);
    void setMode(int mode);
    void setMotoLeague(int league);
    void resetSmth(bool unused);
    void method_26(bool var1);
    void setRenderMinMaxX(int minX, int maxX);
    void processPointerReleased();
    void method_30(int var1, int var2);
    void enableGenerateInputAI();
    void disableGenerateInputAI();
    bool isGenerateInputAI();
    int updatePhysics();
    bool isTrackStarted();
    bool method_38();
    static int getSmthLikeMaxAbs(int xF16, int yF16);
    void setEnableLookAhead(bool value);
    void setMinimalScreenWH(int minWH);
    int getCamPosX();
    int getCamPosY();
    int method_52();
    void method_53();
    void setMotoComponents();
    void renderGame(GameCanvas *gameCanvas);

};
*/