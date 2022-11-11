#include "GameLevel.h"

GameLevel::GameLevel() {
    init();
}

void GameLevel::init() {
    startPosX = 0;
    startPosY = 0;
    finishPosX = 13107200;
    pointsCount = 0;
    field_274 = 0;
}

void GameLevel::method_174(int var1, int var2, int var3, int var4) {
    startPosX = var1 << 16 >> 3;
    startPosY = var2 << 16 >> 3;
    finishPosX = var3 << 16 >> 3;
    finishPosY = var4 << 16 >> 3;
}

int GameLevel::getStartPosX() {
    return startPosX << 3 >> 16;
}

int GameLevel::getStartPosY() {
    return startPosY << 3 >> 16;
}

int GameLevel::getFinishPosX() {
    return finishPosX << 3 >> 16;
}

int GameLevel::getFinishPosY() {
    return finishPosY << 3 >> 16;
}

int GameLevel::getPointX(int pointNo) {
    return pointPositions[pointNo][0] << 3 >> 16;
}

int GameLevel::getPointY(int pointNo) {
    return pointPositions[pointNo][1] << 3 >> 16;
}

int GameLevel::method_181(int var1) {
    int var2 = var1 - pointPositions[startFlagPoint][0];
    int var3;
    return ((var3 = pointPositions[finishFlagPoint][0] - pointPositions[startFlagPoint][0]) < 0 ? -var3 : var3) >= 3 && var2 <= var3 ? (int) (((int64_t) var2 << 32) / (int64_t) var3 >> 16) : 65536;
}

void GameLevel::setMinMaxX(int minX, int maxX) {
    minX = minX << 16 >> 3;
    maxX = maxX << 16 >> 3;
}

void GameLevel::method_183(int var1, int var2) {
    field_264 = var1 >> 1;
    field_265 = var2 >> 1;
}

void GameLevel::method_184(int var1, int var2, int var3) {
    field_264 = var1;
    field_265 = var2;
    field_266 = var3;
}

void GameLevel::renderShadow(GameCanvas &gameCanvas, int var2, int var3) {
    if (var3 <= pointsCount - 1) {
        int var4 = field_266 - (pointPositions[var2][1] + pointPositions[var3 + 1][1] >> 1) < 0 ? 0 : field_266 - (pointPositions[var2][1] + pointPositions[var3 + 1][1] >> 1);
        if (field_266 <= pointPositions[var2][1] || field_266 <= pointPositions[var3 + 1][1]) {
            var4 = var4 < 327680 ? var4 : 327680;
        }

        field_277 = (int) ((int64_t) field_277 * 49152L >> 16) + (int) ((int64_t) var4 * 16384L >> 16);
        if (field_277 <= 557056) {
            int var5 = (int) (1638400L * (int64_t) field_277 >> 16) >> 16;
            gameCanvas.setColor(var5, var5, var5);
            int var6 = pointPositions[var2][0] - pointPositions[var2 + 1][0];
            int var8 = (int) (((int64_t) (pointPositions[var2][1] - pointPositions[var2 + 1][1]) << 32) / (int64_t) var6 >> 16);
            int var9 = pointPositions[var2][1] - (int) ((int64_t) pointPositions[var2][0] * (int64_t) var8 >> 16);
            int var10 = (int) ((int64_t) field_264 * (int64_t) var8 >> 16) + var9;
            var6 = pointPositions[var3][0] - pointPositions[var3 + 1][0];
            var8 = (int) (((int64_t) (pointPositions[var3][1] - pointPositions[var3 + 1][1]) << 32) / (int64_t) var6 >> 16);
            var9 = pointPositions[var3][1] - (int) ((int64_t) pointPositions[var3][0] * (int64_t) var8 >> 16);
            int var11 = (int) ((int64_t) field_265 * (int64_t) var8 >> 16) + var9;
            if (var2 == var3) {
                gameCanvas.drawLine(field_264 << 3 >> 16, var10 + 65536 << 3 >> 16, field_265 << 3 >> 16, var11 + 65536 << 3 >> 16);
                return;
            }

            gameCanvas.drawLine(field_264 << 3 >> 16, var10 + 65536 << 3 >> 16, pointPositions[var2 + 1][0] << 3 >> 16, pointPositions[var2 + 1][1] + 65536 << 3 >> 16);

            for (int i = var2 + 1; i < var3; ++i) {
                gameCanvas.drawLine(pointPositions[i][0] << 3 >> 16, pointPositions[i][1] + 65536 << 3 >> 16, pointPositions[i + 1][0] << 3 >> 16, pointPositions[i + 1][1] + 65536 << 3 >> 16);
            }

            gameCanvas.drawLine(pointPositions[var3][0] << 3 >> 16, pointPositions[var3][1] + 65536 << 3 >> 16, field_265 << 3 >> 16, var11 + 65536 << 3 >> 16);
        }
    }

}

void GameLevel::renderLevel3D(GameCanvas &gameCanvas, int xF16, int yF16) {
    int var7 = 0, var8 = 0;
    int lineNo;
    for (lineNo = 0; lineNo < pointsCount - 1 && pointPositions[lineNo][0] <= minX; ++lineNo) {
    }
    if (lineNo > 0) {
        --lineNo;
    }
    int var9 = xF16 - pointPositions[lineNo][0];
    int var10 = yF16 + 3276800 - pointPositions[lineNo][1];
    int var11 = GamePhysics::getSmthLikeMaxAbs(var9, var10);
    var9 = (int) (((int64_t) var9 << 32) / (int64_t) (var11 >> 1 >> 1) >> 16);
    var10 = (int) (((int64_t) var10 << 32) / (int64_t) (var11 >> 1 >> 1) >> 16);
    gameCanvas.setColor(0, 170, 0);

    while (lineNo < pointsCount - 1) {
        int var4 = var9;
        int var5 = var10;
        var9 = xF16 - pointPositions[lineNo + 1][0];
        var10 = yF16 + 3276800 - pointPositions[lineNo + 1][1];
        var11 = GamePhysics::getSmthLikeMaxAbs(var9, var10);
        var9 = (int) (((int64_t) var9 << 32) / (int64_t) (var11 >> 1 >> 1) >> 16);
        var10 = (int) (((int64_t) var10 << 32) / (int64_t) (var11 >> 1 >> 1) >> 16);
        // far line
        gameCanvas.drawLine(pointPositions[lineNo][0] + var4 << 3 >> 16, pointPositions[lineNo][1] + var5 << 3 >> 16, pointPositions[lineNo + 1][0] + var9 << 3 >> 16, pointPositions[lineNo + 1][1] + var10 << 3 >> 16);
        // from far to near
        gameCanvas.drawLine(pointPositions[lineNo][0] << 3 >> 16, pointPositions[lineNo][1] << 3 >> 16, pointPositions[lineNo][0] + var4 << 3 >> 16, pointPositions[lineNo][1] + var5 << 3 >> 16);
        if (lineNo > 1) {
            if (pointPositions[lineNo][0] > field_264 && var7 == 0) {
                var7 = lineNo - 1;
            }
            if (pointPositions[lineNo][0] > field_265 && var8 == 0) {
                var8 = lineNo - 1;
            }
        }
        if (startFlagPoint == lineNo) {
            // render far start flag
            gameCanvas.renderStartFlag(pointPositions[startFlagPoint][0] + var4 << 3 >> 16, pointPositions[startFlagPoint][1] + var5 << 3 >> 16);
            gameCanvas.setColor(0, 170, 0);
        }
        if (finishFlagPoint == lineNo) {
            // render far finish flag
            gameCanvas.renderFinishFlag(pointPositions[finishFlagPoint][0] + var4 << 3 >> 16, pointPositions[finishFlagPoint][1] + var5 << 3 >> 16);
            gameCanvas.setColor(0, 170, 0);
        }
        if (pointPositions[lineNo][0] > maxX) {
            break;
        }
        ++lineNo;
    }
    gameCanvas.drawLine(pointPositions[pointsCount - 1][0] << 3 >> 16, pointPositions[pointsCount - 1][1] << 3 >> 16, pointPositions[pointsCount - 1][0] + var9 << 3 >> 16, pointPositions[pointsCount - 1][1] + var10 << 3 >> 16);
    if (LevelLoader::isEnabledShadows) {
        renderShadow(gameCanvas, var7, var8);
    }
}


void GameLevel::renderTrackNearestGreenLine(GameCanvas &gameCanvas) {
    int pointNo;
    for (pointNo = 0; pointNo < pointsCount - 1 && pointPositions[pointNo][0] <= minX; ++pointNo) {
    }
    if (pointNo > 0) {
        --pointNo;
    }
    while (pointNo < pointsCount - 1) {
        gameCanvas.drawLine(pointPositions[pointNo][0] << 3 >> 16, pointPositions[pointNo][1] << 3 >> 16, pointPositions[pointNo + 1][0] << 3 >> 16, pointPositions[pointNo + 1][1] << 3 >> 16);
        if (startFlagPoint == pointNo) {
            gameCanvas.renderStartFlag(pointPositions[startFlagPoint][0] << 3 >> 16, pointPositions[startFlagPoint][1] << 3 >> 16);
            gameCanvas.setColor(0, 255, 0);
        }
        if (finishFlagPoint == pointNo) {
            gameCanvas.renderFinishFlag(pointPositions[finishFlagPoint][0] << 3 >> 16, pointPositions[finishFlagPoint][1] << 3 >> 16);
            gameCanvas.setColor(0, 255, 0);
        }
        if (pointPositions[pointNo][0] > maxX) {
            break;
        }
        ++pointNo;
    }
}

// read int from big endian representation
int read_int_be(std::ifstream &is) {
    char data[4];
    is.read(data, 4);
    if (is.eof()) { // if reached eof when reading
        return 0;
    }
    int res = (data[3]<<0) | (data[2]<<8) | (data[1]<<16) | ((unsigned)data[0]<<24);
    // The last byte must be explicitly cast to unsigned before shifting,
    // as by default it's promoted to int, and a shift by 24 bits means
    // manipulating the sign bit which is Undefined Behavior.
    return res;
}

// read int from big endian representation
short read_short_be(std::ifstream &is) {
    char data[2];
    is.read(data, 2);
    if (is.eof()) { // if reached eof when reading
        return 0;
    }
    short res = (data[1]<<0) | ((unsigned)data[0]<<8);
    return res;
}

void GameLevel::load(std::ifstream &var1) {
    init();
    char c;
    var1.read(&c, 1);
    if (c == 50) {
        char var3[20];
        var1.read(var3, 20);
    }

    finishFlagPoint = 0;
    startFlagPoint = 0;
    startPosX = read_int_be(var1);
    startPosY = read_int_be(var1);
    finishPosX = read_int_be(var1);
    finishPosY = read_int_be(var1);
    short pointsCount = read_short_be(var1);
    int pointX = read_int_be(var1);
    int pointY = read_int_be(var1);
    int offsetX = pointX;
    int offsetY = pointY;
    addPointSimple(pointX, pointY);

    for (int i = 1; i < pointsCount; ++i) {
        unsigned char modeOrDx;
        var1.read(reinterpret_cast<char*>(&modeOrDx), 1);
        if (modeOrDx == 0xFF) {
            offsetY = 0;
            offsetX = 0;
            pointX = read_int_be(var1);
            pointY = read_int_be(var1);
        } else {
            pointX = modeOrDx;
            unsigned char tmp;
            var1.read(reinterpret_cast<char*>(&tmp), 1);
            pointY = tmp;
        }

        offsetX += pointX;
        offsetY += pointY;
        addPointSimple(offsetX, offsetY);
    }

}


void GameLevel::addPointSimple(int var1, int var2) {
    addPoint(var1 << 16 >> 3, var2 << 16 >> 3);
}

void GameLevel::addPoint(int x, int y) {
    if (pointPositions.empty() || pointPositions.size() <= pointsCount) {
        int var3 = 100;
        if (!pointPositions.empty()) {
            var3 = var3 < pointPositions.size() + 30 ? pointPositions.size() + 30 : var3;
        }
        pointPositions.resize(var3, std::vector<int>(2)); // ABOBA
    }

    if (pointsCount == 0 || pointPositions[pointsCount - 1][0] < x) {
        pointPositions[pointsCount][0] = x;
        pointPositions[pointsCount][1] = y;
        ++pointsCount;
    }
}




