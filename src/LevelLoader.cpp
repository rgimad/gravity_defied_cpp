#include "LevelLoader.h"

int LevelLoader::field_133 = 0;
int LevelLoader::field_134 = 0;
int LevelLoader::field_135 = 0;
int LevelLoader::field_136 = 0;

const int LevelLoader::field_114 = 0;
const int LevelLoader::field_115 = 1;
const int LevelLoader::field_116 = 2;
const int LevelLoader::field_117 = 0;
const int LevelLoader::field_118 = 1;
bool LevelLoader::isEnabledPerspective = true;
bool LevelLoader::isEnabledShadows = true;

LevelLoader::LevelLoader() {
    for (int i = 0; i < 3; ++i) {
        field_123[i] = (int) ((uint64_t) (GamePhysics::const175_1_half[i] + 19660 >> 1) * (uint64_t) (GamePhysics::const175_1_half[i] + 19660 >> 1) >> 16);
        field_124[i] = (int) ((uint64_t) (GamePhysics::const175_1_half[i] - 19660 >> 1) * (uint64_t) (GamePhysics::const175_1_half[i] - 19660 >> 1) >> 16);
    }

    try {
        loadLevels();
    } catch (std::exception &e) {
        // TODO
    }
    method_87();
}

void LevelLoader::method_87() {
    method_88(field_125, field_126 + 1);
}

int LevelLoader::method_88(int var1, int var2) {
    field_125 = var1;
    field_126 = var2;
    if (field_126 >= levelNames[field_125].size()) {
        field_126 = 0;
    }

    method_89(field_125 + 1, field_126 + 1);
    return field_126;
}

void LevelLoader::method_89(int var1, int var2) {
    std::ifstream dis("levels.mrg", std::ios::binary);
    dis.seekg(levelOffsetInFile[var1 - 1][var2 - 1]);
    if (gameLevel == nullptr) {
        gameLevel = new GameLevel();
    }
    gameLevel->load(dis);
    dis.close();
    method_96(gameLevel);
}


void LevelLoader::method_90(int var1) {
    field_129 = gameLevel->startPosX << 1;
    field_130 = gameLevel->startPosY << 1;
}

int LevelLoader::method_91() {
    return gameLevel->pointPositions[gameLevel->finishFlagPoint][0] << 1;
}

int LevelLoader::method_92() {
    return gameLevel->pointPositions[gameLevel->startFlagPoint][0] << 1;
}

int LevelLoader::method_93() {
    return gameLevel->startPosX << 1;
}

int LevelLoader::method_94() {
    return gameLevel->startPosY << 1;
}

int LevelLoader::method_95(int var1) {
    return gameLevel->method_181(var1 >> 1);
}






