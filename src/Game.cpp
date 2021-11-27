#include "Game.h"

Game::Game() {
    field_242 = false;
    numPhysicsLoops = 2;
    gameLoadingStateStage = 0;
    timeMs = 0, gameTimeMs = 0, field_246 = 0;
    isInited = false, field_248 = false;
    field_249 = false, isPaused = true;   
}

Game::~Game() {
    // TODO
}

void Game::setNumPhysicsLoops(int value) {
    numPhysicsLoops = value;
}