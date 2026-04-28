#include "MotoComponent.h"

MotoComponent::MotoComponent()
    : radiusF16(0)
    , radiusIndex(0)
    , inverseMassF16(0)
    , leanInfluenceF16(0)
{
    for (int i = 0; i < 6; ++i) {
        stateBuffers[i] = std::make_unique<PhysicsElemOrMenuItem>();
    }

    reset();
}

void MotoComponent::reset()
{
    radiusF16 = inverseMassF16 = leanInfluenceF16 = 0;
    for (int i = 0; i < 6; ++i) {
        stateBuffers[i]->setToZeros();
    }
}
