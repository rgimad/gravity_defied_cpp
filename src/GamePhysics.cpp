#include "GamePhysics.h"

const int GamePhysics::const175_1_half[] = {114688, 65536, 32768};
int GamePhysics::curentMotoLeague = 0;


GamePhysics::GamePhysics(LevelLoader *levelLoader) {
    motoComponents = new TimerOrMotoPartOrMenuElem*[6];
    for (int i = 0; i < 6; ++i) {
        motoComponents[i] = new TimerOrMotoPartOrMenuElem();
    }
    field_44 = 0;
    field_45 = 0;
    field_46 = 0;
    isRenderMotoWithSprites = false;
    isInputAcceleration = false;
    isInputBreak = false;
    isInputBack = false;
    isInputForward = false;
    isInputUp = false;
    isInputDown = false;
    isInputLeft = false;
    isInputRight = false;
    field_68 = false;
    field_69 = false;
    isEnableLookAhead = true;
    camShiftX = 0;
    camShiftY = 0;
    field_73 = 655360;
    levelLoader = levelLoader;
    resetSmth(true);
    isGenerateInputAI = false;
    method_53();
    field_35 = false;
}

int GamePhysics::method_21() {
    if (field_46 && isRenderMotoWithSprites) {
        return 3;
    } else if (isRenderMotoWithSprites) {
        return 1;
    } else {
        return field_46 ? 2 : 0;
    }
}

void GamePhysics::method_22(int var1) {
    field_46 = false;
    isRenderMotoWithSprites = false;
    if ((var1 & 2) != 0) {
        field_46 = true;
    }
    if ((var1 & 1) != 0) {
        isRenderMotoWithSprites = true;
    }
}

void GamePhysics::setMode(int mode) {
    field_45 = mode;
    switch (mode) {
        case 1:
        default:
            field_7 = 1310;
            field_8 = 1638400;
            setMotoLeague(1);
            resetSmth(true);
    }
}

void GamePhysics::setMotoLeague(int league) {
    curentMotoLeague = league;
    field_9 = 45875;
    field_10 = 13107;
    field_11 = 39321;
    field_14 = 1310720;
    field_16 = 262144;
    field_19 = 6553;
    switch (league) {
        case 0:
        default:
            motoParam1 = 19660;
            motoParam2 = 19660;
            motoParam3 = 1114112;
            motoParam4 = 52428800;
            motoParam5 = 3276800;
            motoParam6 = 327;
            motoParam7 = 0;
            motoParam8 = 32768;
            motoParam9 = 327680;
            motoParam10 = 19660800;
            break;
        case 1:
            motoParam1 = 32768;
            motoParam2 = 32768;
            motoParam3 = 1114112;
            motoParam4 = 65536000;
            motoParam5 = 3276800;
            motoParam6 = 6553;
            motoParam7 = 26214;
            motoParam8 = 26214;
            motoParam9 = 327680;
            motoParam10 = 19660800;
            break;
        case 2:
            motoParam1 = 32768;
            motoParam2 = 32768;
            motoParam3 = 1310720;
            motoParam4 = 75366400;
            motoParam5 = 3473408;
            motoParam6 = 6553;
            motoParam7 = 26214;
            motoParam8 = 39321;
            motoParam9 = 327680;
            motoParam10 = 21626880;
            break;
        case 3:
            motoParam1 = 32768;
            motoParam2 = 32768;
            motoParam3 = 1441792;
            motoParam4 = 78643200;
            motoParam5 = 3538944;
            motoParam6 = 6553;
            motoParam7 = 26214;
            motoParam8 = 65536;
            motoParam9 = 1310720;
            motoParam10 = 21626880;
    }
    resetSmth(true);
}






bool GamePhysics::getIsGenerateInputAI() {
    return isGenerateInputAI;
}

GamePhysics::~GamePhysics() {
    //
}



