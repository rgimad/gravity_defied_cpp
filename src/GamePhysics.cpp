#include "GamePhysics.h"

#include "LevelLoader.h"
#include "class_10.h"
#include "MathF16.h"
#include <algorithm>

GamePhysics::GamePhysics(LevelLoader *levelLoader) {
    for (int var2 = 0; var2 < 6; ++var2) {
        motoComponents[var2] = std::make_unique<TimerOrMotoPartOrMenuElem>();
    }

    field_44 = 0;
    field_45 = 0;
    field_46 = false;
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

    field_80 = {{45875}, {32768}, {52428}};
    this->levelLoader = levelLoader;
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

void GamePhysics::resetSmth(bool unused) {
    (void)unused;
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
    levelLoader->gameLevel->method_183(field_29[2]->motoComponents[5]->xF16 + 98304 - const175_1_half[0], field_29[1]->motoComponents[5]->xF16 - 98304 + const175_1_half[0]);
}

void GamePhysics::method_26(bool var1) {
    int var2 = (var1 ? 65536 : -65536) << 1;

    for (int var3 = 0; var3 < 6; ++var3) {
        for (int var4 = 0; var4 < 6; ++var4) {
            field_29[var3]->motoComponents[var4]->yF16 += var2;
        }
    }
}

void GamePhysics::method_27(int var1, int var2) {
    if (field_29.empty()) {
        field_29 = std::vector<std::unique_ptr<class_10>>(6);
    }

    if (field_30.empty()) {
        field_30 = std::vector<std::unique_ptr<TimerOrMotoPartOrMenuElem>>(10);
    }

    int var4 = 0;
    int8_t var5 = 0;
    int var6 = 0;
    int var7 = 0;

    int i;
    for (i = 0; i < 6; ++i) {
        short var8 = 0;
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

        if (field_29[i] == nullptr) {
            field_29[i] = std::make_unique<class_10>();
        }

        field_29[i]->reset();
        field_29[i]->field_257 = const175_1_half[var5];
        field_29[i]->field_258 = var5;
        field_29[i]->field_259 = (int) ((int64_t) ((int) (281474976710656L / (int64_t) var4 >> 16)) * (int64_t) field_14 >> 16);
        field_29[i]->motoComponents[index01]->xF16 = var1 + var6;
        field_29[i]->motoComponents[index01]->yF16 = var2 + var7;
        field_29[i]->motoComponents[5]->xF16 = var1 + var6;
        field_29[i]->motoComponents[5]->yF16 = var2 + var7;
        field_29[i]->field_260 = var8;
    }

    for (i = 0; i < 10; ++i) {
        if (field_30[i] == nullptr) {
            field_30[i] = std::make_unique<TimerOrMotoPartOrMenuElem>();
        }

        field_30[i]->setToZeros();
        field_30[i]->xF16 = motoParam10;
        field_30[i]->angleF16 = field_16;
    }

    field_30[0]->yF16 = 229376;
    field_30[1]->yF16 = 229376;
    field_30[2]->yF16 = 236293;
    field_30[3]->yF16 = 236293;
    field_30[4]->yF16 = 262144;
    field_30[5]->yF16 = 219814;
    field_30[6]->yF16 = 219814;
    field_30[7]->yF16 = 185363;
    field_30[8]->yF16 = 185363;
    field_30[9]->yF16 = 327680;
    field_30[5]->angleF16 = (int) ((int64_t) field_16 * 45875L >> 16);
    field_30[6]->xF16 = (int) (6553L * (int64_t) motoParam10 >> 16);
    field_30[5]->xF16 = (int) (6553L * (int64_t) motoParam10 >> 16);
    field_30[9]->xF16 = (int) (72089L * (int64_t) motoParam10 >> 16);
    field_30[8]->xF16 = (int) (72089L * (int64_t) motoParam10 >> 16);
    field_30[7]->xF16 = (int) (72089L * (int64_t) motoParam10 >> 16);
}

void GamePhysics::setRenderMinMaxX(int minX, int maxX) {
    levelLoader->setMinMaxX(minX, maxX);
}

void GamePhysics::processPointerReleased() {
    isInputUp = isInputDown = isInputRight = isInputLeft = false;
}

void GamePhysics::method_30(int var1, int var2) {
    if (!isGenerateInputAI) {
        isInputUp = isInputDown = isInputRight = isInputLeft = false;
        if (var1 > 0) {
            isInputUp = true;
        } else if (var1 < 0) {
            isInputDown = true;
        }

        if (var2 > 0) {
            isInputRight = true;
            return;
        }

        if (var2 < 0) {
            isInputLeft = true;
        }
    }

}

void GamePhysics::enableGenerateInputAI() {
    resetSmth(true);
    isGenerateInputAI = true;
}

void GamePhysics::disableGenerateInputAI() {
    isGenerateInputAI = false;
}

void GamePhysics::setInputFromAI() {
    int var1 = field_29[1]->motoComponents[index01]->xF16 - field_29[2]->motoComponents[index01]->xF16;
    int var2 = field_29[1]->motoComponents[index01]->yF16 - field_29[2]->motoComponents[index01]->yF16;
    int var3 = getSmthLikeMaxAbs(var1, var2);
    var2 = (int) (((int64_t) var2 << 32) / (int64_t) var3 >> 16);
    isInputBreak = false;
    if (var2 < 0) {
        isInputBack = true;
        isInputForward = false;
    } else if (var2 > 0) {
        isInputForward = true;
        isInputBack = false;
    }

    bool var4;
    if ((!(var4 = (field_29[2]->motoComponents[index01]->yF16 - field_29[0]->motoComponents[index01]->yF16 > 0 ? 1 : -1) * (field_29[2]->motoComponents[index01]->field_382 - field_29[0]->motoComponents[index01]->field_382 > 0 ? 1 : -1) > 0) || !isInputForward) && (var4 || !isInputBack)) {
        isInputAcceleration = false;
    } else {
        isInputAcceleration = true;
    }
}

void GamePhysics::method_35() {
    if (!field_35) {
        int var1 = field_29[1]->motoComponents[index01]->xF16 - field_29[2]->motoComponents[index01]->xF16;
        int var2 = field_29[1]->motoComponents[index01]->yF16 - field_29[2]->motoComponents[index01]->yF16;
        int var3 = getSmthLikeMaxAbs(var1, var2);
        var1 = (int) (((int64_t) var1 << 32) / (int64_t) var3 >> 16);
        var2 = (int) (((int64_t) var2 << 32) / (int64_t) var3 >> 16);
        if (isInputAcceleration && field_31 >= -motoParam4) {
            field_31 -= motoParam5;
        }

        if (isInputBreak) {
            field_31 = 0;
            field_29[1]->motoComponents[index01]->field_384 = (int) ((int64_t) field_29[1]->motoComponents[index01]->field_384 * (int64_t) (65536 - motoParam6) >> 16);
            field_29[2]->motoComponents[index01]->field_384 = (int) ((int64_t) field_29[2]->motoComponents[index01]->field_384 * (int64_t) (65536 - motoParam6) >> 16);
            if (field_29[1]->motoComponents[index01]->field_384 < 6553) {
                field_29[1]->motoComponents[index01]->field_384 = 0;
            }

            if (field_29[2]->motoComponents[index01]->field_384 < 6553) {
                field_29[2]->motoComponents[index01]->field_384 = 0;
            }
        }

        field_29[0]->field_259 = (int) (11915L * (int64_t) field_14 >> 16);
        field_29[0]->field_259 = (int) (11915L * (int64_t) field_14 >> 16);
        field_29[4]->field_259 = (int) (18724L * (int64_t) field_14 >> 16);
        field_29[3]->field_259 = (int) (18724L * (int64_t) field_14 >> 16);
        field_29[1]->field_259 = (int) (43690L * (int64_t) field_14 >> 16);
        field_29[2]->field_259 = (int) (11915L * (int64_t) field_14 >> 16);
        field_29[5]->field_259 = (int) (14563L * (int64_t) field_14 >> 16);
        if (isInputBack) {
            field_29[0]->field_259 = (int) (18724L * (int64_t) field_14 >> 16);
            field_29[4]->field_259 = (int) (14563L * (int64_t) field_14 >> 16);
            field_29[3]->field_259 = (int) (18724L * (int64_t) field_14 >> 16);
            field_29[1]->field_259 = (int) (43690L * (int64_t) field_14 >> 16);
            field_29[2]->field_259 = (int) (10082L * (int64_t) field_14 >> 16);
        } else if (isInputForward) {
            field_29[0]->field_259 = (int) (18724L * (int64_t) field_14 >> 16);
            field_29[4]->field_259 = (int) (18724L * (int64_t) field_14 >> 16);
            field_29[3]->field_259 = (int) (14563L * (int64_t) field_14 >> 16);
            field_29[1]->field_259 = (int) (26214L * (int64_t) field_14 >> 16);
            field_29[2]->field_259 = (int) (11915L * (int64_t) field_14 >> 16);
        }

        if (isInputBack || isInputForward) {
            int var4 = -var2;
            TimerOrMotoPartOrMenuElem *var10000;
            int var6;
            int var7;
            int var8;
            int var9;
            int var10;
            int var11;
            if (isInputBack && field_39 > -motoParam9) {
                var6 = 65536;
                if (field_39 < 0) {
                    var6 = (int) (((int64_t) (motoParam9 - (field_39 < 0 ? -field_39 : field_39)) << 32) / (int64_t) motoParam9 >> 16);
                }

                var7 = (int) ((int64_t) motoParam8 * (int64_t) var6 >> 16);
                var8 = (int) ((int64_t) var4 * (int64_t) var7 >> 16);
                var9 = (int) ((int64_t) var1 * (int64_t) var7 >> 16);
                var10 = (int) ((int64_t) var1 * (int64_t) var7 >> 16);
                var11 = (int) ((int64_t) var2 * (int64_t) var7 >> 16);
                if (field_37 > 32768) {
                    field_37 = field_37 - 1638 < 0 ? 0 : field_37 - 1638;
                } else {
                    field_37 = field_37 - 3276 < 0 ? 0 : field_37 - 3276;
                }

                var10000 = field_29[4]->motoComponents[index01].get();
                var10000->field_382 -= var8;
                var10000 = field_29[4]->motoComponents[index01].get();
                var10000->field_383 -= var9;
                var10000 = field_29[3]->motoComponents[index01].get();
                var10000->field_382 += var8;
                var10000 = field_29[3]->motoComponents[index01].get();
                var10000->field_383 += var9;
                var10000 = field_29[5]->motoComponents[index01].get();
                var10000->field_382 -= var10;
                var10000 = field_29[5]->motoComponents[index01].get();
                var10000->field_383 -= var11;
            }

            if (isInputForward && field_39 < motoParam9) {
                var6 = 65536;
                if (field_39 > 0) {
                    var6 = (int) (((int64_t) (motoParam9 - field_39) << 32) / (int64_t) motoParam9 >> 16);
                }

                var7 = (int) ((int64_t) motoParam8 * (int64_t) var6 >> 16);
                var8 = (int) ((int64_t) var4 * (int64_t) var7 >> 16);
                var9 = (int) ((int64_t) var1 * (int64_t) var7 >> 16);
                var10 = (int) ((int64_t) var1 * (int64_t) var7 >> 16);
                var11 = (int) ((int64_t) var2 * (int64_t) var7 >> 16);
                if (field_37 > 32768) {
                    field_37 = field_37 + 1638 < 65536 ? field_37 + 1638 : 65536;
                } else {
                    field_37 = field_37 + 3276 < 65536 ? field_37 + 3276 : 65536;
                }

                var10000 = field_29[4]->motoComponents[index01].get();
                var10000->field_382 += var8;
                var10000 = field_29[4]->motoComponents[index01].get();
                var10000->field_383 += var9;
                var10000 = field_29[3]->motoComponents[index01].get();
                var10000->field_382 -= var8;
                var10000 = field_29[3]->motoComponents[index01].get();
                var10000->field_383 -= var9;
                var10000 = field_29[5]->motoComponents[index01].get();
                var10000->field_382 += var10;
                var10000 = field_29[5]->motoComponents[index01].get();
                var10000->field_383 += var11;
            }

            return;
        }

        if (field_37 < 26214) {
            field_37 += 3276;
            return;
        }

        if (field_37 > 39321) {
            field_37 -= 3276;
            return;
        }

        field_37 = 32768;
    }

}

int GamePhysics::updatePhysics() {
    isInputAcceleration = isInputUp;
    isInputBreak = isInputDown;
    isInputBack = isInputLeft;
    isInputForward = isInputRight;
    if (isGenerateInputAI) {
        setInputFromAI();
    }

    GameCanvas::method_151();
    method_35();
    int var1;
    if ((var1 = method_39(field_7)) != 5 && !field_36) {
        if (field_35) {
            return 3;
        } else if (isTrackStarted()) {
            field_69 = false;
            return 4;
        } else {
            return var1;
        }
    } else {
        return 5;
    }
}

bool GamePhysics::isTrackStarted() {
    return field_29[1]->motoComponents[index01]->xF16 < levelLoader->method_92();
}

bool GamePhysics::method_38() {
    return field_29[1]->motoComponents[index10]->xF16 > levelLoader->method_91() || field_29[2]->motoComponents[index10]->xF16 > levelLoader->method_91();
}

int GamePhysics::method_39(int var1) {
    bool var2 = field_68;
    int var3 = 0;
    int var4 = var1;

    label77:
    do {
        int var5;
        while (var3 < var1) {
            method_45(var4 - var3);
            if (!var2 && method_38()) {
                var5 = 3;
            } else {
                var5 = method_46(index10);
            }

            if (!var2 && field_68) {
                if (var5 != 3) {
                    return 2;
                }

                return 1;
            }

            if (var5 == 0) {
                goto label77;
            }

            if (var5 == 3) {
                field_68 = true;
                var4 = (var3 + var4) >> 1;
            } else {
                int var6;
                if (var5 == 1) {
                    do {
                        method_47(index10);
                        if ((var6 = method_46(index10)) == 0) {
                            return 5;
                        }
                    } while (var6 != 2);
                }

                var3 = var4;
                var4 = var1;
                index01 = index01 == 1 ? 0 : 1;
                index10 = index10 == 1 ? 0 : 1;
            }
        }

        if ((var5 = (int) ((int64_t) (field_29[1]->motoComponents[index01]->xF16 - field_29[2]->motoComponents[index01]->xF16) * (int64_t) (field_29[1]->motoComponents[index01]->xF16 - field_29[2]->motoComponents[index01]->xF16) >> 16) + (int) ((int64_t) (field_29[1]->motoComponents[index01]->yF16 - field_29[2]->motoComponents[index01]->yF16) * (int64_t) (field_29[1]->motoComponents[index01]->yF16 - field_29[2]->motoComponents[index01]->yF16) >> 16)) < 983040) {
            field_35 = true;
        }

        if (var5 > 4587520) {
            field_35 = true;
        }

        return 0;
    } while (((var4 = (var3 + var4) >> 1) - var3 < 0 ? -(var4 - var3) : var4 - var3) >= 65);

    return 5;
}

void GamePhysics::method_40(int var1) {
    TimerOrMotoPartOrMenuElem *var3;
    int var4;
    for (var4 = 0; var4 < 6; ++var4) {
        class_10 *var2 = field_29[var4].get();
        var3 = var2->motoComponents[var1].get();
        var3->field_385 = 0;

        var3->field_386 = 0;
        var3->field_387 = 0;
        var3->field_386 -= (int) (((int64_t) field_8 << 32) / (int64_t) var2->field_259 >> 16);
    }

    if (!field_35) {
        method_42(field_29[0].get(), field_30[1].get(), field_29[2].get(), var1, 65536);
        method_42(field_29[0].get(), field_30[0].get(), field_29[1].get(), var1, 65536);
        method_42(field_29[2].get(), field_30[6].get(), field_29[4].get(), var1, 131072);
        method_42(field_29[1].get(), field_30[5].get(), field_29[3].get(), var1, 131072);
    }

    method_42(field_29[0].get(), field_30[2].get(), field_29[3].get(), var1, 65536);
    method_42(field_29[0].get(), field_30[3].get(), field_29[4].get(), var1, 65536);
    method_42(field_29[3].get(), field_30[4].get(), field_29[4].get(), var1, 65536);
    method_42(field_29[5].get(), field_30[8].get(), field_29[3].get(), var1, 65536);
    method_42(field_29[5].get(), field_30[7].get(), field_29[4].get(), var1, 65536);
    method_42(field_29[5].get(), field_30[9].get(), field_29[0].get(), var1, 65536);
    var3 = field_29[2]->motoComponents[var1].get();
    field_31 = (int) ((int64_t) field_31 * (int64_t) (65536 - field_19) >> 16);
    var3->field_387 = field_31;
    if (var3->field_384 > motoParam3) {
        var3->field_384 = motoParam3;
    }

    if (var3->field_384 < -motoParam3) {
        var3->field_384 = -motoParam3;
    }

    var4 = 0;
    int var5 = 0;

    int var6;
    for (var6 = 0; var6 < 6; ++var6) {
        var4 += field_29[var6]->motoComponents[var1]->field_382;
        var5 += field_29[var6]->motoComponents[var1]->field_383;
    }

    var4 = (int) (((int64_t) var4 << 32) / 393216L >> 16);
    var5 = (int) (((int64_t) var5 << 32) / 393216L >> 16);
    int var10 = 0;

    int var11;
    for (var11 = 0; var11 < 6; ++var11) {
        var6 = field_29[var11]->motoComponents[var1]->field_382 - var4;
        int var7 = field_29[var11]->motoComponents[var1]->field_383 - var5;
        if ((var10 = getSmthLikeMaxAbs(var6, var7)) > 1966080) {
            int var8 = (int) (((int64_t) var6 << 32) / (int64_t) var10 >> 16);
            int var9 = (int) (((int64_t) var7 << 32) / (int64_t) var10 >> 16);
            field_29[var11]->motoComponents[var1]->field_382 -= var8;
            field_29[var11]->motoComponents[var1]->field_383 -= var9;
        }
    }

    var11 = field_29[2]->motoComponents[var1]->yF16 - field_29[0]->motoComponents[var1]->yF16 >= 0 ? 1 : -1;
    int var12 = field_29[2]->motoComponents[var1]->field_382 - field_29[0]->motoComponents[var1]->field_382 >= 0 ? 1 : -1;
    if (var11 * var12 > 0) {
        field_39 = var10;
    } else {
        field_39 = -var10;
    }
}

int GamePhysics::getSmthLikeMaxAbs(int xF16, int yF16) {
    int absXF16 = xF16 < 0 ? -xF16 : xF16;
    int absYF16;
    int maxAbs;
    int minAbs;
    if ((absYF16 = yF16 < 0 ? -yF16 : yF16) >= absXF16) {
        maxAbs = absYF16;
        minAbs = absXF16;
    } else {
        maxAbs = absXF16;
        minAbs = absYF16;
    }

    return (int) (64448L * (int64_t) maxAbs >> 16) + (int) (28224L * (int64_t) minAbs >> 16);
}

void GamePhysics::method_42(class_10 *var1, TimerOrMotoPartOrMenuElem *var2, class_10 *var3, int var4, int var5) {
    TimerOrMotoPartOrMenuElem *var6 = var1->motoComponents[var4].get();
    TimerOrMotoPartOrMenuElem *var7 = var3->motoComponents[var4].get();
    int var8 = var6->xF16 - var7->xF16;
    int var9 = var6->yF16 - var7->yF16;
    int var10;
    if (((var10 = getSmthLikeMaxAbs(var8, var9)) < 0 ? -var10 : var10) >= 3) {
        var8 = (int) (((int64_t) var8 << 32) / (int64_t) var10 >> 16);
        var9 = (int) (((int64_t) var9 << 32) / (int64_t) var10 >> 16);
        int var11 = var10 - var2->yF16;
        int var12 = (int) ((int64_t) var8 * (int64_t) ((int) ((int64_t) var11 * (int64_t) var2->xF16 >> 16)) >> 16);
        int var13 = (int) ((int64_t) var9 * (int64_t) ((int) ((int64_t) var11 * (int64_t) var2->xF16 >> 16)) >> 16);
        int var14 = var6->field_382 - var7->field_382;
        int var15 = var6->field_383 - var7->field_383;
        int var16 = (int) ((int64_t) ((int) ((int64_t) var8 * (int64_t) var14 >> 16) + (int) ((int64_t) var9 * (int64_t) var15 >> 16)) * (int64_t) var2->angleF16 >> 16);
        var12 += (int) ((int64_t) var8 * (int64_t) var16 >> 16);
        var13 += (int) ((int64_t) var9 * (int64_t) var16 >> 16);
        var12 = (int) ((int64_t) var12 * (int64_t) var5 >> 16);
        var13 = (int) ((int64_t) var13 * (int64_t) var5 >> 16);
        var6->field_385 -= var12;
        var6->field_386 -= var13;
        var7->field_385 += var12;
        var7->field_386 += var13;
    }

}

void GamePhysics::method_43(int var1, int var2, int var3) {
    for (int var7 = 0; var7 < 6; ++var7) {
        TimerOrMotoPartOrMenuElem *var4 = field_29[var7]->motoComponents[var1].get();
        TimerOrMotoPartOrMenuElem *var5;
        (var5 = field_29[var7]->motoComponents[var2].get())->xF16 = (int) ((int64_t) var4->field_382 * (int64_t) var3 >> 16);
        var5->yF16 = (int) ((int64_t) var4->field_383 * (int64_t) var3 >> 16);
        int var6 = (int) ((int64_t) var3 * (int64_t) field_29[var7]->field_259 >> 16);
        var5->field_382 = (int) ((int64_t) var4->field_385 * (int64_t) var6 >> 16);
        var5->field_383 = (int) ((int64_t) var4->field_386 * (int64_t) var6 >> 16);
    }
}

void GamePhysics::method_44(int var1, int var2, int var3) {
    for (int var7 = 0; var7 < 6; ++var7) {
        TimerOrMotoPartOrMenuElem *var4 = field_29[var7]->motoComponents[var1].get();
        TimerOrMotoPartOrMenuElem *var5 = field_29[var7]->motoComponents[var2].get();
        TimerOrMotoPartOrMenuElem *var6 = field_29[var7]->motoComponents[var3].get();
        var4->xF16 = var5->xF16 + (var6->xF16 >> 1);
        var4->yF16 = var5->yF16 + (var6->yF16 >> 1);
        var4->field_382 = var5->field_382 + (var6->field_382 >> 1);
        var4->field_383 = var5->field_383 + (var6->field_383 >> 1);
    }
}

void GamePhysics::method_45(int var1) {
    method_40(index01);
    method_43(index01, 2, var1);
    method_44(4, index01, 2);
    method_40(4);
    method_43(4, 3, var1 >> 1);
    method_44(4, index01, 3);
    method_44(index10, index01, 2);
    method_44(index10, index10, 3);

    for (int var4 = 1; var4 <= 2; ++var4) {
        TimerOrMotoPartOrMenuElem *var2 = field_29[var4]->motoComponents[index01].get();
        TimerOrMotoPartOrMenuElem *var3;
        (var3 = field_29[var4]->motoComponents[index10].get())->angleF16 = var2->angleF16 + (int) ((int64_t) var1 * (int64_t) var2->field_384 >> 16);
        var3->field_384 = var2->field_384 + (int) ((int64_t) var1 * (int64_t) ((int) ((int64_t) field_29[var4]->field_260 * (int64_t) var2->field_387 >> 16)) >> 16);
    }
}

int GamePhysics::method_46(int var1) {
    int8_t var2 = 2;
    int var4 = std::max({field_29[1]->motoComponents[var1]->xF16, field_29[2]->motoComponents[var1]->xF16, field_29[5]->motoComponents[var1]->xF16});
    int var5 = std::min({field_29[1]->motoComponents[var1]->xF16, field_29[2]->motoComponents[var1]->xF16, field_29[5]->motoComponents[var1]->xF16});
    levelLoader->method_100(var5 - const175_1_half[0], var4 + const175_1_half[0], field_29[5]->motoComponents[var1]->yF16);
    int var6 = field_29[1]->motoComponents[var1]->xF16 - field_29[2]->motoComponents[var1]->xF16;
    int var7 = field_29[1]->motoComponents[var1]->yF16 - field_29[2]->motoComponents[var1]->yF16;
    int var8 = getSmthLikeMaxAbs(var6, var7);
    var6 = (int) (((int64_t) var6 << 32) / (int64_t) var8 >> 16);
    int var9 = -((int) (((int64_t) var7 << 32) / (int64_t) var8 >> 16));
    int var10 = var6;

    for (int var11 = 0; var11 < 6; ++var11) {
        if (var11 != 4 && var11 != 3) {
            TimerOrMotoPartOrMenuElem *var3 = field_29[var11]->motoComponents[var1].get();
            if (var11 == 0) {
                var3->xF16 += (int) ((int64_t) var9 * 65536L >> 16);
                var3->yF16 += (int) ((int64_t) var10 * 65536L >> 16);
            }

            int var12 = levelLoader->method_101(var3, field_29[var11]->field_258);
            if (var11 == 0) {
                var3->xF16 -= (int) ((int64_t) var9 * 65536L >> 16);
                var3->yF16 -= (int) ((int64_t) var10 * 65536L >> 16);
            }

            field_33 = levelLoader->field_137;
            field_34 = levelLoader->field_138;
            if (var11 == 5 && var12 != 2) {
                field_36 = true;
            }

            if (var11 == 1 && var12 != 2) {
                field_69 = true;
            }

            if (var12 == 1) {
                field_28 = var11;
                var2 = 1;
            } else if (var12 == 0) {
                field_28 = var11;
                var2 = 0;
                break;
            }
        }
    }

    return var2;
}

void GamePhysics::method_47(int var1) {
    class_10 *var2;
    TimerOrMotoPartOrMenuElem *var3;
    TimerOrMotoPartOrMenuElem *var10000 = var3 = (var2 = field_29[field_28].get())->motoComponents[var1].get();
    var10000->xF16 += (int) ((int64_t) field_33 * 3276L >> 16);
    var3->yF16 += (int) ((int64_t) field_34 * 3276L >> 16);
    int var4;
    int var5;
    int var6;
    int var7;
    int var8;
    if (isInputBreak && (field_28 == 2 || field_28 == 1) && var3->field_384 < 6553) {
        var4 = field_9 - motoParam7;
        var5 = 13107;
        var6 = 39321;
        var7 = 26214 - motoParam7;
        var8 = 26214 - motoParam7;
    } else {
        var4 = field_9;
        var5 = field_10;
        var6 = field_11;
        var7 = motoParam1;
        var8 = motoParam2;
    }

    int var9 = getSmthLikeMaxAbs(field_33, field_34);
    field_33 = (int) (((int64_t) field_33 << 32) / (int64_t) var9 >> 16);
    field_34 = (int) (((int64_t) field_34 << 32) / (int64_t) var9 >> 16);
    int var10 = var3->field_382;
    int var11 = var3->field_383;
    int var12 = -((int) ((int64_t) var10 * (int64_t) field_33 >> 16) + (int) ((int64_t) var11 * (int64_t) field_34 >> 16));
    int var13 = -((int) ((int64_t) var10 * (int64_t) (-field_34) >> 16) + (int) ((int64_t) var11 * (int64_t) field_33 >> 16));
    int var14 = (int) ((int64_t) var4 * (int64_t) var3->field_384 >> 16) - (int) ((int64_t) var5 * (int64_t) ((int) (((int64_t) var13 << 32) / (int64_t) var2->field_257 >> 16)) >> 16);
    int var15 = (int) ((int64_t) var7 * (int64_t) var13 >> 16) - (int) ((int64_t) var6 * (int64_t) ((int) ((int64_t) var3->field_384 * (int64_t) var2->field_257 >> 16)) >> 16);
    int var16 = -((int) ((int64_t) var8 * (int64_t) var12 >> 16));
    int var17 = (int) ((int64_t) (-var15) * (int64_t) (-field_34) >> 16);
    int var18 = (int) ((int64_t) (-var15) * (int64_t) field_33 >> 16);
    int var19 = (int) ((int64_t) (-var16) * (int64_t) field_33 >> 16);
    int var20 = (int) ((int64_t) (-var16) * (int64_t) field_34 >> 16);
    var3->field_384 = var14;
    var3->field_382 = var17 + var19;
    var3->field_383 = var18 + var20;
}

void GamePhysics::setEnableLookAhead(bool value) {
    isEnableLookAhead = value;
}

void GamePhysics::setMinimalScreenWH(int minWH) {
    field_73 = (int) (((int64_t) ((int) (655360L * (int64_t) (minWH << 16) >> 16)) << 32) / 8388608L >> 16);
}

int GamePhysics::getCamPosX() {
    if (isEnableLookAhead) {
        camShiftX = (int) (((int64_t) motoComponents[0]->field_382 << 32) / 1572864L >> 16) + (int) ((int64_t) camShiftX * 57344L >> 16);
    } else {
        camShiftX = 0;
    }

    // camShiftX = clamp(camShiftX, -field_73, field_73);
    camShiftX = camShiftX < field_73 ? camShiftX : field_73;
    camShiftX = camShiftX < -field_73 ? -field_73 : camShiftX;
    return (motoComponents[0]->xF16 + camShiftX) << 2 >> 16;
}

int GamePhysics::getCamPosY() {
    if (isEnableLookAhead) {
        camShiftY = (int) (((int64_t) motoComponents[0]->field_383 << 32) / 1572864L >> 16) + (int) ((int64_t) camShiftY * 57344L >> 16);
    } else {
        camShiftY = 0;
    }

    camShiftY = camShiftY < field_73 ? camShiftY : field_73;
    camShiftY = camShiftY < -field_73 ? -field_73 : camShiftY;
    return (motoComponents[0]->yF16 + camShiftY) << 2 >> 16;
}

int GamePhysics::method_52() {
    int var1 = motoComponents[1]->xF16 < motoComponents[2]->xF16 ? motoComponents[2]->xF16 : motoComponents[1]->xF16;
    return field_35 ? levelLoader->method_95(motoComponents[0]->xF16) : levelLoader->method_95(var1);
}

void GamePhysics::method_53() {
    // synchronized (field_29) {
        for (int var2 = 0; var2 < 6; ++var2) {
            field_29[var2]->motoComponents[5]->xF16 = field_29[var2]->motoComponents[index01]->xF16;
            field_29[var2]->motoComponents[5]->yF16 = field_29[var2]->motoComponents[index01]->yF16;
            field_29[var2]->motoComponents[5]->angleF16 = field_29[var2]->motoComponents[index01]->angleF16;
        }

        field_29[0]->motoComponents[5]->field_382 = field_29[0]->motoComponents[index01]->field_382;
        field_29[0]->motoComponents[5]->field_383 = field_29[0]->motoComponents[index01]->field_383;
        field_29[2]->motoComponents[5]->field_384 = field_29[2]->motoComponents[index01]->field_384;
    // }
}

void GamePhysics::setMotoComponents() {
    // synchronized (field_29) {
        for (int i = 0; i < 6; ++i) {
            motoComponents[i]->xF16 = field_29[i]->motoComponents[5]->xF16;
            motoComponents[i]->yF16 = field_29[i]->motoComponents[5]->yF16;
            motoComponents[i]->angleF16 = field_29[i]->motoComponents[5]->angleF16;
        }

        motoComponents[0]->field_382 = field_29[0]->motoComponents[5]->field_382;
        motoComponents[0]->field_383 = field_29[0]->motoComponents[5]->field_383;
        motoComponents[2]->field_384 = field_29[2]->motoComponents[5]->field_384;
    // }
}

void GamePhysics::renderEngine(GameCanvas *gameCanvas, int var2, int var3) {
    int engineAngle4F16 = MathF16::atan2F16(motoComponents[0]->xF16 - motoComponents[3]->xF16, motoComponents[0]->yF16 - motoComponents[3]->yF16);
    int fenderAngle4F16 = MathF16::atan2F16(motoComponents[0]->xF16 - motoComponents[4]->xF16, motoComponents[0]->yF16 - motoComponents[4]->yF16);
    int engineXF16 = (motoComponents[0]->xF16 >> 1) + (motoComponents[3]->xF16 >> 1);
    int engineYF16 = (motoComponents[0]->yF16 >> 1) + (motoComponents[3]->yF16 >> 1);
    int fenderXF16 = (motoComponents[0]->xF16 >> 1) + (motoComponents[4]->xF16 >> 1);
    int fenderYF16 = (motoComponents[0]->yF16 >> 1) + (motoComponents[4]->yF16 >> 1);
    int var10 = -var3;
    engineXF16 += (int) ((int64_t) var10 * 65536L >> 16) - (int) ((int64_t) var2 * 32768L >> 16);
    engineYF16 += (int) ((int64_t) var2 * 65536L >> 16) - (int) ((int64_t) var3 * 32768L >> 16);
    fenderXF16 += (int) ((int64_t) var10 * 65536L >> 16) - (int) ((int64_t) var2 * 117964L >> 16);
    fenderYF16 += (int) ((int64_t) var2 * 65536L >> 16) - (int) ((int64_t) var3 * 131072L >> 16);
    gameCanvas->renderFender(fenderXF16 << 2 >> 16, fenderYF16 << 2 >> 16, fenderAngle4F16);
    gameCanvas->renderEngine(engineXF16 << 2 >> 16, engineYF16 << 2 >> 16, engineAngle4F16);
}

void GamePhysics::renderMotoFork(GameCanvas *canvas) {
    canvas->setColor(128, 128, 128);
    canvas->drawLineF16(motoComponents[3]->xF16, motoComponents[3]->yF16, motoComponents[1]->xF16, motoComponents[1]->yF16);
}

void GamePhysics::renderWheelTires(GameCanvas *canvas) {
    int8_t backWheelIsThin = 1;
    int8_t forwardWheelIsThin = 1;
    switch (curentMotoLeague) {
        case 1:
            backWheelIsThin = 0;
            break;
        case 2:
        case 3:
            forwardWheelIsThin = 0;
            backWheelIsThin = 0;
    }

    // back wheel
    canvas->drawWheelTires(motoComponents[2]->xF16 << 2 >> 16, motoComponents[2]->yF16 << 2 >> 16, backWheelIsThin);
    // forward wheel
    canvas->drawWheelTires(motoComponents[1]->xF16 << 2 >> 16, motoComponents[1]->yF16 << 2 >> 16, forwardWheelIsThin);
}

void GamePhysics::renderWheelSpokes(GameCanvas *gameCanvas) {
    int var2;
    int xxxF16 = (int) ((int64_t) (var2 = field_29[1]->field_257) * 58982L >> 16);
    int yyyF16 = (int) ((int64_t) var2 * 45875L >> 16);
    gameCanvas->setColor(0, 0, 0);
    if (Micro::isInGameMenu) {
        gameCanvas->drawCircle(motoComponents[1]->xF16 << 2 >> 16, motoComponents[1]->yF16 << 2 >> 16, (var2 + var2) << 2 >> 16);
        gameCanvas->drawCircle(motoComponents[1]->xF16 << 2 >> 16, motoComponents[1]->yF16 << 2 >> 16, (xxxF16 + xxxF16) << 2 >> 16);
        gameCanvas->drawCircle(motoComponents[2]->xF16 << 2 >> 16, motoComponents[2]->yF16 << 2 >> 16, (var2 + var2) << 2 >> 16);
        gameCanvas->drawCircle(motoComponents[2]->xF16 << 2 >> 16, motoComponents[2]->yF16 << 2 >> 16, (yyyF16 + yyyF16) << 2 >> 16);
    }

    int8_t var6 = 0;
    int angle;
    int cosF16 = MathF16::cosF16(angle = motoComponents[1]->angleF16);
    int sinF16 = MathF16::sinF16(angle);
    int dxF16 = (int) ((int64_t) cosF16 * (int64_t) xxxF16 >> 16) + (int) ((int64_t) (-sinF16) * (int64_t) var6 >> 16);
    int dyF16 = (int) ((int64_t) sinF16 * (int64_t) xxxF16 >> 16) + (int) ((int64_t) cosF16 * (int64_t) var6 >> 16);
    angle = 82354;
    cosF16 = MathF16::cosF16(82354);
    sinF16 = MathF16::sinF16(angle);

    int var10;
    int i;
    for (i = 0; i < 5; ++i) {
        // forward wheel spokes
        gameCanvas->drawLineF16(motoComponents[1]->xF16, motoComponents[1]->yF16, motoComponents[1]->xF16 + dxF16, motoComponents[1]->yF16 + dyF16);
        var10 = dxF16;
        dxF16 = (int) ((int64_t) cosF16 * (int64_t) dxF16 >> 16) + (int) ((int64_t) (-sinF16) * (int64_t) dyF16 >> 16);
        dyF16 = (int) ((int64_t) sinF16 * (int64_t) var10 >> 16) + (int) ((int64_t) cosF16 * (int64_t) dyF16 >> 16);
    }

    var6 = 0;
    cosF16 = MathF16::cosF16(angle = motoComponents[2]->angleF16);
    sinF16 = MathF16::sinF16(angle);
    dxF16 = (int) ((int64_t) cosF16 * (int64_t) xxxF16 >> 16) + (int) ((int64_t) (-sinF16) * (int64_t) var6 >> 16);
    dyF16 = (int) ((int64_t) sinF16 * (int64_t) xxxF16 >> 16) + (int) ((int64_t) cosF16 * (int64_t) var6 >> 16);
    angle = 82354;
    cosF16 = MathF16::cosF16(82354);
    sinF16 = MathF16::sinF16(angle);

    for (i = 0; i < 5; ++i) {
        // back wheel spokes
        gameCanvas->drawLineF16(motoComponents[2]->xF16, motoComponents[2]->yF16, motoComponents[2]->xF16 + dxF16, motoComponents[2]->yF16 + dyF16);
        var10 = dxF16;
        dxF16 = (int) ((int64_t) cosF16 * (int64_t) dxF16 >> 16) + (int) ((int64_t) (-sinF16) * (int64_t) dyF16 >> 16);
        dyF16 = (int) ((int64_t) sinF16 * (int64_t) var10 >> 16) + (int) ((int64_t) cosF16 * (int64_t) dyF16 >> 16);
    }

    if (curentMotoLeague > 0) {
        gameCanvas->setColor(255, 0, 0);
        if (curentMotoLeague > 2) {
            gameCanvas->setColor(100, 100, 255);
        }

        gameCanvas->drawCircle(motoComponents[2]->xF16 << 2 >> 16, motoComponents[2]->yF16 << 2 >> 16, 4);
        gameCanvas->drawCircle(motoComponents[1]->xF16 << 2 >> 16, motoComponents[1]->yF16 << 2 >> 16, 4);
    }
}

void GamePhysics::renderSmth(GameCanvas *gameCanvas, int var2, int var3, int var4, int var5) {
    int8_t var6 = 0;
    int var7 = 65536;
    int var8 = motoComponents[0]->xF16;
    int var9 = motoComponents[0]->yF16;
    int x6F16 = 0;
    int y6F16 = 0;
    int xF16 = 0;
    int yF16 = 0;
    int var14 = 0;
    int var15 = 0;
    int x2F16 = 0;
    int y2F16 = 0;
    int x3F16 = 0;
    int y3F16 = 0;
    int x4F16 = 0;
    int y4F16 = 0;
    int circleXF16 = 0;
    int circleYF16 = 0;
    int x5F16 = 0;
    int y5F16 = 0;
    std::vector<std::vector<int>> var27, var28, var29;
    if (field_46) {
        if (field_37 < 32768) {
            var28 = hardcodedArr2;
            var29 = hardcodedArr1;
            var7 = (int) ((int64_t) field_37 * 131072L >> 16);
        } else if (field_37 > 32768) {
            var6 = 1;
            var28 = hardcodedArr1;
            var29 = hardcodedArr3;
            var7 = (int) ((int64_t) (field_37 - 32768) * 131072L >> 16);
        } else {
            var27 = hardcodedArr1;
        }
    } else if (field_37 < 32768) {
        var28 = hardcodedArr5;
        var29 = hardcodedArr4;
        var7 = (int) ((int64_t) field_37 * 131072L >> 16);
    } else if (field_37 > 32768) {
        var6 = 1;
        var28 = hardcodedArr4;
        var29 = hardcodedArr6;
        var7 = (int) ((int64_t) (field_37 - 32768) * 131072L >> 16);
    } else {
        var27 = hardcodedArr4;
    }

    for (std::size_t var30 = 0; var30 < hardcodedArr1.size(); ++var30) {
        int var31;
        int var32;
        if (!var28.empty()) {
            var32 = (int) ((int64_t) var28[var30][0] * (int64_t) (65536 - var7) >> 16) + (int) ((int64_t) var29[var30][0] * (int64_t) var7 >> 16);
            var31 = (int) ((int64_t) var28[var30][1] * (int64_t) (65536 - var7) >> 16) + (int) ((int64_t) var29[var30][1] * (int64_t) var7 >> 16);
        } else {
            var32 = var27[var30][0];
            var31 = var27[var30][1];
        }

        int xxF16 = var8 + (int) ((int64_t) var4 * (int64_t) var32 >> 16) + (int) ((int64_t) var2 * (int64_t) var31 >> 16);
        int yyF16 = var9 + (int) ((int64_t) var5 * (int64_t) var32 >> 16) + (int) ((int64_t) var3 * (int64_t) var31 >> 16);
        switch (var30) {
            case 0:
                x2F16 = xxF16;
                y2F16 = yyF16;
                break;
            case 1:
                x3F16 = xxF16;
                y3F16 = yyF16;
                break;
            case 2:
                x4F16 = xxF16;
                y4F16 = yyF16;
                break;
            case 3:
                circleXF16 = xxF16;
                circleYF16 = yyF16;
                break;
            case 4:
                x5F16 = xxF16;
                y5F16 = yyF16;
                break;
            case 5:
                xF16 = xxF16;
                yF16 = yyF16;
                break;
            case 6:
                var14 = xxF16;
                var15 = yyF16;
                break;
            case 7:
                x6F16 = xxF16;
                y6F16 = yyF16;
        }
    }

    int var26 = (int) ((int64_t) field_80[var6][0] * (int64_t) (65536 - var7) >> 16) + (int) ((int64_t) field_80[var6 + 1][0] * (int64_t) var7 >> 16);
    if (field_46) {
        gameCanvas->renderBodyPart(xF16 << 2, yF16 << 2, x2F16 << 2, y2F16 << 2, 1);
        gameCanvas->renderBodyPart(x2F16 << 2, y2F16 << 2, x3F16 << 2, y3F16 << 2, 1);
        gameCanvas->renderBodyPart(x3F16 << 2, y3F16 << 2, x4F16 << 2, y4F16 << 2, 2, var26);
        gameCanvas->renderBodyPart(x4F16 << 2, y4F16 << 2, x5F16 << 2, y5F16 << 2, 0);
        int var30 = MathF16::atan2F16(var2, var3);
        if (field_37 > 32768) {
            var30 += 20588;
        }

        gameCanvas->drawHelmet(circleXF16 << 2 >> 16, circleYF16 << 2 >> 16, var30);
    } else {
        gameCanvas->setColor(0, 0, 0);
        gameCanvas->drawLineF16(xF16, yF16, x2F16, y2F16);
        gameCanvas->drawLineF16(x2F16, y2F16, x3F16, y3F16);
        gameCanvas->setColor(0, 0, 128);
        gameCanvas->drawLineF16(x3F16, y3F16, x4F16, y4F16);
        gameCanvas->drawLineF16(x4F16, y4F16, x5F16, y5F16);
        gameCanvas->drawLineF16(x5F16, y5F16, x6F16, y6F16);
        int var30 = 65536;
        gameCanvas->setColor(156, 0, 0);
        gameCanvas->drawCircle(circleXF16 << 2 >> 16, circleYF16 << 2 >> 16, (var30 + var30) << 2 >> 16);
    }

    gameCanvas->setColor(0, 0, 0);
    gameCanvas->drawForthSpriteByCenter(x6F16 << 2 >> 16, y6F16 << 2 >> 16);
    gameCanvas->drawForthSpriteByCenter(var14 << 2 >> 16, var15 << 2 >> 16);
}

void GamePhysics::renderMotoAsLines(GameCanvas *gameCanvas, int var2, int var3, int var4, int var5) {
    int var7 = motoComponents[2]->xF16;
    int var8 = motoComponents[2]->yF16;
    int var9 = var7 + (int) ((int64_t) var4 * (int64_t) 32768 >> 16);
    int var10 = var8 + (int) ((int64_t) var5 * (int64_t) 32768 >> 16);
    int var11 = var7 - (int) ((int64_t) var4 * (int64_t) 32768 >> 16);
    int var12 = var8 - (int) ((int64_t) var5 * (int64_t) 32768 >> 16);
    int var13 = motoComponents[0]->xF16 + (int) ((int64_t) var2 * 32768L >> 16);
    int var14 = motoComponents[0]->yF16 + (int) ((int64_t) var3 * 32768L >> 16);
    int var15 = var13 - (int) ((int64_t) var2 * 131072L >> 16);
    int var16 = var14 - (int) ((int64_t) var3 * 131072L >> 16);
    int var17 = var15 + (int) ((int64_t) var4 * 65536L >> 16);
    int var18 = var16 + (int) ((int64_t) var5 * 65536L >> 16);
    int var19 = var15 + (int) ((int64_t) var2 * 49152L >> 16) + (int) ((int64_t) var4 * 49152L >> 16);
    int var20 = var16 + (int) ((int64_t) var3 * 49152L >> 16) + (int) ((int64_t) var5 * 49152L >> 16);
    int var21 = var15 + (int) ((int64_t) var4 * 32768L >> 16);
    int var22 = var16 + (int) ((int64_t) var5 * 32768L >> 16);
    int var23 = motoComponents[1]->xF16;
    int var24 = motoComponents[1]->yF16;
    int var25 = motoComponents[4]->xF16 - (int) ((int64_t) var2 * 49152L >> 16);
    int var26 = motoComponents[4]->yF16 - (int) ((int64_t) var3 * 49152L >> 16);
    int var27 = var25 - (int) ((int64_t) var4 * 32768L >> 16);
    int var28 = var26 - (int) ((int64_t) var5 * 32768L >> 16);
    int var29 = var25 - (int) ((int64_t) var2 * 131072L >> 16) + (int) ((int64_t) var4 * 16384L >> 16);
    int var30 = var26 - (int) ((int64_t) var3 * 131072L >> 16) + (int) ((int64_t) var5 * 16384L >> 16);
    int var31 = motoComponents[3]->xF16;
    int var32 = motoComponents[3]->yF16;
    int var33 = var31 + (int) ((int64_t) var4 * 32768L >> 16);
    int var34 = var32 + (int) ((int64_t) var5 * 32768L >> 16);
    int var35 = var31 + (int) ((int64_t) var4 * 114688L >> 16) - (int) ((int64_t) var2 * 32768L >> 16);
    int var36 = var32 + (int) ((int64_t) var5 * 114688L >> 16) - (int) ((int64_t) var3 * 32768L >> 16);
    gameCanvas->setColor(50, 50, 50);
    gameCanvas->drawCircle(var21 << 2 >> 16, var22 << 2 >> 16, (32768 + 32768) << 2 >> 16);
    if (!field_35) {
        gameCanvas->drawLineF16(var9, var10, var17, var18);
        gameCanvas->drawLineF16(var11, var12, var15, var16);
    }

    gameCanvas->drawLineF16(var13, var14, var15, var16);
    gameCanvas->drawLineF16(var13, var14, var31, var32);
    gameCanvas->drawLineF16(var19, var20, var33, var34);
    gameCanvas->drawLineF16(var33, var34, var35, var36);
    if (!field_35) {
        gameCanvas->drawLineF16(var31, var32, var23, var24);
        gameCanvas->drawLineF16(var35, var36, var23, var24);
    }

    gameCanvas->drawLineF16(var17, var18, var27, var28);
    gameCanvas->drawLineF16(var19, var20, var25, var26);
    gameCanvas->drawLineF16(var25, var26, var29, var30);
    gameCanvas->drawLineF16(var27, var28, var29, var30);
}

void GamePhysics::renderGame(GameCanvas *gameCanvas) {
    gameCanvas->clearScreenWithWhite();
    int xxF16 = motoComponents[3]->xF16 - motoComponents[4]->xF16;
    int yyF16 = motoComponents[3]->yF16 - motoComponents[4]->yF16;
    int maxAbs;
    if ((maxAbs = getSmthLikeMaxAbs(xxF16, yyF16)) != 0) {
        xxF16 = (int) (((int64_t) xxF16 << 32) / (int64_t) maxAbs >> 16);
        yyF16 = (int) (((int64_t) yyF16 << 32) / (int64_t) maxAbs >> 16);
    }

    int var5 = -yyF16;
    if (field_35) {
        int var8 = motoComponents[4]->xF16;
        int var7;
        if ((var7 = motoComponents[3]->xF16) >= var8) {
            int var9 = var7;
            var7 = var8;
            var8 = var9;
        }

        levelLoader->gameLevel->method_183(var7, var8);
    }

    if (LevelLoader::isEnabledPerspective) {
        levelLoader->renderLevel3D(gameCanvas, motoComponents[0]->xF16, motoComponents[0]->yF16);
    }

    if (isRenderMotoWithSprites) {
        renderEngine(gameCanvas, xxF16, yyF16);
    }

    if (!Micro::isInGameMenu) {
        renderWheelTires(gameCanvas);
    }

    renderWheelSpokes(gameCanvas);
    if (isRenderMotoWithSprites) {
        gameCanvas->setColor(170, 0, 0);
    } else {
        gameCanvas->setColor(50, 50, 50);
    }

    gameCanvas->method_142(motoComponents[1]->xF16 << 2 >> 16, motoComponents[1]->yF16 << 2 >> 16, const175_1_half[0] << 2 >> 16, MathF16::atan2F16(xxF16, yyF16));
    if (!field_35) {
        renderMotoFork(gameCanvas);
    }

    renderSmth(gameCanvas, xxF16, yyF16, var5, xxF16);
    if (!isRenderMotoWithSprites) {
        renderMotoAsLines(gameCanvas, xxF16, yyF16, var5, xxF16);
    }

    levelLoader->renderTrackNearestLine(gameCanvas);
}
