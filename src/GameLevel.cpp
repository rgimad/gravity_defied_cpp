#include "GameLevel.h"

#include "LevelLoader.h"
#include "GamePhysics.h"
#include "GameCanvas.h"

GameLevel::GameLevel()
{
    init();
}

void GameLevel::init()
{
    startPosX = 0;
    startPosY = 0;
    finishPosX = 13107200;
    pointsCount = 0;
    field_274 = 0;
}

void GameLevel::setStartFinishPositions(int startX, int startY, int finishX, int finishY)
{
    startPosX = startX << 16 >> 3;
    startPosY = startY << 16 >> 3;
    finishPosX = finishX << 16 >> 3;
    finishPosY = finishY << 16 >> 3;
}

int GameLevel::getStartPosX()
{
    return startPosX << 3 >> 16;
}

int GameLevel::getStartPosY()
{
    return startPosY << 3 >> 16;
}

int GameLevel::getFinishPosX()
{
    return finishPosX << 3 >> 16;
}

int GameLevel::getFinishPosY()
{
    return finishPosY << 3 >> 16;
}

int GameLevel::getPointX(int pointNo)
{
    return pointPositions[pointNo][0] << 3 >> 16;
}

int GameLevel::getPointY(int pointNo)
{
    return pointPositions[pointNo][1] << 3 >> 16;
}

int GameLevel::calculateProgressPercent(int currentX)
{
    int distanceFromStart = currentX - pointPositions[startFlagPoint][0];
    int totalDistance;
    return ((totalDistance = pointPositions[finishFlagPoint][0] - pointPositions[startFlagPoint][0]) < 0 ? -totalDistance : totalDistance) >= 3 && distanceFromStart <= totalDistance ? (int)(((int64_t)distanceFromStart << 32) / (int64_t)totalDistance >> 16) : 65536;
}

void GameLevel::setMinMaxX(int minX, int maxX)
{
    this->minX = minX << 16 >> 3;
    this->maxX = maxX << 16 >> 3;
}

void GameLevel::setShadowBoundariesHalf(int startX, int endX)
{
    shadowStartX = startX >> 1;
    shadowEndX = endX >> 1;
}

void GameLevel::setShadowBoundaries(int startX, int endX, int heightThreshold)
{
    shadowStartX = startX;
    shadowEndX = endX;
    shadowHeightThreshold = heightThreshold;
}

void GameLevel::renderShadow(GameCanvas* gameCanvas, int startLineIdx, int endLineIdx)
{
    if (endLineIdx <= pointsCount - 1) {
        int shadowHeight = shadowHeightThreshold - ((pointPositions[startLineIdx][1] + pointPositions[endLineIdx + 1][1]) >> 1) < 0 ? 0 : shadowHeightThreshold - ((pointPositions[startLineIdx][1] + pointPositions[endLineIdx + 1][1]) >> 1);
        if (shadowHeightThreshold <= pointPositions[startLineIdx][1] || shadowHeightThreshold <= pointPositions[endLineIdx + 1][1]) {
            shadowHeight = shadowHeight < 327680 ? shadowHeight : 327680;
        }

        shadowIntensity = (int)((int64_t)shadowIntensity * 49152L >> 16) + (int)((int64_t)shadowHeight * 16384L >> 16);
        if (shadowIntensity <= 557056) {
            int shadowColor = (int)(1638400L * (int64_t)shadowIntensity >> 16) >> 16;
            gameCanvas->setColor(shadowColor, shadowColor, shadowColor);
            int lineDx = pointPositions[startLineIdx][0] - pointPositions[startLineIdx + 1][0];
            int lineSlope = (int)(((int64_t)(pointPositions[startLineIdx][1] - pointPositions[startLineIdx + 1][1]) << 32) / (int64_t)lineDx >> 16);
            int lineIntercept = pointPositions[startLineIdx][1] - (int)((int64_t)pointPositions[startLineIdx][0] * (int64_t)lineSlope >> 16);
            int startYProjected = (int)((int64_t)shadowStartX * (int64_t)lineSlope >> 16) + lineIntercept;
            lineDx = pointPositions[endLineIdx][0] - pointPositions[endLineIdx + 1][0];
            lineSlope = (int)(((int64_t)(pointPositions[endLineIdx][1] - pointPositions[endLineIdx + 1][1]) << 32) / (int64_t)lineDx >> 16);
            lineIntercept = pointPositions[endLineIdx][1] - (int)((int64_t)pointPositions[endLineIdx][0] * (int64_t)lineSlope >> 16);
            int endYProjected = (int)((int64_t)shadowEndX * (int64_t)lineSlope >> 16) + lineIntercept;
            if (startLineIdx == endLineIdx) {
                gameCanvas->drawLine(shadowStartX << 3 >> 16, (startYProjected + 65536) << 3 >> 16, shadowEndX << 3 >> 16, (endYProjected + 65536) << 3 >> 16);
                return;
            }

            gameCanvas->drawLine(shadowStartX << 3 >> 16, (startYProjected + 65536) << 3 >> 16, pointPositions[startLineIdx + 1][0] << 3 >> 16, (pointPositions[startLineIdx + 1][1] + 65536) << 3 >> 16);

            for (int i = startLineIdx + 1; i < endLineIdx; ++i) {
                gameCanvas->drawLine(pointPositions[i][0] << 3 >> 16, (pointPositions[i][1] + 65536) << 3 >> 16, pointPositions[i + 1][0] << 3 >> 16, (pointPositions[i + 1][1] + 65536) << 3 >> 16);
            }

            gameCanvas->drawLine(pointPositions[endLineIdx][0] << 3 >> 16, (pointPositions[endLineIdx][1] + 65536) << 3 >> 16, shadowEndX << 3 >> 16, (endYProjected + 65536) << 3 >> 16);
        }
    }
}

void GameLevel::renderLevel3D(GameCanvas* gameCanvas, int xF16, int yF16)
{
    int shadowStartLineIdx = 0, shadowEndLineIdx = 0;
    int lineNo;
    for (lineNo = 0; lineNo < pointsCount - 1 && pointPositions[lineNo][0] <= minX; ++lineNo) {
    }
    if (lineNo > 0) {
        --lineNo;
    }
    int deltaX = xF16 - pointPositions[lineNo][0];
    int deltaY = yF16 + 3276800 - pointPositions[lineNo][1];
    int vectorLength = GamePhysics::fastVectorLengthF16(deltaX, deltaY);
    deltaX = (int)(((int64_t)deltaX << 32) / (int64_t)(vectorLength >> 1 >> 1) >> 16);
    deltaY = (int)(((int64_t)deltaY << 32) / (int64_t)(vectorLength >> 1 >> 1) >> 16);
    gameCanvas->setColor(0, 170, 0);

    while (lineNo < pointsCount - 1) {
        int prevDeltaX = deltaX;
        int prevDeltaY = deltaY;
        deltaX = xF16 - pointPositions[lineNo + 1][0];
        deltaY = yF16 + 3276800 - pointPositions[lineNo + 1][1];
        vectorLength = GamePhysics::fastVectorLengthF16(deltaX, deltaY);
        deltaX = (int)(((int64_t)deltaX << 32) / (int64_t)(vectorLength >> 1 >> 1) >> 16);
        deltaY = (int)(((int64_t)deltaY << 32) / (int64_t)(vectorLength >> 1 >> 1) >> 16);
        // far line
        gameCanvas->drawLine((pointPositions[lineNo][0] + prevDeltaX) << 3 >> 16, (pointPositions[lineNo][1] + prevDeltaY) << 3 >> 16, (pointPositions[lineNo + 1][0] + deltaX) << 3 >> 16, (pointPositions[lineNo + 1][1] + deltaY) << 3 >> 16);
        // from far to near
        gameCanvas->drawLine(pointPositions[lineNo][0] << 3 >> 16, pointPositions[lineNo][1] << 3 >> 16, (pointPositions[lineNo][0] + prevDeltaX) << 3 >> 16, (pointPositions[lineNo][1] + prevDeltaY) << 3 >> 16);
        if (lineNo > 1) {
            if (pointPositions[lineNo][0] > shadowStartX && shadowStartLineIdx == 0) {
                shadowStartLineIdx = lineNo - 1;
            }
            if (pointPositions[lineNo][0] > shadowEndX && shadowEndLineIdx == 0) {
                shadowEndLineIdx = lineNo - 1;
            }
        }
        if (startFlagPoint == lineNo) {
            // render far start flag
            gameCanvas->renderStartFlag((pointPositions[startFlagPoint][0] + prevDeltaX) << 3 >> 16, (pointPositions[startFlagPoint][1] + prevDeltaY) << 3 >> 16);
            gameCanvas->setColor(0, 170, 0);
        }
        if (finishFlagPoint == lineNo) {
            // render far finish flag
            gameCanvas->renderFinishFlag((pointPositions[finishFlagPoint][0] + prevDeltaX) << 3 >> 16, (pointPositions[finishFlagPoint][1] + prevDeltaY) << 3 >> 16);
            gameCanvas->setColor(0, 170, 0);
        }
        if (pointPositions[lineNo][0] > maxX) {
            break;
        }
        ++lineNo;
    }
    gameCanvas->drawLine(pointPositions[pointsCount - 1][0] << 3 >> 16, pointPositions[pointsCount - 1][1] << 3 >> 16, (pointPositions[pointsCount - 1][0] + deltaX) << 3 >> 16, (pointPositions[pointsCount - 1][1] + deltaY) << 3 >> 16);
    if (LevelLoader::isEnabledShadows) {
        renderShadow(gameCanvas, shadowStartLineIdx, shadowEndLineIdx);
    }
}

void GameLevel::renderTrackNearestGreenLine(GameCanvas* gameCanvas)
{
    int pointNo;
    for (pointNo = 0; pointNo < pointsCount - 1 && pointPositions[pointNo][0] <= minX; ++pointNo) {
    }
    if (pointNo > 0) {
        --pointNo;
    }
    while (pointNo < pointsCount - 1) {
        gameCanvas->drawLine(pointPositions[pointNo][0] << 3 >> 16, pointPositions[pointNo][1] << 3 >> 16, pointPositions[pointNo + 1][0] << 3 >> 16, pointPositions[pointNo + 1][1] << 3 >> 16);
        if (startFlagPoint == pointNo) {
            gameCanvas->renderStartFlag(pointPositions[startFlagPoint][0] << 3 >> 16, pointPositions[startFlagPoint][1] << 3 >> 16);
            gameCanvas->setColor(0, 255, 0);
        }
        if (finishFlagPoint == pointNo) {
            gameCanvas->renderFinishFlag(pointPositions[finishFlagPoint][0] << 3 >> 16, pointPositions[finishFlagPoint][1] << 3 >> 16);
            gameCanvas->setColor(0, 255, 0);
        }
        if (pointPositions[pointNo][0] > maxX) {
            break;
        }
        ++pointNo;
    }
}

void GameLevel::load(FileStream* inStream)
{
    init();
    int8_t c;
    inStream->readVariable(&c, true);
    if (c == 50) {
        // DEAD CODE: version header buffer, read but never used
        char versionHeader[20];
        inStream->readVariable(versionHeader, false, 20);
    }

    finishFlagPoint = 0;
    startFlagPoint = 0;
    int pointX, pointY;
    short pointsCount;
    inStream->readVariable(&startPosX, true);
    inStream->readVariable(&startPosY, true);
    inStream->readVariable(&finishPosX, true);
    inStream->readVariable(&finishPosY, true);
    inStream->readVariable(&pointsCount, true);
    inStream->readVariable(&pointX, true);
    inStream->readVariable(&pointY, true);
    int offsetX = pointX;
    int offsetY = pointY;
    addPointSimple(pointX, pointY);

    for (int i = 1; i < pointsCount; ++i) {
        int8_t modeOrDx;
        inStream->readVariable(&modeOrDx, true);
        if (modeOrDx == -1) {
            offsetY = 0;
            offsetX = 0;
            inStream->readVariable(&pointX, true);
            inStream->readVariable(&pointY, true);
        } else {
            pointX = modeOrDx;
            int8_t temp;
            inStream->readVariable(&temp, true);
            pointY = temp;
        }

        offsetX += pointX;
        offsetY += pointY;
        addPointSimple(offsetX, offsetY);
    }
}

void GameLevel::addPointSimple(int x, int y)
{
    addPoint(x << 16 >> 3, y << 16 >> 3);
}

void GameLevel::addPoint(int x, int y)
{
    if (pointPositions.empty() || static_cast<int>(pointPositions.size()) <= pointsCount) {
        int newCapacity = 100;
        if (!pointPositions.empty()) {
            newCapacity = newCapacity < static_cast<int>(pointPositions.size()) + 30 ? pointPositions.size() + 30 : newCapacity;
        }
        pointPositions.resize(newCapacity, std::vector<int>(2));
    }

    if (pointsCount == 0 || pointPositions[pointsCount - 1][0] < x) {
        pointPositions[pointsCount][0] = x;
        pointPositions[pointsCount][1] = y;
        ++pointsCount;
    }
}
