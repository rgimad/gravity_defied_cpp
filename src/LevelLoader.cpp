#include "LevelLoader.h"
#include "utils/FileStream.h"
#include "utils/EmbedFileStream.h"

#include <climits>
#include <algorithm>

int LevelLoader::visibleSegmentStartIdx = 0;
int LevelLoader::visibleSegmentEndIdx = 0;
int LevelLoader::visibleSegmentStartX = 0;
int LevelLoader::visibleSegmentEndX = 0;

bool LevelLoader::isEnabledPerspective = true;
bool LevelLoader::isEnabledShadows = true;

LevelLoader::LevelLoader(const std::filesystem::path& mrgFilePath)
{
    for (int i = 0; i < 3; ++i) {
        collisionRadiusSqOuter[i] = (int)((int64_t)((GamePhysics::wheelRadiusValuesF16[i] + 19660) >> 1) * (int64_t)((GamePhysics::wheelRadiusValuesF16[i] + 19660) >> 1) >> 16);
        collisionRadiusSqInner[i] = (int)((int64_t)((GamePhysics::wheelRadiusValuesF16[i] - 19660) >> 1) * (int64_t)((GamePhysics::wheelRadiusValuesF16[i] - 19660) >> 1) >> 16);
    }

    if (!mrgFilePath.string().empty()) {
        FileStream* fileStream = new FileStream(mrgFilePath, std::ios::in | std::ios::binary);
        if (!fileStream->isOpen()) {
            throw std::system_error(errno, std::system_category(), "Failed to open " + mrgFilePath.string());
        }
        levelFileStream = fileStream;
    } else {
        EmbedFileStream* embedFileStream = new EmbedFileStream("levels.mrg");
        levelFileStream = static_cast<FileStream*>(embedFileStream);
    }

    loadLevels();
    loadCurrentTrack();
}

LevelLoader::~LevelLoader()
{
    delete levelFileStream;
}

void LevelLoader::loadLevels()
{
    // Buffer for reading level name (null-terminated)
    std::vector<int8_t> nameBuffer(40);
    // Number of tracks per level
    std::vector<int> trackCounts(3);

    for (int level = 0; level < 3; ++level) {
        // Read number of levels in this league
        levelFileStream->readVariable(&trackCounts[level], true);
        trackOffsetInFile[level] = std::vector<int>(trackCounts[level]);
        trackNames[level] = std::vector<std::string>(trackCounts[level]);

        for (int track = 0; track < trackCounts[level]; ++track) {
            // Read byte offset of track data in MRG file
            int byteOffset;
            levelFileStream->readVariable(&byteOffset, true);
            trackOffsetInFile[level][track] = byteOffset;

            // Read level name (null-terminated string, max 40 chars)
            for (int charIdx = 0; charIdx < 40; ++charIdx) {
                levelFileStream->readVariable(&nameBuffer[charIdx], true);
                if (nameBuffer[charIdx] == 0) {
                    std::string levelName = std::string(reinterpret_cast<char*>(nameBuffer.data()), charIdx);
                    std::replace(levelName.begin(), levelName.end(), '_', ' ');
                    trackNames[level][track] = levelName;
                    break;
                }
            }
        }
    }
}

std::string LevelLoader::getName(int league, int level)
{
    return league < 3 && level < static_cast<int>(trackNames[league].size()) ? trackNames[league][level] : "---";
}

void LevelLoader::loadCurrentTrack()
{
    loadTrack(currentLevel, currentTrack + 1);
}

int LevelLoader::loadTrack(int league, int track)
{
    currentLevel = league;
    currentTrack = track;
    // Wrap track index if out of bounds
    if (currentTrack >= static_cast<int>(trackNames[currentLevel].size())) {
        currentTrack = 0;
    }

    // Convert to 1-based indexing for seekAndLoadTrackData
    seekAndLoadTrackData(currentLevel + 1, currentTrack + 1);
    return currentTrack;
}

void LevelLoader::seekAndLoadTrackData(int league, int track)
{
    // Seek to track byte offset in MRG file (1-based indices)
    levelFileStream->setPos(trackOffsetInFile[league - 1][track - 1]);
    if (gameLevel == nullptr) {
        gameLevel = new GameLevel();
    }
    gameLevel->load(levelFileStream);
    precomputeTrackGeometry(gameLevel);
}

void LevelLoader::cacheStartPosition()
{
    // Cache start position in F16 format (shifted left by 1)
    cachedStartPosXF16 = gameLevel->startPosX << 1;
    cachedStartPosYF16 = gameLevel->startPosY << 1;
}

int LevelLoader::getFinishFlagX()
{
    return gameLevel->pointPositions[gameLevel->finishFlagPoint][0] << 1;
}

int LevelLoader::getStartFlagX()
{
    return gameLevel->pointPositions[gameLevel->startFlagPoint][0] << 1;
}

int LevelLoader::getStartPosX()
{
    return gameLevel->startPosX << 1;
}

int LevelLoader::getStartPosY()
{
    return gameLevel->startPosY << 1;
}

int LevelLoader::getTrackProgressRatio(int xF16)
{
    // Convert from F16 and compute progress ratio along track
    return gameLevel->calculateProgressPercent(xF16 >> 1);
}

void LevelLoader::precomputeTrackGeometry(GameLevel* level)
{
    trackMinX = INT_MIN;
    this->gameLevel = level;
    int pointsCount = level->pointsCount;

    // Resize trackSegmentNormals array if needed (min capacity: 100)
    if (trackSegmentNormals.empty() || trackNormalsCapacity < pointsCount) {
        trackNormalsCapacity = pointsCount < 100 ? 100 : pointsCount;
        trackSegmentNormals.assign(trackNormalsCapacity, std::vector<int>(2));
    }

    // Reset visible segment range
    visibleSegmentStartIdx = 0;
    visibleSegmentEndIdx = 0;
    visibleSegmentStartX = level->pointPositions[visibleSegmentStartIdx][0];
    visibleSegmentEndX = level->pointPositions[visibleSegmentEndIdx][0];

    // Compute normalized normal vectors for each track segment
    for (int i = 0; i < pointsCount; ++i) {
        int dx = level->pointPositions[(i + 1) % pointsCount][0] - level->pointPositions[i][0];
        int dy = level->pointPositions[(i + 1) % pointsCount][1] - level->pointPositions[i][1];

        // Track minimum X (excluding first and last point)
        if (i != 0 && i != pointsCount - 1) {
            trackMinX = trackMinX < level->pointPositions[i][0] ? level->pointPositions[i][0] : trackMinX;
        }

        // Compute normal vector (perpendicular to segment)
        int nx = -dy;
        int length = GamePhysics::fastVectorLengthF16(nx, dx);
        trackSegmentNormals[i][0] = (int)(((int64_t)nx << 32) / (int64_t)length >> 16);
        trackSegmentNormals[i][1] = (int)(((int64_t)dx << 32) / (int64_t)length >> 16);

        // Determine start flag point index
        if (level->startFlagPoint == 0 && level->pointPositions[i][0] > level->startPosX) {
            level->startFlagPoint = i + 1;
        }

        // Determine finish flag point index
        if (level->finishFlagPoint == 0 && level->pointPositions[i][0] > level->finishPosX) {
            level->finishFlagPoint = i;
        }
    }

    // Reset visible segment cache
    visibleSegmentStartIdx = 0;
    visibleSegmentEndIdx = 0;
    visibleSegmentStartX = 0;
    visibleSegmentEndX = 0;
}

void LevelLoader::setLevelBounds(int minX, int maxX)
{
    gameLevel->setMinMaxX(minX, maxX);
}

void LevelLoader::renderTrack3D(GameCanvas* canvas, int cameraXF16, int cameraYF16)
{
    if (canvas != nullptr) {
        canvas->setColor(0, 170, 0); // Green track
        // Convert from F16 to internal format
        cameraXF16 >>= 1;
        cameraYF16 >>= 1;
        gameLevel->renderLevel3D(canvas, cameraXF16, cameraYF16);
    }
}

void LevelLoader::renderTrackCenterline(GameCanvas* canvas)
{
    canvas->setColor(0, 255, 0); // Bright green centerline
    gameLevel->renderTrackNearestGreenLine(canvas);
}

void LevelLoader::updateVisibleSegmentRange(int minXF16, int maxXF16, int centerYF16)
{
    // Update level rendering bounds (with margin)
    gameLevel->setShadowBoundaries((minXF16 + 98304) >> 1, (maxXF16 - 98304) >> 1, centerYF16 >> 1);

    // Convert bounds from F16 to internal format
    maxXF16 >>= 1;
    minXF16 >>= 1;

    // Clamp visible segment indices to valid range
    visibleSegmentEndIdx = visibleSegmentEndIdx < gameLevel->pointsCount - 1 ? visibleSegmentEndIdx : gameLevel->pointsCount - 1;
    visibleSegmentStartIdx = visibleSegmentStartIdx < 0 ? 0 : visibleSegmentStartIdx;

    // Expand visible range based on camera bounds
    if (maxXF16 > visibleSegmentEndX) {
        // Expand right boundary
        while (visibleSegmentEndIdx < gameLevel->pointsCount - 1 && maxXF16 > gameLevel->pointPositions[++visibleSegmentEndIdx][0]) {
        }
    } else if (minXF16 < visibleSegmentStartX) {
        // Expand left boundary
        while (visibleSegmentStartIdx > 0 && minXF16 < gameLevel->pointPositions[--visibleSegmentStartIdx][0]) {
        }
    } else {
        // Recalculate visible range from scratch
        while (visibleSegmentStartIdx < gameLevel->pointsCount && minXF16 > gameLevel->pointPositions[++visibleSegmentStartIdx][0]) {
        }

        if (visibleSegmentStartIdx > 0) {
            --visibleSegmentStartIdx;
        }

        while (visibleSegmentEndIdx > 0 && maxXF16 < gameLevel->pointPositions[--visibleSegmentEndIdx][0]) {
        }

        visibleSegmentEndIdx = visibleSegmentEndIdx + 1 < gameLevel->pointsCount - 1 ? visibleSegmentEndIdx + 1 : gameLevel->pointsCount - 1;
    }

    // Cache X positions of visible segment boundaries
    visibleSegmentStartX = gameLevel->pointPositions[visibleSegmentStartIdx][0];
    visibleSegmentEndX = gameLevel->pointPositions[visibleSegmentEndIdx][0];
}

int LevelLoader::checkSegmentCollisions(PhysicsElemOrMenuItem* obj, int radiusIndex)
{
    int collisionCount = 0;
    int8_t collisionType = 2; // 0=deep, 1=surface, 2=none
    int objX = obj->xF16 >> 1;
    int objY = obj->yF16 >> 1;
    if (isEnabledPerspective) {
        objY -= 65536;
    }

    int accumulatedNormalX = 0, accumulatedNormalY = 0;

    // Check collision with each visible track segment
    for (int i = visibleSegmentStartIdx; i < visibleSegmentEndIdx; ++i) {
        int segStartX = gameLevel->pointPositions[i][0];
        int segStartY = gameLevel->pointPositions[i][1];
        int segEndX = gameLevel->pointPositions[i + 1][0];
        int segEndY;
        if ((segEndY = gameLevel->pointPositions[i + 1][1]) < segStartY) {
            ; // No-op, just assignment
        }

        // Quick bounding box check
        if (objX - collisionRadiusSqOuter[radiusIndex] <= segEndX && objX + collisionRadiusSqOuter[radiusIndex] >= segStartX) {
            int segDx = segStartX - segEndX;
            int segDy = segStartY - segEndY;
            int segLengthSq = (int)((int64_t)segDx * (int64_t)segDx >> 16) + (int)((int64_t)segDy * (int64_t)segDy >> 16);

            // Project object onto segment line
            int projection = (int)((int64_t)(objX - segStartX) * (int64_t)(-segDx) >> 16) + (int)((int64_t)(objY - segStartY) * (int64_t)(-segDy) >> 16);
            int t; // Parametric position on segment (0-65536)
            if ((segLengthSq < 0 ? -segLengthSq : segLengthSq) >= 3) {
                t = (int)(((int64_t)projection << 32) / (int64_t)segLengthSq >> 16);
            } else {
                t = (projection > 0 ? 1 : -1) * (segLengthSq > 0 ? 1 : -1) * INT_MAX;
            }

            // Clamp t to [0, 65536]
            if (t < 0)
                t = 0;
            if (t > 65536)
                t = 65536;

            // Find closest point on segment
            int closestX = segStartX + (int)((int64_t)t * (int64_t)(-segDx) >> 16);
            int closestY = segStartY + (int)((int64_t)t * (int64_t)(-segDy) >> 16);
            segDx = objX - closestX;
            segDy = objY - closestY;

            int8_t localCollisionType;
            int64_t distSq;
            if ((distSq = ((int64_t)segDx * (int64_t)segDx >> 16) + ((int64_t)segDy * (int64_t)segDy >> 16)) < (int64_t)collisionRadiusSqOuter[radiusIndex]) {
                if (distSq >= (int64_t)collisionRadiusSqInner[radiusIndex]) {
                    localCollisionType = 1; // Surface collision
                } else {
                    localCollisionType = 0; // Deep collision
                }
            } else {
                localCollisionType = 2; // No collision
            }

            // Check if object is moving towards the segment
            int dotProduct = (int)((int64_t)trackSegmentNormals[i][0] * (int64_t)obj->velocityXF16 >> 16) + (int)((int64_t)trackSegmentNormals[i][1] * (int64_t)obj->velocityYF16 >> 16);

            if (localCollisionType == 0 && dotProduct < 0) {
                // Deep collision - store normal and return immediately
                lastCollisionNormalXF16 = trackSegmentNormals[i][0];
                lastCollisionNormalYF16 = trackSegmentNormals[i][1];
                return 0;
            }

            if (localCollisionType == 1 && dotProduct < 0) {
                // Surface collision - accumulate normals for response
                ++collisionCount;
                collisionType = 1;
                if (collisionCount == 1) {
                    accumulatedNormalX = trackSegmentNormals[i][0];
                    accumulatedNormalY = trackSegmentNormals[i][1];
                } else {
                    accumulatedNormalX += trackSegmentNormals[i][0];
                    accumulatedNormalY += trackSegmentNormals[i][1];
                }
            }
        }
    }

    if (collisionType == 1) {
        // Check if accumulated normal indicates collision response needed
        if ((int)((int64_t)accumulatedNormalX * (int64_t)obj->velocityXF16 >> 16) + (int)((int64_t)accumulatedNormalY * (int64_t)obj->velocityYF16 >> 16) >= 0) {
            return 2; // No response needed
        }

        lastCollisionNormalXF16 = accumulatedNormalX;
        lastCollisionNormalYF16 = accumulatedNormalY;
    }

    return collisionType;
}
