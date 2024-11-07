#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include<array>

#include "utils/FileStream.h"

namespace MRGLoader {
struct TrackPoint {
    int32_t x;
    int32_t y;
};
struct TrackInfo {
    int32_t startPosX;
    int32_t startPosY;
    int32_t finishPosX;
    int32_t finishPosY;
    uint16_t pointsCount;
    // int32_t pointX;
    // int32_t pointY;
    std::vector<TrackPoint> points;
};
struct Track {
    uint32_t offset;
    std::string trackName;
};
struct LevelTracks {
    uint32_t tracksCount;
    std::vector<Track> tracks;
};

static std::array<MRGLoader::LevelTracks, 3> loadLevels(const std::filesystem::path& mrgFilePath);
static MRGLoader::TrackInfo loadTrack(const std::filesystem::path& mrgFilePath, const uint32_t fileOffset);
};
