#pragma once

#include <vector>
#include <memory>
#include "PhysicsElemOrMenuItem.h"

class LevelLoader;
class MotoComponent;
class GameCanvas;

/**
 * PHYSICS BUFFERING SCHEME
 *
 * Each MotoComponent has 6 stateBuffers for multi-stage physics integration:
 *
 *   0 - readBuffer   - Current validated state (read source for simulation)
 *   1 - writeBuffer  - Next state under construction (write target)
 *   2 - workA        - Integration intermediate (predictor step)
 *   3 - workB        - Integration intermediate (corrector step)
 *   4 - workMid      - Midpoint state for predictor-corrector
 *   5 - renderCache  - Snapshot for rendering (decoupled from physics)
 *
 * Double buffering: readBuffer/writeBuffer swap after each physics frame.
 * Integration: Uses buffers 2-4 for predictor-corrector (RK2-like) scheme.
 * Rendering: Reads from buffer 5 via renderCache copy.
 */
class GamePhysics {
private:
    // Current validated physics state (read source)
    int readBufferIndex = 0;
    // Next physics state under construction (write target)
    int writeBufferIndex = 1;
    // Last collided component index (-1 if none)
    int lastCollidedComponentIndex = -1;
    /**
     * Spring/suspension connections between bike components, storing rest length, stiffness, and damping
     *
     * 0 - chassis ↔ front wheel
     * 1 - chassis ↔ back wheel
     * 2 - chassis ↔ handlebar
     * 3 - chassis ↔ seat
     * 4 - handlebar ↔ seat
     * 5 - front wheel ↔ handlebar
     * 6 - back wheel ↔ seat
     * 7 - rider ↔ seat
     * 8 - rider ↔ handlebar
     * 9 - rider ↔ chassis
     */
    std::vector<std::unique_ptr<PhysicsElemOrMenuItem>> springConstraints;

    // Engine momentum / throttle value (drives wheel torque)
    int engineMomentumF16 = 0;
    LevelLoader* levelLoader;
    // Collision normal X - from LevelLoader
    int collisionNormalXF16 = 0;
    // Collision normal Y - from LevelLoader
    int collisionNormalYF16 = 0;
    // Bike destroyed / crashed flag
    bool isBikeDestroyed = false;
    // Player head crash flag - set when rider head (component 5) touches ground
    bool isPlayerHeadCrashed = false;
    /**
     * Lean back/forward
     * 0     - back
     * 32768 - center
     * 65536 - forward
     */
    int leanF16 = 32768;
    // Lean restore rate
    const int leanRestoreRateF16 = 3276;
    // Lean rate accumulator / angular momentum
    int leanRateAccumulatorF16 = 0;
    // Track started flag - set when track has started
    bool isTrackStartedFlag = false;
    /**
     * Render-ready cache from stateBuffer[5].
     *
     * 0 - center
     * 1 - front wheel
     * 2 - back wheel
     * 3 - handlebar
     * 4 - seat
     * 5 - player
     */
    std::vector<std::unique_ptr<PhysicsElemOrMenuItem>> renderCache = std::vector<std::unique_ptr<PhysicsElemOrMenuItem>>(6);
    // Physics frame counter - incremented each physics frame
    int physicsFrameCounter;
    bool isInputAcceleration;
    bool isInputBreak;
    bool isInputBack;
    bool isInputForward;
    bool isInputUp;
    bool isInputDown;
    bool isInputLeft;
    bool isInputRight;
    // Track finished flag - set when isTrackFinished() returns true
    bool isTrackFinishedFlag;
    bool isEnableLookAhead;
    int camShiftX;
    int camShiftY;
    // Camera look-ahead limit (pixels) - clamps camShiftX and camShiftY
    int cameraLookAheadLimit;

    // Rider pose keyframes for different lean angles.
    // Indices represent skeletal joints:
    //   0: Knee
    //   1: Hip/Pelvis
    //   2: Shoulder
    //   3: Head/Neck
    //   4: Elbow/Hand
    //   5: Ankle/Foot base
    //   6: Foot peg position
    //   7: Handlebar grip position

    // Keyframes (when sprite rendering)
    const std::vector<std::vector<int>> riderPoseCenterSprites = { { 183500, -52428 }, { 262144, -163840 }, { 406323, -65536 }, { 445644, -39321 }, { 235929, 39321 }, { 16384, -144179 }, { 13107, -78643 }, { 288358, 81920 } };
    const std::vector<std::vector<int>> riderPoseLeanBackSprites = { { 190054, -111411 }, { 308019, -235929 }, { 334233, -114688 }, { 393216, -58982 }, { 262144, 98304 }, { 65536, -124518 }, { 13107, -78643 }, { 288358, 81920 } };
    const std::vector<std::vector<int>> riderPoseLeanForwardSprites = { { 157286, 13107 }, { 294912, -13107 }, { 367001, 91750 }, { 406323, 190054 }, { 347340, 72089 }, { 39321, -98304 }, { 13107, -52428 }, { 294912, 81920 } };
    // Keyframes (when line drawing)
    const std::vector<std::vector<int>> riderPoseCenterLine = { { 183500, -39321 }, { 262144, -131072 }, { 393216, -65536 }, { 458752, -39321 }, { 294912, 6553 }, { 16384, -144179 }, { 13107, -78643 }, { 288358, 85196 } };
    const std::vector<std::vector<int>> riderPoseLeanBackLine = { { 190054, -91750 }, { 255590, -235929 }, { 334233, -114688 }, { 393216, -42598 }, { 301465, 6553 }, { 65536, -78643 }, { 13107, -78643 }, { 288358, 85196 } };
    const std::vector<std::vector<int>> riderPoseLeanForwardLine = { { 157286, 13107 }, { 294912, -13107 }, { 367001, 104857 }, { 406323, 176947 }, { 347340, 72089 }, { 39321, -98304 }, { 13107, -52428 }, { 288358, 85196 } };

    std::vector<std::vector<int>> torsoAnchorOffsets;

    void resetPhysics(int startX, int startY);
    void setInputFromAI();
    void processLeanInput();
    int physicsSubstepLoop(int iterations);
    void applyForces(int bufferIndex);
    void applySpringConstraint(MotoComponent* anchor, PhysicsElemOrMenuItem* spring, MotoComponent* target, int bufferIndex, int stiffnessF16);
    void integratePosition(int fromBuffer, int toBuffer, int dtF16);
    void interpolatePosition(int toBuffer, int buf1, int buf2);
    void performPhysicsSubstep(int dtF16);
    int checkTrackCollisions(int bufferIndex);
    void applyCollisionResponse(int bufferIndex);
    void renderEngine(GameCanvas* gameCanvas, int upXF16, int upYF16);
    void renderMotoFork(GameCanvas* canvas);
    void renderWheelTires(GameCanvas* canvas);
    void renderWheelSpokes(GameCanvas* gameCanvas);
    void renderRider(GameCanvas* gameCanvas, int cosTheta, int sinTheta, int cosPhi, int sinPhi);
    void renderMotoAsLines(GameCanvas* gameCanvas, int upXF16, int upYF16, int fwdXF16, int fwdYF16);

public:
    // Physics step count / substeps per frame - passed to physicsSubstepLoop()
    inline static int physicsSubstepsPerFrame;
    // Gravity - applied as downward force
    inline static int gravityF16;
    // Normal friction coefficient - used in collision response
    inline static int normalFrictionF16;
    // Tangential friction coefficient - used in collision response
    inline static int tangentialFrictionF16;
    // Restitution/bounce coefficient - used in collision bounce
    inline static int restitutionF16;
    // Lean force coefficient X
    inline static int leanForceCoefficientXF16;
    // Lean force coefficient Y
    inline static int leanForceCoefficientYF16;
    // Global mass/inertia scaler - used to compute per-component mass
    inline static int globalMassScalerF16;
    // Default X position offset - used to initialize component X positions
    inline static int defaultXOffsetF16;
    // Default wheel angle (radians) - typically 262144 (4π)
    inline static int defaultWheelAngleF16;
    // Wheel radius values - index 0,1,2 map to different wheel sizes
    inline static std::vector<int> wheelRadiusValuesF16 = { 114688, 65536, 32768 };
    // Max angular velocity clamp - wheel angular velocity limited to ±this value
    inline static int maxAngularVelocityF16;
    // Engine momentum decay rate - exponential decay: momentum *= (65536 - decay)
    inline static int engineMomentumDecayF16;
    // Max engine momentum (throttle limit)
    inline static int maxEngineMomentumF16;
    // Engine acceleration rate - subtracted from engineMomentum when accelerating
    inline static int engineAccelerationRateF16;
    // Brake angular damping - angular vel *= (65536 - damping) when braking
    inline static int brakeAngularDampingF16;
    // Brake friction modifier - subtracted from friction when braking on wheels
    inline static int brakeFrictionModifierF16;
    // Lean input sensitivity - scales leanF16 force from input
    inline static int leanInputSensitivityF16;
    // Max leanF16 rate - leanRateAccumulator clamped to ±this value
    inline static int maxLeanRateF16;
    // Moto components (6 bike parts: chassis, wheels, rider, etc.)
    std::vector<std::unique_ptr<MotoComponent>> motoComponents;
    // Track started flag (duplicate, used in resetPhysicsState)
    bool isTrackStartedFlag2 = false;
    int renderMode;
    bool isRenderBodySprites;
    bool isRenderMotoWithSprites;
    inline static int currentLeague = 0;
    // Front wheel contact latch - becomes true when front wheel touches ground, never reset
    bool frontWheelContactLatch;
    bool isGenerateInputAI = false;

    GamePhysics(LevelLoader* levelLoader);
    int getRenderModeIndex();
    void setRenderFlags(int flags);
    void setMode(int mode);
    void setMotoLeague(int league);
    void resetPhysicsState(bool unused);
    void invertYPositions(bool isInverted);
    void setRenderMinMaxX(int minX, int maxX);
    void resetInputs();
    void updateInputs(int upDown, int leftRight);
    void enableGenerateInputAI();
    void disableGenerateInputAI();
    int updatePhysics();
    bool isTrackStarted();
    bool isTrackFinished();
    static int fastVectorLengthF16(int xF16, int yF16);
    void setEnableLookAhead(bool value);
    void setMinimalScreenWH(int minWH);
    int getCamPosX();
    int getCamPosY();
    int getRawXDistance();
    void captureRenderSnapshot();
    void prepareRenderCache();
    void renderGame(GameCanvas* gameCanvas);
};
