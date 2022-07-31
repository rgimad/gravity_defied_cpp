#include "GamePhysics.h"

const int GamePhysics::const175_1_half[] = {114688, 65536, 32768};
static int GamePhysics::curentMotoLeague = 0;


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
    field_80 = new int[][]{{45875}, {32768}, {52428}};
    levelLoader = levelLoader;
    resetSmth(true);
    isGenerateInputAI = false;
    method_53();
    field_35 = false;


}

GamePhysics::~GamePhysics() {
    //
}
