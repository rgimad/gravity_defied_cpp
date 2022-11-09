#pragma once

#include <cstdint>
#include <vector>
#include <fstream>

#include "GamePhysics.h"

class GameLevel {
private:
    int minX = 0;
    int maxX = 0;
    int field_264 = 0;
    int field_265 = 0;
    int field_266 = 0;
    int field_277 = 0;

public:
    int startPosX;
    int startPosY;
    int finishPosX;
    int startFlagPoint = 0;
    int finishFlagPoint = 0;
    int finishPosY;
    int pointsCount;
    int field_274;
    std::vector< std::vector<int> > pointPositions;
    // String field_276 = "levelname"; // unused?

    GameLevel();
    ~GameLevel();
    void init();
    void method_174(int var1, int var2, int var3, int var4);
    int getStartPosX();
    int getStartPosY();
    int getFinishPosX();
    int getFinishPosY();
    int getPointX(int pointNo);
    int getPointY(int pointNo);
    int method_181(int var1);
    void setMinMaxX(int minX, int maxX);
    void method_183(int var1, int var2);
    void method_184(int var1, int var2, int var3);
    void renderShadow(GameCanvas *gameCanvas, int var2, int var3);
    /*synchronized*/ void renderLevel3D(GameCanvas *gameCanvas, int xF16, int yF16);
    /*synchronized*/ void renderTrackNearestGreenLine(GameCanvas *canvas);
    void addPointSimple(int var1, int var2);
    void addPoint(int x, int y);
    /*synchronized*/ void load(std::ifstream &var1);

};
