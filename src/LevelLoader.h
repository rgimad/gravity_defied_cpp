#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <climits>

#include "config.h"
#include "utils/FileStream.h"
#include "utils/EmbedFileStream.h"
#include "MRGLoader.h"
#include "GamePhysics.h"
#include "GameCanvas.h"
#include "GameLevel.h"
#include "TimerOrMotoPartOrMenuElem.h"
#include "utils/Log.h"
#include "utils/FileStream.h"

class LevelLoader {
private:
    std::vector<std::vector<int>> field_121;
    int field_123[3];
    int field_124[3];
    // inline static std::vector<std::vector<int>> levelOffsetInFile = std::vector<std::vector<int>>(3);
    std::array<MRGLoader::LevelTracks, 3> trackHeaders;
    std::filesystem::path mrgFilePath;

    int field_132 = 0;
    static int field_133;
    static int field_134;
    static int field_135;
    static int field_136;

    // FileStream* levelFileStream;
    // void loadLevels();

public:
    static const int field_114;
    static const int field_115;
    static const int field_116;
    static const int field_117;
    static const int field_118;
    static bool isEnabledPerspective;
    static bool isEnabledShadows;
    GameLevel* gameLevel = nullptr;
    int loadedLevel = 0;
    int loadedTrack = -1;
    // std::vector<std::vector<std::string>> levelNames = std::vector<std::vector<std::string>>(3);
    int field_129;
    int field_130;
    int field_131;
    int field_137;
    int field_138;

    LevelLoader(const std::filesystem::path& mrgFilePath);
    // ~LevelLoader();

    std::string getName(const int level, const int track) const;
    int getTracksCount(const int level) const;

    void loadNextTrack();
    int loadTrack(const int level, const int track);
    std::vector<std::string> GetTrackNames(const int level) const;

    void method_90(int var1);
    int method_91();
    int method_92();
    int method_93();
    int method_94();
    int method_95(int var1);
    void method_96(GameLevel* gameLevel);
    void setMinMaxX(int minX, int maxX);
    void renderLevel3D(GameCanvas* gameCanvas, int xF16, int yF16);
    void renderTrackNearestLine(GameCanvas* canvas);
    void method_100(int var1, int var2, int var3);
    int method_101(TimerOrMotoPartOrMenuElem* var1, int var2);
};
