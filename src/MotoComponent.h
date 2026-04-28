#pragma once

#include <vector>
#include <memory>

#include "PhysicsElemOrMenuItem.h"

// Moto component physics data (bike parts: chassis, wheels, rider, etc.)
class MotoComponent {
public:
    // Component radius - used in collision detection
    int radiusF16;
    // Component type / radius index (0, 1, or 2 maps to different radii)
    int radiusIndex;
    // Component mass (inverse) - used in force calculations: force / mass = acceleration
    int inverseMassF16;
    // Component leanF16 influence - only set for back wheel, used in angular velocity calc
    int leanInfluenceF16;
    /**
     * Buffered physics states for multi-stage integration
     *
     * 0 - Current state (read buffer)
     * 1 - Next state (write buffer)
     * 2 - Integration work A (predictor)
     * 3 - Integration work B (corrector)
     * 4 - Midpoint state (predictor-corrector)
     * 5 - Render snapshot (copied to GamePhysics::renderCache)
     */
    std::vector<std::unique_ptr<PhysicsElemOrMenuItem>> stateBuffers = std::vector<std::unique_ptr<PhysicsElemOrMenuItem>>(6);

    MotoComponent();
    ~MotoComponent() = default;
    void reset();
};
