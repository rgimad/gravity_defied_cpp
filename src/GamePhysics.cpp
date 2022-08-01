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

bool GamePhysics::getIsGenerateInputAI() {
    return isGenerateInputAI;
}

GamePhysics::~GamePhysics() {
    //
}



