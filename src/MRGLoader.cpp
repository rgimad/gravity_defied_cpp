#include "MRGLoader.h"

static MRGLoader::LevelTracks loadLevel(FileStream& levelFileStream, const uint8_t level)
{
    MRGLoader::LevelTracks levelTracks;
    levelFileStream.readVariable(&levelTracks.tracksCount, true);

    for (uint32_t trackNo = 0; trackNo < levelTracks.tracksCount; ++trackNo) {
        MRGLoader::Track track;
        levelFileStream.readVariable(&track.offset, true);

        std::stringstream ss;
        char nameCh;

        do {
            levelFileStream.readVariable(&nameCh, true);
            ss << nameCh;
        } while (nameCh > 0);

        track.trackName = ss.str();

        levelTracks.tracks.push_back(track);
    }

    std::cout << "LEVEL " << (uint32_t)level << std::endl;
    std::cout << levelTracks.tracksCount << " TRACKS" << std::endl;

    for (auto& i : levelTracks.tracks) {
        std::cout << i.offset << " " << i.trackName << std::endl;
    }

    return levelTracks;
}

static std::array<MRGLoader::LevelTracks, 3> MRGLoader::loadLevels(const std::filesystem::path& mrgFilePath)
{
    FileStream levelFileStream(mrgFilePath, std::ios::in | std::ios::binary);
    std::array<MRGLoader::LevelTracks, 3> levels;

    for (uint8_t level = 0; level < 3; ++level) {
        levels[level] = loadLevel(levelFileStream, level);
    }

    return levels;
}

static MRGLoader::TrackInfo MRGLoader::loadTrack(const std::filesystem::path& mrgFilePath, const uint32_t fileOffset)
{
    FileStream levelFileStream(mrgFilePath, std::ios::in | std::ios::binary);
    levelFileStream.setPos(fileOffset);

    uint8_t c;
    levelFileStream.readVariable(&c, true);
    std::cout << (int)c << std::endl;
    // if (c == 50) {
    //     char var3[20];
    //     inStream->readVariable(var3, false, 20);
    // }

    int32_t pointX, pointY;
    int8_t tempDX, tempDY;

    MRGLoader::TrackInfo trackInfo;
    levelFileStream.readVariable(&trackInfo.startPosX, true);
    levelFileStream.readVariable(&trackInfo.startPosY, true);
    levelFileStream.readVariable(&trackInfo.finishPosX, true);
    levelFileStream.readVariable(&trackInfo.finishPosY, true);
    levelFileStream.readVariable(&trackInfo.pointsCount, true);
    levelFileStream.readVariable(&pointX, true);
    levelFileStream.readVariable(&pointY, true);
    std::cout << trackInfo.pointsCount << std::endl;
    std::cout << trackInfo.startPosX << " " << trackInfo.startPosY << std::endl;
    std::cout << trackInfo.finishPosX << " " << trackInfo.finishPosY << std::endl;

    // l.startPosX = l.startPosX >> 16 << 3;
    // l.startPosY = l.startPosY >> 16 << 3;
    // l.finishPosX = l.finishPosX >> 16 << 3;
    // l.finishPosY = l.finishPosY >> 16 << 3;
    // l.pointX = l.pointX >> 16 << 3;
    // l.pointY = l.pointY >> 16 << 3;

    // std::cout << l.pointsCount << std::endl;
    // std::cout << l.startPosX << " " << l.startPosY << std::endl;
    // std::cout << l.finishPosX << " " << l.finishPosY << std::endl;
    // std::cout << l.pointX << " " << l.pointY << std::endl;

    int32_t offsetX = pointX;
    int32_t offsetY = pointY;
    trackInfo.points.push_back({ .x = offsetX, .y = offsetY });

    for (uint16_t i = 1; i < trackInfo.pointsCount; ++i) {
        levelFileStream.readVariable(&tempDX, true);

        if (tempDX == -1) {
            offsetY = 0;
            offsetX = 0;
            levelFileStream.readVariable(&pointX, true);
            levelFileStream.readVariable(&pointY, true);
        } else {
            pointX = tempDX;
            levelFileStream.readVariable(&tempDY, true);
            pointY = tempDY;
        }

        offsetX += pointX;
        offsetY += pointY;
        trackInfo.points.push_back({ .x = offsetX, .y = offsetY });
        // addPointSimple(offsetX, offsetY);
    }

    for (auto& i : trackInfo.points) {
        std::cout << "X: " << i.x << " Y: " << i.y << std::endl;
    }

    return trackInfo;
}

// void GameLevel::load(FileStream* inStream)
// {
//     init();
//     int8_t c;
//     inStream->readVariable(&c, true);
//     if (c == 50) {
//         char var3[20];
//         inStream->readVariable(var3, false, 20);
//     }

//     finishFlagPoint = 0;
//     startFlagPoint = 0;
//     int pointX, pointY;
//     short pointsCount;
//     inStream->readVariable(&startPosX, true);
//     inStream->readVariable(&startPosY, true);
//     inStream->readVariable(&finishPosX, true);
//     inStream->readVariable(&finishPosY, true);
//     inStream->readVariable(&pointsCount, true);
//     inStream->readVariable(&pointX, true);
//     inStream->readVariable(&pointY, true);
//     int offsetX = pointX;
//     int offsetY = pointY;
//     addPointSimple(pointX, pointY);

//     for (int i = 1; i < pointsCount; ++i) {
//         int8_t modeOrDx;
//         inStream->readVariable(&modeOrDx, true);
//         if (modeOrDx == -1) {
//             offsetY = 0;
//             offsetX = 0;
//             inStream->readVariable(&pointX, true);
//             inStream->readVariable(&pointY, true);
//         } else {
//             pointX = modeOrDx;
//             int8_t temp;
//             inStream->readVariable(&temp, true);
//             pointY = temp;
//         }

//         offsetX += pointX;
//         offsetY += pointY;
//         addPointSimple(offsetX, offsetY);
//     }
// }

// void GameLevel::addPointSimple(int var1, int var2)
// {
//     addPoint(var1 << 16 >> 3, var2 << 16 >> 3);
// }

// void GameLevel::addPoint(int x, int y)
// {
//     if (pointPositions.empty() || static_cast<int>(pointPositions.size()) <= pointsCount) {
//         int var3 = 100;
//         if (!pointPositions.empty()) {
//             var3 = var3 < static_cast<int>(pointPositions.size()) + 30 ? pointPositions.size() + 30 : var3;
//         }
//         pointPositions.resize(var3, std::vector<int>(2)); // ABOBA
//     }

//     if (pointsCount == 0 || pointPositions[pointsCount - 1][0] < x) {
//         pointPositions[pointsCount][0] = x;
//         pointPositions[pointsCount][1] = y;
//         ++pointsCount;
//     }
// }

int main(void)
{
    std::array<MRGLoader::LevelTracks, 3> levels = MRGLoader::loadLevels("./levels.mrg");
    MRGLoader::loadTrack("./levels.mrg", levels[0].tracks[0].offset);
    return 0;
}
