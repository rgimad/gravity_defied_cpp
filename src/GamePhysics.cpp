#include "GamePhysics.h"

const int GamePhysics::const175_1_half[] = {114688, 65536, 32768};
int GamePhysics::curentMotoLeague = 0;


GamePhysics::GamePhysics(LevelLoader *levelLoader)
    : motoComponents(6, TimerOrMotoPartOrMenuElem()),
    field_29(6, class_10()),
    field_30(10, TimerOrMotoPartOrMenuElem()),
    levelLoader(levelLoader)
{
    resetSmth(true);
    method_53();
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

void GamePhysics::method_27(int var1, int var2) {
    int var4 = 0, var5 = 0, var6 = 0, var7 = 0;

    for (int i = 0; i < 6; ++i) {
        int var8 = 0;
        switch (i) {
            case 0:
                var5 = 1;
                var4 = 360448;
                var6 = 0;
                var7 = 0;
                break;
            case 1:
                var5 = 0;
                var4 = 98304;
                var6 = 229376;
                var7 = 0;
                break;
            case 2:
                var5 = 0;
                var4 = 360448;
                var6 = -229376;
                var7 = 0;
                var8 = 21626;
                break;
            case 3:
                var5 = 1;
                var4 = 229376;
                var6 = 131072;
                var7 = 196608;
                break;
            case 4:
                var5 = 1;
                var4 = 229376;
                var6 = -131072;
                var7 = 196608;
                break;
            case 5:
                var5 = 2;
                var4 = 294912;
                var6 = 0;
                var7 = 327680;
        }

        field_29[i].reset();
        field_29[i].field_257 = const175_1_half[var5];
        field_29[i].field_258 = var5;
        field_29[i].field_259 = (int) ((int64_t) ((int) (281474976710656LL / (int64_t) var4 >> 16)) * (int64_t) field_14 >> 16);
        field_29[i].motoComponents[index01].xF16 = var1 + var6;
        field_29[i].motoComponents[index01].yF16 = var2 + var7;
        field_29[i].motoComponents[5].xF16 = var1 + var6;
        field_29[i].motoComponents[5].yF16 = var2 + var7;
        field_29[i].field_260 = var8;
    }

    for (int i = 0; i < 10; ++i) {
        field_30[i].setToZeros();
        field_30[i].xF16 = motoParam10;
        field_30[i].angleF16 = field_16;
    }

    field_30[0].yF16 = 229376;
    field_30[1].yF16 = 229376;
    field_30[2].yF16 = 236293;
    field_30[3].yF16 = 236293;
    field_30[4].yF16 = 262144;
    field_30[5].yF16 = 219814;
    field_30[6].yF16 = 219814;
    field_30[7].yF16 = 185363;
    field_30[8].yF16 = 185363;
    field_30[9].yF16 = 327680;
    field_30[5].angleF16 = (int) ((int64_t) field_16 * 45875LL >> 16);
    field_30[6].xF16 = (int) (6553LL * (int64_t) motoParam10 >> 16);
    field_30[5].xF16 = (int) (6553LL * (int64_t) motoParam10 >> 16);
    field_30[9].xF16 = (int) (72089LL * (int64_t) motoParam10 >> 16);
    field_30[8].xF16 = (int) (72089LL * (int64_t) motoParam10 >> 16);
    field_30[7].xF16 = (int) (72089LL * (int64_t) motoParam10 >> 16);
}

void GamePhysics::resetSmth(bool unused) {
    field_44 = 0;
    method_27(levelLoader->method_93(), levelLoader->method_94());
    field_31 = 0;
    field_39 = 0;
    field_35 = false;
    field_36 = false;
    field_68 = false;
    field_69 = false;
    isGenerateInputAI = false;
    field_41 = false;
    field_42 = false;
    levelLoader->gameLevel->method_183(field_29[2].motoComponents[5].xF16 + 98304 - const175_1_half[0], field_29[1].motoComponents[5].xF16 - 98304 + const175_1_half[0]);
}




bool GamePhysics::getIsGenerateInputAI() {
    return isGenerateInputAI;
}

GamePhysics::~GamePhysics() {
    //
}



