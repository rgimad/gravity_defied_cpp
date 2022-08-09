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
        field_123[i] = (int) ((int64_t) (GamePhysics::const175_1_half[i] + 19660 >> 1) * (int64_t) (GamePhysics::const175_1_half[i] + 19660 >> 1) >> 16);
        field_124[i] = (int) ((int64_t) (GamePhysics::const175_1_half[i] - 19660 >> 1) * (int64_t) (GamePhysics::const175_1_half[i] - 19660 >> 1) >> 16);
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

void LevelLoader::method_96(GameLevel *gameLevel) {
    field_131 = INT_MIN;
    gameLevel = gameLevel;
    int var2 = gameLevel->pointsCount;
    if (field_121.empty() || field_132 < var2) {
        field_132 = var2 < 100 ? 100 : var2;
        field_121.assign(field_132, std::vector<int>(2));
    }

    field_133 = 0;
    field_134 = 0;
    field_135 = gameLevel->pointPositions[field_133][0];
    field_136 = gameLevel->pointPositions[field_134][0];

    for (int var3 = 0; var3 < var2; ++var3) {
        int var4 = gameLevel->pointPositions[(var3 + 1) % var2][0] - gameLevel->pointPositions[var3][0];
        int var5 = gameLevel->pointPositions[(var3 + 1) % var2][1] - gameLevel->pointPositions[var3][1];
        if (var3 != 0 && var3 != var2 - 1) {
            field_131 = field_131 < gameLevel->pointPositions[var3][0] ? gameLevel->pointPositions[var3][0] : field_131;
        }

        int var6 = -var5;
        int var8 = GamePhysics::getSmthLikeMaxAbs(var6, var4);
        field_121[var3][0] = (int) (((int64_t) var6 << 32) / (int64_t) var8 >> 16);
        field_121[var3][1] = (int) (((int64_t) var4 << 32) / (int64_t) var8 >> 16);
        if (gameLevel->startFlagPoint == 0 && gameLevel->pointPositions[var3][0] > gameLevel->startPosX) {
            gameLevel->startFlagPoint = var3 + 1;
        }

        if (gameLevel->finishFlagPoint == 0 && gameLevel->pointPositions[var3][0] > gameLevel->finishPosX) {
            gameLevel->finishFlagPoint = var3;
        }
    }

    field_133 = 0;
    field_134 = 0;
    field_135 = 0;
    field_136 = 0;
}

void LevelLoader::setMinMaxX(int minX, int maxX) {
    gameLevel->setMinMaxX(minX, maxX);
}

void LevelLoader::renderLevel3D(GameCanvas *gameCanvas, int xF16, int yF16) {
    if (gameCanvas != nullptr) {
        gameCanvas->setColor(0, 170, 0);
        xF16 >>= 1;
        yF16 >>= 1;
        gameLevel->renderLevel3D(gameCanvas, xF16, yF16);
    }
}

void LevelLoader::renderTrackNearestLine(GameCanvas *canvas) {
    canvas->setColor(0, 255, 0);
    gameLevel->renderTrackNearestGreenLine(canvas);
}








