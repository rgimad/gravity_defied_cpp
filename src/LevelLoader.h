#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

#include "GamePhysics.h"
#include "GameCanvas.h"
#include "GameLevel.h"
#include "PhysicsElemOrMenuItem.h"
#include "utils/FileStream.h"

class LevelLoader {
private:
    // Track segment normals (array of [nx, ny])
    std::vector<std::vector<int>> trackSegmentNormals;
    // Collision radius² outer threshold
    int collisionRadiusSqOuter[3];
    // Collision radius² inner threshold
    int collisionRadiusSqInner[3];
    inline static std::vector<std::vector<int>> trackOffsetInFile = std::vector<std::vector<int>>(3);

    // Allocated capacity of trackSegmentNormals array
    int trackNormalsCapacity = 0;
    // Leftmost visible track point index
    static int visibleSegmentStartIdx;
    // Rightmost visible track point index
    static int visibleSegmentEndIdx;
    // Cached X position of visibleSegmentStartIdx
    static int visibleSegmentStartX;
    // Cached X position of visibleSegmentEndIdx
    static int visibleSegmentEndX;

    FileStream* levelFileStream;
    void loadLevels();

public:
    static bool isEnabledPerspective;
    static bool isEnabledShadows;
    GameLevel* gameLevel = nullptr;
    int currentLevel = 0;
    int currentTrack = -1;
    std::vector<std::vector<std::string>> trackNames = std::vector<std::vector<std::string>>(3);
    // Start position X in F16 format (startPosX << 1)
    int cachedStartPosXF16;
    // Start position Y in F16 format (startPosY << 1)
    int cachedStartPosYF16;
    // Minimum X among track points (excluding first/last)
    int trackMinX;
    int lastCollisionNormalXF16;
    int lastCollisionNormalYF16;

    LevelLoader(const std::filesystem::path& mrgFilePath);
    ~LevelLoader();

    std::string getName(int league, int level);

    void loadCurrentTrack();
    int loadTrack(int league, int track);
    void seekAndLoadTrackData(int league, int track);

    // Caches start position coordinates (xF16, yF16)
    void cacheStartPosition();
    int getFinishFlagX();
    int getStartFlagX();
    int getStartPosX();
    int getStartPosY();
    // Returns progress ratio (0-65536, F16)
    int getTrackProgressRatio(int xF16);
    void precomputeTrackGeometry(GameLevel* level);
    void setLevelBounds(int minX, int maxX);
    void renderTrack3D(GameCanvas* canvas, int cameraXF16, int cameraYF16);
    void renderTrackCenterline(GameCanvas* canvas);
    void updateVisibleSegmentRange(int minXF16, int maxXF16, int centerYF16);
    int checkSegmentCollisions(PhysicsElemOrMenuItem* obj, int radiusIndex);
};
