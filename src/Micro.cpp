#include "Micro.h"

bool Micro::field_249 = false;
bool Micro::isPaused = true;
int Micro::gameLoadingStateStage = 0;

Micro::Micro() {

}

Micro::~Micro() {
    // TODO
}

void Micro::setNumPhysicsLoops(int value) {
    numPhysicsLoops = value;
}