#pragma once

class LevelLoader {
private:
    int[][] field_121 = (int[][]) null; // TODO
    int[] field_123 = new int[3]; // TODO
    int[] field_124 = new int[3]; // TODO
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
    String[][] levelNames = new String[3][]; // TODO
    int field_129;
    int field_130;
    int field_131;
    int field_137;
    int field_138;


};


