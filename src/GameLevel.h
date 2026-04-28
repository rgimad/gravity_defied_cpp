#pragma once

#include <cstdint>
#include <vector>
#include <fstream>

#include "utils/FileStream.h"

class GameCanvas;

class GameLevel {
private:
    int minX = 0;
    int maxX = 0;
    // Shadow rendering start X coordinate
    int shadowStartX = 0;
    // Shadow rendering end X coordinate
    int shadowEndX = 0;
    // Y threshold for shadow height calculation
    int shadowHeightThreshold = 0;
    // Accumulated shadow intensity value
    int shadowIntensity = 0;

public:
    int startPosX;
    int startPosY;
    int finishPosX;
    int startFlagPoint = 0;
    int finishFlagPoint = 0;
    int finishPosY;
    int pointsCount;
    int field_274; // UNUSED - dead code, initialized but never used
    std::vector<std::vector<int>> pointPositions;

    GameLevel();
    ~GameLevel();
    void init();
    void setStartFinishPositions(int startX, int startY, int finishX, int finishY);
    int getStartPosX();
    int getStartPosY();
    int getFinishPosX();
    int getFinishPosY();
    int getPointX(int pointNo);
    int getPointY(int pointNo);
    int calculateProgressPercent(int currentX);
    void setMinMaxX(int minX, int maxX);
    // Sets boundaries divided by 2
    void setShadowBoundariesHalf(int startX, int endX);
    void setShadowBoundaries(int startX, int endX, int heightThreshold);
    void renderShadow(GameCanvas* gameCanvas, int startLineIdx, int endLineIdx);
    /*synchronized*/ void renderLevel3D(GameCanvas* gameCanvas, int xF16, int yF16);
    /*synchronized*/ void renderTrackNearestGreenLine(GameCanvas* canvas);
    void addPointSimple(int x, int y);
    void addPoint(int x, int y);
    /*synchronized*/ void load(FileStream* inStream);
};
