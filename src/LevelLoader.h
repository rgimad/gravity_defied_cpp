#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#include "GamePhysics.h"

class LevelLoader {
private:
    int[][] field_121 = (int[][]) null; // TODO
    int field_123[3];
    int field_124[3];
    static int[][] levelOffsetInFile = new int[3][]; // TODO

    int field_132 = 0;
    static int field_133;
    static int field_134;
    static int field_135;
    static int field_136;

public:
    static const int field_114;
    static const int field_115;
    static const int field_116;
    static const int field_117;
    static const int field_118;
    static bool isEnabledPerspective;
    static bool isEnabledShadows;
    GameLevel *gameLevel = nullptr;
    int field_125 = 0;
    int field_126 = -1;
    std::vector<std::string> levelNames[3];
    int field_129;
    int field_130;
    int field_131;
    int field_137;
    int field_138;

    LevelLoader();
    ~LevelLoader();

    void method_87();
    int method_88(int var1, int var2);
    void method_89(int var1, int var2);

    void method_90(int var1);
    int method_91();
    int method_92();
    int method_93();
    int method_94();
    int method_95(int var1);


};


