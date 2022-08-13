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




