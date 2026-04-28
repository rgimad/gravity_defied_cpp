#include "GamePhysics.h"

#include "LevelLoader.h"
#include "MotoComponent.h"
#include "MathF16.h"
#include <algorithm>

GamePhysics::GamePhysics(LevelLoader* levelLoader)
{
    for (int i = 0; i < 6; ++i) {
        renderCache[i] = std::make_unique<PhysicsElemOrMenuItem>();
    }

    physicsFrameCounter = 0;
    renderMode = 0;
    isRenderBodySprites = false;
    isRenderMotoWithSprites = false;
    isInputAcceleration = false;
    isInputBreak = false;
    isInputBack = false;
    isInputForward = false;
    isInputUp = false;
    isInputDown = false;
    isInputLeft = false;
    isInputRight = false;
    isTrackFinishedFlag = false;
    frontWheelContactLatch = false;
    isEnableLookAhead = true;
    camShiftX = 0;
    camShiftY = 0;
    cameraLookAheadLimit = 655360;

    torsoAnchorOffsets = {
        { 45875 }, // Lean Back: Sprite center is 70% toward the shoulder
        { 32768 }, // Neutral: Sprite center is 50% (middle)
        { 52428 } // Lean Forward: Sprite center is 80% toward the shoulder
    };
    this->levelLoader = levelLoader;
    resetPhysicsState(true);
    isGenerateInputAI = false;
    captureRenderSnapshot();
    isBikeDestroyed = false;
}

int GamePhysics::getRenderModeIndex()
{
    if (isRenderBodySprites && isRenderMotoWithSprites) {
        return 3;
    } else if (isRenderMotoWithSprites) {
        return 1;
    } else {
        return isRenderBodySprites ? 2 : 0;
    }
}

void GamePhysics::setRenderFlags(int flags)
{
    isRenderBodySprites = false;
    isRenderMotoWithSprites = false;
    if ((flags & 2) != 0) {
        isRenderBodySprites = true;
    }

    if ((flags & 1) != 0) {
        isRenderMotoWithSprites = true;
    }
}

void GamePhysics::setMode(int mode)
{
    renderMode = mode;
    switch (mode) {
    case 1:
    default:
        physicsSubstepsPerFrame = 1310;
        gravityF16 = 1638400;
        setMotoLeague(1);
        resetPhysicsState(true);
    }
}

void GamePhysics::setMotoLeague(int league)
{
    currentLeague = league;
    normalFrictionF16 = 45875;
    tangentialFrictionF16 = 13107;
    restitutionF16 = 39321;
    globalMassScalerF16 = 1310720;
    defaultWheelAngleF16 = 262144;
    engineMomentumDecayF16 = 6553;
    switch (league) {
    case 0:
    default:
        leanForceCoefficientXF16 = 19660;
        leanForceCoefficientYF16 = 19660;
        maxAngularVelocityF16 = 1114112;
        maxEngineMomentumF16 = 52428800;
        engineAccelerationRateF16 = 3276800;
        brakeAngularDampingF16 = 327;
        brakeFrictionModifierF16 = 0;
        leanInputSensitivityF16 = 32768;
        maxLeanRateF16 = 327680;
        defaultXOffsetF16 = 19660800;
        break;
    case 1:
        leanForceCoefficientXF16 = 32768;
        leanForceCoefficientYF16 = 32768;
        maxAngularVelocityF16 = 1114112;
        maxEngineMomentumF16 = 65536000;
        engineAccelerationRateF16 = 3276800;
        brakeAngularDampingF16 = 6553;
        brakeFrictionModifierF16 = 26214;
        leanInputSensitivityF16 = 26214;
        maxLeanRateF16 = 327680;
        defaultXOffsetF16 = 19660800;
        break;
    case 2:
        leanForceCoefficientXF16 = 32768;
        leanForceCoefficientYF16 = 32768;
        maxAngularVelocityF16 = 1310720;
        maxEngineMomentumF16 = 75366400;
        engineAccelerationRateF16 = 3473408;
        brakeAngularDampingF16 = 6553;
        brakeFrictionModifierF16 = 26214;
        leanInputSensitivityF16 = 39321;
        maxLeanRateF16 = 327680;
        defaultXOffsetF16 = 21626880;
        break;
    case 3:
        leanForceCoefficientXF16 = 32768;
        leanForceCoefficientYF16 = 32768;
        maxAngularVelocityF16 = 1441792;
        maxEngineMomentumF16 = 78643200;
        engineAccelerationRateF16 = 3538944;
        brakeAngularDampingF16 = 6553;
        brakeFrictionModifierF16 = 26214;
        leanInputSensitivityF16 = 65536;
        maxLeanRateF16 = 1310720;
        defaultXOffsetF16 = 21626880;
    }

    resetPhysicsState(true);
}

void GamePhysics::resetPhysicsState(bool unused)
{
    (void)unused;
    physicsFrameCounter = 0;
    resetPhysics(levelLoader->getStartPosX(), levelLoader->getStartPosY());
    engineMomentumF16 = 0;
    leanRateAccumulatorF16 = 0;
    isBikeDestroyed = false;
    isPlayerHeadCrashed = false;
    isTrackFinishedFlag = false;
    frontWheelContactLatch = false;
    isGenerateInputAI = false;
    isTrackStartedFlag2 = false;
    isTrackStartedFlag = false;
    levelLoader->gameLevel->setShadowBoundariesHalf(motoComponents[2]->stateBuffers[5]->xF16 + 98304 - wheelRadiusValuesF16[0], motoComponents[1]->stateBuffers[5]->xF16 - 98304 + wheelRadiusValuesF16[0]);
}

void GamePhysics::invertYPositions(bool isInverted)
{
    int yOffset = (isInverted ? 65536 : -65536) << 1;

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            motoComponents[i]->stateBuffers[j]->yF16 += yOffset;
        }
    }
}

void GamePhysics::resetPhysics(int startX, int startY)
{
    if (motoComponents.empty()) {
        motoComponents = std::vector<std::unique_ptr<MotoComponent>>(6);
    }

    if (springConstraints.empty()) {
        springConstraints = std::vector<std::unique_ptr<PhysicsElemOrMenuItem>>(10);
    }

    int massValue = 0;
    int8_t radiusIdx = 0;
    int xOffset = 0;
    int yOffset = 0;

    int i;
    for (i = 0; i < 6; ++i) {
        short leanInf = 0;
        switch (i) {
        case 0: // Chassis (center)
            radiusIdx = 1;
            massValue = 360448;
            xOffset = 0;
            yOffset = 0;
            break;
        case 1: // Front wheel
            radiusIdx = 0;
            massValue = 98304;
            xOffset = 229376;
            yOffset = 0;
            break;
        case 2: // Back wheel
            radiusIdx = 0;
            massValue = 360448;
            xOffset = -229376;
            yOffset = 0;
            leanInf = 21626;
            break;
        case 3: // Handlebar
            radiusIdx = 1;
            massValue = 229376;
            xOffset = 131072;
            yOffset = 196608;
            break;
        case 4: // Seat
            radiusIdx = 1;
            massValue = 229376;
            xOffset = -131072;
            yOffset = 196608;
            break;
        case 5: // Rider head
            radiusIdx = 2;
            massValue = 294912;
            xOffset = 0;
            yOffset = 327680;
        }

        if (motoComponents[i] == nullptr) {
            motoComponents[i] = std::make_unique<MotoComponent>();
        }

        motoComponents[i]->reset();
        motoComponents[i]->radiusF16 = wheelRadiusValuesF16[radiusIdx];
        motoComponents[i]->radiusIndex = radiusIdx;
        motoComponents[i]->inverseMassF16 = (int)((int64_t)((int)(281474976710656L / (int64_t)massValue >> 16)) * (int64_t)globalMassScalerF16 >> 16);
        motoComponents[i]->stateBuffers[readBufferIndex]->xF16 = startX + xOffset;
        motoComponents[i]->stateBuffers[readBufferIndex]->yF16 = startY + yOffset;
        motoComponents[i]->stateBuffers[5]->xF16 = startX + xOffset;
        motoComponents[i]->stateBuffers[5]->yF16 = startY + yOffset;
        motoComponents[i]->leanInfluenceF16 = leanInf;
    }

    for (i = 0; i < 10; ++i) {
        if (springConstraints[i] == nullptr) {
            springConstraints[i] = std::make_unique<PhysicsElemOrMenuItem>();
        }

        springConstraints[i]->setToZeros();
        springConstraints[i]->xF16 = defaultXOffsetF16;
        springConstraints[i]->angleF16 = defaultWheelAngleF16;
    }

    springConstraints[0]->yF16 = 229376;
    springConstraints[1]->yF16 = 229376;
    springConstraints[2]->yF16 = 236293;
    springConstraints[3]->yF16 = 236293;
    springConstraints[4]->yF16 = 262144;
    springConstraints[5]->yF16 = 219814;
    springConstraints[6]->yF16 = 219814;
    springConstraints[7]->yF16 = 185363;
    springConstraints[8]->yF16 = 185363;
    springConstraints[9]->yF16 = 327680;
    springConstraints[5]->angleF16 = (int)((int64_t)defaultWheelAngleF16 * 45875L >> 16);
    springConstraints[6]->xF16 = (int)(6553L * (int64_t)defaultXOffsetF16 >> 16);
    springConstraints[5]->xF16 = (int)(6553L * (int64_t)defaultXOffsetF16 >> 16);
    springConstraints[9]->xF16 = (int)(72089L * (int64_t)defaultXOffsetF16 >> 16);
    springConstraints[8]->xF16 = (int)(72089L * (int64_t)defaultXOffsetF16 >> 16);
    springConstraints[7]->xF16 = (int)(72089L * (int64_t)defaultXOffsetF16 >> 16);
}

void GamePhysics::setRenderMinMaxX(int minX, int maxX)
{
    levelLoader->setLevelBounds(minX, maxX);
}

void GamePhysics::resetInputs()
{
    isInputUp = isInputDown = isInputRight = isInputLeft = false;
}

void GamePhysics::updateInputs(int upDown, int leftRight)
{
    if (!isGenerateInputAI) {
        isInputUp = isInputDown = isInputRight = isInputLeft = false;
        if (upDown > 0) {
            isInputUp = true;
        } else if (upDown < 0) {
            isInputDown = true;
        }

        if (leftRight > 0) {
            isInputRight = true;
            return;
        }

        if (leftRight < 0) {
            isInputLeft = true;
        }
    }
}

void GamePhysics::enableGenerateInputAI()
{
    resetPhysicsState(true);
    isGenerateInputAI = true;
}

void GamePhysics::disableGenerateInputAI()
{
    isGenerateInputAI = false;
}

void GamePhysics::setInputFromAI()
{
    int dx = motoComponents[1]->stateBuffers[readBufferIndex]->xF16 - motoComponents[2]->stateBuffers[readBufferIndex]->xF16;
    int dy = motoComponents[1]->stateBuffers[readBufferIndex]->yF16 - motoComponents[2]->stateBuffers[readBufferIndex]->yF16;
    int dist = fastVectorLengthF16(dx, dy);
    dy = (int)(((int64_t)dy << 32) / (int64_t)dist >> 16);
    isInputBreak = false;
    if (dy < 0) {
        isInputBack = true;
        isInputForward = false;
    } else if (dy > 0) {
        isInputForward = true;
        isInputBack = false;
    }

    bool pitchSign; // tilt direction matches horizontal motion difference
    if ((!(pitchSign = (motoComponents[2]->stateBuffers[readBufferIndex]->yF16 - motoComponents[0]->stateBuffers[readBufferIndex]->yF16 > 0 ? 1 : -1) * (motoComponents[2]->stateBuffers[readBufferIndex]->velocityXF16 - motoComponents[0]->stateBuffers[readBufferIndex]->velocityXF16 > 0 ? 1 : -1) > 0) || !isInputForward) && (pitchSign || !isInputBack)) {
        isInputAcceleration = false;
    } else {
        isInputAcceleration = true;
    }
}

void GamePhysics::processLeanInput()
{
    if (!isBikeDestroyed) {
        int dx = motoComponents[1]->stateBuffers[readBufferIndex]->xF16 - motoComponents[2]->stateBuffers[readBufferIndex]->xF16;
        int dy = motoComponents[1]->stateBuffers[readBufferIndex]->yF16 - motoComponents[2]->stateBuffers[readBufferIndex]->yF16;
        int dist = fastVectorLengthF16(dx, dy);
        dx = (int)(((int64_t)dx << 32) / (int64_t)dist >> 16);
        dy = (int)(((int64_t)dy << 32) / (int64_t)dist >> 16);
        if (isInputAcceleration && engineMomentumF16 >= -maxEngineMomentumF16) {
            engineMomentumF16 -= engineAccelerationRateF16;
        }

        if (isInputBreak) {
            engineMomentumF16 = 0;
            motoComponents[1]->stateBuffers[readBufferIndex]->angularVelocityF16 = (int)((int64_t)motoComponents[1]->stateBuffers[readBufferIndex]->angularVelocityF16 * (int64_t)(65536 - brakeAngularDampingF16) >> 16);
            motoComponents[2]->stateBuffers[readBufferIndex]->angularVelocityF16 = (int)((int64_t)motoComponents[2]->stateBuffers[readBufferIndex]->angularVelocityF16 * (int64_t)(65536 - brakeAngularDampingF16) >> 16);
            if (motoComponents[1]->stateBuffers[readBufferIndex]->angularVelocityF16 < 6553) {
                motoComponents[1]->stateBuffers[readBufferIndex]->angularVelocityF16 = 0;
            }

            if (motoComponents[2]->stateBuffers[readBufferIndex]->angularVelocityF16 < 6553) {
                motoComponents[2]->stateBuffers[readBufferIndex]->angularVelocityF16 = 0;
            }
        }

        motoComponents[0]->inverseMassF16 = (int)(11915L * (int64_t)globalMassScalerF16 >> 16);
        motoComponents[0]->inverseMassF16 = (int)(11915L * (int64_t)globalMassScalerF16 >> 16);
        motoComponents[4]->inverseMassF16 = (int)(18724L * (int64_t)globalMassScalerF16 >> 16);
        motoComponents[3]->inverseMassF16 = (int)(18724L * (int64_t)globalMassScalerF16 >> 16);
        motoComponents[1]->inverseMassF16 = (int)(43690L * (int64_t)globalMassScalerF16 >> 16);
        motoComponents[2]->inverseMassF16 = (int)(11915L * (int64_t)globalMassScalerF16 >> 16);
        motoComponents[5]->inverseMassF16 = (int)(14563L * (int64_t)globalMassScalerF16 >> 16);
        if (isInputBack) {
            motoComponents[0]->inverseMassF16 = (int)(18724L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[4]->inverseMassF16 = (int)(14563L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[3]->inverseMassF16 = (int)(18724L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[1]->inverseMassF16 = (int)(43690L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[2]->inverseMassF16 = (int)(10082L * (int64_t)globalMassScalerF16 >> 16);
        } else if (isInputForward) {
            motoComponents[0]->inverseMassF16 = (int)(18724L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[4]->inverseMassF16 = (int)(18724L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[3]->inverseMassF16 = (int)(14563L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[1]->inverseMassF16 = (int)(26214L * (int64_t)globalMassScalerF16 >> 16);
            motoComponents[2]->inverseMassF16 = (int)(11915L * (int64_t)globalMassScalerF16 >> 16);
        }

        if (isInputBack || isInputForward) {
            int negDy = -dy;
            PhysicsElemOrMenuItem* elem;
            int interpFactor;
            int sensitivity;
            int forceX;
            int forceY;
            int riderForceX;
            int riderForceY;
            if (isInputBack && leanRateAccumulatorF16 > -maxLeanRateF16) {
                interpFactor = 65536;
                if (leanRateAccumulatorF16 < 0) {
                    interpFactor = (int)(((int64_t)(maxLeanRateF16 - (leanRateAccumulatorF16 < 0 ? -leanRateAccumulatorF16 : leanRateAccumulatorF16)) << 32) / (int64_t)maxLeanRateF16 >> 16);
                }

                sensitivity = (int)((int64_t)leanInputSensitivityF16 * (int64_t)interpFactor >> 16);
                forceX = (int)((int64_t)negDy * (int64_t)sensitivity >> 16);
                forceY = (int)((int64_t)dx * (int64_t)sensitivity >> 16);
                riderForceX = (int)((int64_t)dx * (int64_t)sensitivity >> 16);
                riderForceY = (int)((int64_t)dy * (int64_t)sensitivity >> 16);
                if (leanF16 > 32768) {
                    leanF16 = leanF16 - 1638 < 0 ? 0 : leanF16 - 1638;
                } else {
                    leanF16 = leanF16 - 3276 < 0 ? 0 : leanF16 - 3276;
                }

                // seat
                elem = motoComponents[4]->stateBuffers[readBufferIndex].get();
                elem->velocityXF16 -= forceX;
                elem = motoComponents[4]->stateBuffers[readBufferIndex].get();
                elem->velocityYF16 -= forceY;
                // handlebar
                elem = motoComponents[3]->stateBuffers[readBufferIndex].get();
                elem->velocityXF16 += forceX;
                elem = motoComponents[3]->stateBuffers[readBufferIndex].get();
                elem->velocityYF16 += forceY;
                // rider
                elem = motoComponents[5]->stateBuffers[readBufferIndex].get();
                elem->velocityXF16 -= riderForceX;
                elem = motoComponents[5]->stateBuffers[readBufferIndex].get();
                elem->velocityYF16 -= riderForceY;
            }

            if (isInputForward && leanRateAccumulatorF16 < maxLeanRateF16) {
                interpFactor = 65536;
                if (leanRateAccumulatorF16 > 0) {
                    interpFactor = (int)(((int64_t)(maxLeanRateF16 - leanRateAccumulatorF16) << 32) / (int64_t)maxLeanRateF16 >> 16);
                }

                sensitivity = (int)((int64_t)leanInputSensitivityF16 * (int64_t)interpFactor >> 16);
                forceX = (int)((int64_t)negDy * (int64_t)sensitivity >> 16);
                forceY = (int)((int64_t)dx * (int64_t)sensitivity >> 16);
                riderForceX = (int)((int64_t)dx * (int64_t)sensitivity >> 16);
                riderForceY = (int)((int64_t)dy * (int64_t)sensitivity >> 16);
                if (leanF16 > 32768) {
                    leanF16 = leanF16 + 1638 < 65536 ? leanF16 + 1638 : 65536;
                } else {
                    leanF16 = leanF16 + 3276 < 65536 ? leanF16 + 3276 : 65536;
                }

                // seat
                elem = motoComponents[4]->stateBuffers[readBufferIndex].get();
                elem->velocityXF16 += forceX;
                elem = motoComponents[4]->stateBuffers[readBufferIndex].get();
                elem->velocityYF16 += forceY;
                // handlebar
                elem = motoComponents[3]->stateBuffers[readBufferIndex].get();
                elem->velocityXF16 -= forceX;
                elem = motoComponents[3]->stateBuffers[readBufferIndex].get();
                elem->velocityYF16 -= forceY;
                // rider
                elem = motoComponents[5]->stateBuffers[readBufferIndex].get();
                elem->velocityXF16 += riderForceX;
                elem = motoComponents[5]->stateBuffers[readBufferIndex].get();
                elem->velocityYF16 += riderForceY;
            }

            return;
        }

        // Auto-center lean when no input
        if (leanF16 < 26214) {
            leanF16 += 3276;
            return;
        }
        if (leanF16 > 39321) {
            leanF16 -= 3276;
            return;
        }
        leanF16 = 32768;
    }
}

int GamePhysics::updatePhysics()
{
    isInputAcceleration = isInputUp;
    isInputBreak = isInputDown;
    isInputBack = isInputLeft;
    isInputForward = isInputRight;
    if (isGenerateInputAI) {
        setInputFromAI();
    }

    GameCanvas::flagAnimation();
    processLeanInput();
    int result;
    if ((result = physicsSubstepLoop(physicsSubstepsPerFrame)) != 5 && !isPlayerHeadCrashed) {
        if (isBikeDestroyed) {
            return 3;
        } else if (isTrackStarted()) {
            frontWheelContactLatch = false;
            return 4;
        } else {
            return result;
        }
    } else {
        return 5;
    }
}

bool GamePhysics::isTrackStarted()
{
    return motoComponents[1]->stateBuffers[readBufferIndex]->xF16 < levelLoader->getStartFlagX();
}

bool GamePhysics::isTrackFinished()
{
    return motoComponents[1]->stateBuffers[writeBufferIndex]->xF16 > levelLoader->getFinishFlagX() || motoComponents[2]->stateBuffers[writeBufferIndex]->xF16 > levelLoader->getFinishFlagX();
}

int GamePhysics::physicsSubstepLoop(int iterations)
{
    bool isTrackFinishedPrev = isTrackFinishedFlag;
    int low = 0;
    int high = iterations;

label77:
    do {
        int collisionResult;
        while (low < iterations) {
            performPhysicsSubstep(high - low);
            if (!isTrackFinishedPrev && isTrackFinished()) {
                collisionResult = 3;
            } else {
                collisionResult = checkTrackCollisions(writeBufferIndex);
            }

            // Finish reached?
            if (!isTrackFinishedPrev && isTrackFinishedFlag) {
                if (collisionResult != 3) {
                    return 2;
                }

                return 1;
            }

            if (collisionResult == 0) {
                high = (low + high) >> 1;
                goto label77;
            }

            if (collisionResult == 3) {
                isTrackFinishedFlag = true;
                high = (low + high) >> 1;
            } else {
                int res;
                if (collisionResult == 1) {
                    do {
                        applyCollisionResponse(writeBufferIndex);
                        if ((res = checkTrackCollisions(writeBufferIndex)) == 0) {
                            return 5;
                        }
                    } while (res != 2);
                }

                low = high;
                high = iterations;
                readBufferIndex = readBufferIndex == 1 ? 0 : 1;
                writeBufferIndex = writeBufferIndex == 1 ? 0 : 1;
            }
        }

        // Check if bike breaks (distance between wheels too small or too large)
        int dx = motoComponents[1]->stateBuffers[readBufferIndex]->xF16 - motoComponents[2]->stateBuffers[readBufferIndex]->xF16;
        int dy = motoComponents[1]->stateBuffers[readBufferIndex]->yF16 - motoComponents[2]->stateBuffers[readBufferIndex]->yF16;
        int distSq = (int)((int64_t)dx * (int64_t)dx >> 16) + (int)((int64_t)dy * (int64_t)dy >> 16);

        if (distSq < 983040) {
            isBikeDestroyed = true;
        }

        if (distSq > 4587520) {
            isBikeDestroyed = true;
        }

        return 0;
    } while (((high = (low + high) >> 1) - low < 0 ? -(high - low) : high - low) >= 65);

    return 5;
}

void GamePhysics::applyForces(int bufferIndex)
{
    PhysicsElemOrMenuItem* elem;
    int i;

    for (i = 0; i < 6; ++i) {
        MotoComponent* comp = motoComponents[i].get();
        elem = comp->stateBuffers[bufferIndex].get();
        elem->forceAccumXF16 = 0;

        elem->forceAccumYF16 = 0;
        elem->torqueF16 = 0;
        // Apply gravity: force = gravity / inverseMass = gravity * mass
        elem->forceAccumYF16 -= (int)(((int64_t)gravityF16 << 32) / (int64_t)comp->inverseMassF16 >> 16);
    }

    // Apply spring constraints between bike components

    if (!isBikeDestroyed) {
        // Keep bike together while it's intact
        applySpringConstraint(motoComponents[0].get(), springConstraints[1].get(), motoComponents[2].get(), bufferIndex, 65536);
        applySpringConstraint(motoComponents[0].get(), springConstraints[0].get(), motoComponents[1].get(), bufferIndex, 65536);
        applySpringConstraint(motoComponents[2].get(), springConstraints[6].get(), motoComponents[4].get(), bufferIndex, 131072);
        applySpringConstraint(motoComponents[1].get(), springConstraints[5].get(), motoComponents[3].get(), bufferIndex, 131072);
    }

    applySpringConstraint(motoComponents[0].get(), springConstraints[2].get(), motoComponents[3].get(), bufferIndex, 65536);
    applySpringConstraint(motoComponents[0].get(), springConstraints[3].get(), motoComponents[4].get(), bufferIndex, 65536);
    applySpringConstraint(motoComponents[3].get(), springConstraints[4].get(), motoComponents[4].get(), bufferIndex, 65536);
    applySpringConstraint(motoComponents[5].get(), springConstraints[8].get(), motoComponents[3].get(), bufferIndex, 65536);
    applySpringConstraint(motoComponents[5].get(), springConstraints[7].get(), motoComponents[4].get(), bufferIndex, 65536);
    applySpringConstraint(motoComponents[5].get(), springConstraints[9].get(), motoComponents[0].get(), bufferIndex, 65536);

    // Apply engine torque to back wheel and decay engine momentum
    elem = motoComponents[2]->stateBuffers[bufferIndex].get();
    engineMomentumF16 = (int)((int64_t)engineMomentumF16 * (int64_t)(65536 - engineMomentumDecayF16) >> 16);
    elem->torqueF16 = engineMomentumF16;

    // Clamp angular velocity
    if (elem->angularVelocityF16 > maxAngularVelocityF16) {
        elem->angularVelocityF16 = maxAngularVelocityF16;
    }
    if (elem->angularVelocityF16 < -maxAngularVelocityF16) {
        elem->angularVelocityF16 = -maxAngularVelocityF16;
    }

    // Calculate center of mass velocity and clamp individual component velocities
    int totalVx = 0;
    int totalVy = 0;
    for (i = 0; i < 6; ++i) {
        totalVx += motoComponents[i]->stateBuffers[bufferIndex]->velocityXF16;
        totalVy += motoComponents[i]->stateBuffers[bufferIndex]->velocityYF16;
    }

    // Average velocity (center of mass)
    int avgVx = (int)(((int64_t)totalVx << 32) / 393216L >> 16);
    int avgVy = (int)(((int64_t)totalVy << 32) / 393216L >> 16);
    int maxRelVel = 0;

    for (i = 0; i < 6; ++i) {
        int relVx = motoComponents[i]->stateBuffers[bufferIndex]->velocityXF16 - avgVx;
        int relVy = motoComponents[i]->stateBuffers[bufferIndex]->velocityYF16 - avgVy;
        if ((maxRelVel = fastVectorLengthF16(relVx, relVy)) > 1966080) {
            // Normalize and clamp relative velocity
            int normX = (int)(((int64_t)relVx << 32) / (int64_t)maxRelVel >> 16);
            int normY = (int)(((int64_t)relVy << 32) / (int64_t)maxRelVel >> 16);
            motoComponents[i]->stateBuffers[bufferIndex]->velocityXF16 -= normX;
            motoComponents[i]->stateBuffers[bufferIndex]->velocityYF16 -= normY;
        }
    }

    // Update lean rate accumulator based on back wheel vs chassis movement
    int backAboveCenter = motoComponents[2]->stateBuffers[bufferIndex]->yF16 - motoComponents[0]->stateBuffers[bufferIndex]->yF16 >= 0 ? 1 : -1;
    int backVelFaster = motoComponents[2]->stateBuffers[bufferIndex]->velocityXF16 - motoComponents[0]->stateBuffers[bufferIndex]->velocityXF16 >= 0 ? 1 : -1;
    if (backAboveCenter * backVelFaster > 0) {
        leanRateAccumulatorF16 = maxRelVel;
    } else {
        leanRateAccumulatorF16 = -maxRelVel;
    }
}

int GamePhysics::fastVectorLengthF16(int xF16, int yF16)
{
    int absXF16 = xF16 < 0 ? -xF16 : xF16;
    int absYF16;
    int maxAbs;
    int minAbs;
    if ((absYF16 = yF16 < 0 ? -yF16 : yF16) >= absXF16) {
        maxAbs = absYF16;
        minAbs = absXF16;
    } else {
        maxAbs = absXF16;
        minAbs = absYF16;
    }

    // fast 2D vector length approximation
    return (int)(64448L * (int64_t)maxAbs >> 16) + (int)(28224L * (int64_t)minAbs >> 16);
}

void GamePhysics::applySpringConstraint(MotoComponent* anchor, PhysicsElemOrMenuItem* spring, MotoComponent* target, int bufferIndex, int stiffnessF16)
{
    PhysicsElemOrMenuItem* anchorElem = anchor->stateBuffers[bufferIndex].get();
    PhysicsElemOrMenuItem* targetElem = target->stateBuffers[bufferIndex].get();
    int dx = anchorElem->xF16 - targetElem->xF16;
    int dy = anchorElem->yF16 - targetElem->yF16;
    int dist;
    if (((dist = fastVectorLengthF16(dx, dy)) < 0 ? -dist : dist) >= 3) {
        dx = (int)(((int64_t)dx << 32) / (int64_t)dist >> 16);
        dy = (int)(((int64_t)dy << 32) / (int64_t)dist >> 16);
        int springExtension = dist - spring->yF16;
        int forceX = (int)((int64_t)dx * (int64_t)((int)((int64_t)springExtension * (int64_t)spring->xF16 >> 16)) >> 16);
        int forceY = (int)((int64_t)dy * (int64_t)((int)((int64_t)springExtension * (int64_t)spring->xF16 >> 16)) >> 16);
        int relVelX = anchorElem->velocityXF16 - targetElem->velocityXF16;
        int relVelY = anchorElem->velocityYF16 - targetElem->velocityYF16;
        int damping = (int)((int64_t)((int)((int64_t)dx * (int64_t)relVelX >> 16) + (int)((int64_t)dy * (int64_t)relVelY >> 16)) * (int64_t)spring->angleF16 >> 16);
        forceX += (int)((int64_t)dx * (int64_t)damping >> 16);
        forceY += (int)((int64_t)dy * (int64_t)damping >> 16);
        forceX = (int)((int64_t)forceX * (int64_t)stiffnessF16 >> 16);
        forceY = (int)((int64_t)forceY * (int64_t)stiffnessF16 >> 16);
        anchorElem->forceAccumXF16 -= forceX;
        anchorElem->forceAccumYF16 -= forceY;
        targetElem->forceAccumXF16 += forceX;
        targetElem->forceAccumYF16 += forceY;
    }
}

void GamePhysics::integratePosition(int fromBuffer, int toBuffer, int dtF16)
{
    for (int i = 0; i < 6; ++i) {
        PhysicsElemOrMenuItem* fromElem = motoComponents[i]->stateBuffers[fromBuffer].get();
        PhysicsElemOrMenuItem* toElem;
        (toElem = motoComponents[i]->stateBuffers[toBuffer].get())->xF16 = (int)((int64_t)fromElem->velocityXF16 * (int64_t)dtF16 >> 16);
        toElem->yF16 = (int)((int64_t)fromElem->velocityYF16 * (int64_t)dtF16 >> 16);
        int invMassDt = (int)((int64_t)dtF16 * (int64_t)motoComponents[i]->inverseMassF16 >> 16);
        toElem->velocityXF16 = (int)((int64_t)fromElem->forceAccumXF16 * (int64_t)invMassDt >> 16);
        toElem->velocityYF16 = (int)((int64_t)fromElem->forceAccumYF16 * (int64_t)invMassDt >> 16);
    }
}

void GamePhysics::interpolatePosition(int toBuffer, int buf1, int buf2)
{
    for (int i = 0; i < 6; ++i) {
        PhysicsElemOrMenuItem* toElem = motoComponents[i]->stateBuffers[toBuffer].get();
        PhysicsElemOrMenuItem* elem1 = motoComponents[i]->stateBuffers[buf1].get();
        PhysicsElemOrMenuItem* elem2 = motoComponents[i]->stateBuffers[buf2].get();
        toElem->xF16 = elem1->xF16 + (elem2->xF16 >> 1);
        toElem->yF16 = elem1->yF16 + (elem2->yF16 >> 1);
        toElem->velocityXF16 = elem1->velocityXF16 + (elem2->velocityXF16 >> 1);
        toElem->velocityYF16 = elem1->velocityYF16 + (elem2->velocityYF16 >> 1);
    }
}

void GamePhysics::performPhysicsSubstep(int dtF16)
{
    applyForces(readBufferIndex);
    integratePosition(readBufferIndex, 2, dtF16);
    interpolatePosition(4, readBufferIndex, 2);
    applyForces(4);
    integratePosition(4, 3, dtF16 >> 1);
    interpolatePosition(4, readBufferIndex, 3);
    interpolatePosition(writeBufferIndex, readBufferIndex, 2);
    interpolatePosition(writeBufferIndex, writeBufferIndex, 3);

    for (int i = 1; i <= 2; ++i) {
        PhysicsElemOrMenuItem* fromElem = motoComponents[i]->stateBuffers[readBufferIndex].get();
        PhysicsElemOrMenuItem* toElem;
        (toElem = motoComponents[i]->stateBuffers[writeBufferIndex].get())->angleF16 = fromElem->angleF16 + (int)((int64_t)dtF16 * (int64_t)fromElem->angularVelocityF16 >> 16);
        toElem->angularVelocityF16 = fromElem->angularVelocityF16 + (int)((int64_t)dtF16 * (int64_t)((int)((int64_t)motoComponents[i]->leanInfluenceF16 * (int64_t)fromElem->torqueF16 >> 16)) >> 16);
    }
}

int GamePhysics::checkTrackCollisions(int bufferIndex)
{
    // 2=no collision, 1=collision with response, 0=collision without response
    int8_t collisionResult = 2;
    int maxXF16 = std::max({ motoComponents[1]->stateBuffers[bufferIndex]->xF16,
        motoComponents[2]->stateBuffers[bufferIndex]->xF16, motoComponents[5]->stateBuffers[bufferIndex]->xF16 });
    int minXF16 = std::min({ motoComponents[1]->stateBuffers[bufferIndex]->xF16,
        motoComponents[2]->stateBuffers[bufferIndex]->xF16, motoComponents[5]->stateBuffers[bufferIndex]->xF16 });
    levelLoader->updateVisibleSegmentRange(minXF16 - wheelRadiusValuesF16[0], maxXF16 + wheelRadiusValuesF16[0], motoComponents[5]->stateBuffers[bufferIndex]->yF16);

    // Calculate normalized bike direction vector
    int dxF16 = motoComponents[1]->stateBuffers[bufferIndex]->xF16 - motoComponents[2]->stateBuffers[bufferIndex]->xF16;
    int dyF16 = motoComponents[1]->stateBuffers[bufferIndex]->yF16 - motoComponents[2]->stateBuffers[bufferIndex]->yF16;
    int dist = fastVectorLengthF16(dxF16, dyF16);
    dxF16 = (int)(((int64_t)dxF16 << 32) / (int64_t)dist >> 16);
    int negDyF16 = -((int)(((int64_t)dyF16 << 32) / (int64_t)dist >> 16));
    int normalXF16 = dxF16;

    for (int compIdx = 0; compIdx < 6; ++compIdx) {
        // Skip handlebar and seat for collision
        if (compIdx != 4 && compIdx != 3) {
            PhysicsElemOrMenuItem* elem = motoComponents[compIdx]->stateBuffers[bufferIndex].get();

            // Offset chassis position for accurate collision
            if (compIdx == 0) {
                elem->xF16 += (int)((int64_t)negDyF16 * 65536L >> 16);
                elem->yF16 += (int)((int64_t)normalXF16 * 65536L >> 16);
            }

            int isCollision = levelLoader->checkSegmentCollisions(elem, motoComponents[compIdx]->radiusIndex);

            // Restore chassis position
            if (compIdx == 0) {
                elem->xF16 -= (int)((int64_t)negDyF16 * 65536L >> 16);
                elem->yF16 -= (int)((int64_t)normalXF16 * 65536L >> 16);
            }

            // Store collision normal from LevelLoader
            collisionNormalXF16 = levelLoader->lastCollisionNormalXF16;
            collisionNormalYF16 = levelLoader->lastCollisionNormalYF16;

            // Check for player head crash
            if (compIdx == 5 && isCollision != 2) {
                isPlayerHeadCrashed = true;
            }

            // Front wheel contact latch (never resets)
            if (compIdx == 1 && isCollision != 2) {
                frontWheelContactLatch = true;
            }

            if (isCollision == 1) {
                lastCollidedComponentIndex = compIdx;
                collisionResult = 1;
            } else if (isCollision == 0) {
                lastCollidedComponentIndex = compIdx;
                collisionResult = 0;
                break;
            }
        }
    }

    return collisionResult;
}

void GamePhysics::applyCollisionResponse(int bufferIndex)
{
    MotoComponent* collidedComp = motoComponents[lastCollidedComponentIndex].get();
    PhysicsElemOrMenuItem* elem = collidedComp->stateBuffers[bufferIndex].get();

    // Push element out of collision along normal
    elem->xF16 += (int)((int64_t)collisionNormalXF16 * 3276L >> 16);
    elem->yF16 += (int)((int64_t)collisionNormalYF16 * 3276L >> 16);

    int frictionNormalF16;
    int frictionTangentialF16;
    int restitutionLocalF16;
    int leanForceXF16;
    int leanForceYF16;

    // Apply brake friction modifier when braking on wheels
    if (isInputBreak && (lastCollidedComponentIndex == 2 || lastCollidedComponentIndex == 1) && elem->angularVelocityF16 < 6553) {
        frictionNormalF16 = normalFrictionF16 - brakeFrictionModifierF16;
        frictionTangentialF16 = 13107;
        restitutionLocalF16 = 39321;
        leanForceXF16 = 26214 - brakeFrictionModifierF16;
        leanForceYF16 = 26214 - brakeFrictionModifierF16;
    } else {
        frictionNormalF16 = normalFrictionF16;
        frictionTangentialF16 = tangentialFrictionF16;
        restitutionLocalF16 = restitutionF16;
        leanForceXF16 = leanForceCoefficientXF16;
        leanForceYF16 = leanForceCoefficientYF16;
    }

    // Normalize collision normal
    int normalMag = fastVectorLengthF16(collisionNormalXF16, collisionNormalYF16);
    collisionNormalXF16 = (int)(((int64_t)collisionNormalXF16 << 32) / (int64_t)normalMag >> 16);
    collisionNormalYF16 = (int)(((int64_t)collisionNormalYF16 << 32) / (int64_t)normalMag >> 16);

    int velXF16 = elem->velocityXF16;
    int velYF16 = elem->velocityYF16;

    // Calculate velocity in collision frame (normal and tangential components)
    int velNormal = -((int)((int64_t)velXF16 * (int64_t)collisionNormalXF16 >> 16) + (int)((int64_t)velYF16 * (int64_t)collisionNormalYF16 >> 16));
    int velTangent = -((int)((int64_t)velXF16 * (int64_t)(-collisionNormalYF16) >> 16) + (int)((int64_t)velYF16 * (int64_t)collisionNormalXF16 >> 16));

    // Apply friction to angular velocity and tangential velocity
    int newAngularVel = (int)((int64_t)frictionNormalF16 * (int64_t)elem->angularVelocityF16 >> 16) - (int)((int64_t)frictionTangentialF16 * (int64_t)((int)(((int64_t)velTangent << 32) / (int64_t)collidedComp->radiusF16 >> 16)) >> 16);
    int newVelTangent = (int)((int64_t)leanForceXF16 * (int64_t)velTangent >> 16) - (int)((int64_t)restitutionLocalF16 * (int64_t)((int)((int64_t)elem->angularVelocityF16 * (int64_t)collidedComp->radiusF16 >> 16)) >> 16);
    int newVelNormal = -((int)((int64_t)leanForceYF16 * (int64_t)velNormal >> 16));

    // Transform back to world coordinates
    int newVelXF16 = (int)((int64_t)(-newVelTangent) * (int64_t)(-collisionNormalYF16) >> 16);
    int newVelYF16 = (int)((int64_t)(-newVelTangent) * (int64_t)collisionNormalXF16 >> 16);
    int normalVelXF16 = (int)((int64_t)(-newVelNormal) * (int64_t)collisionNormalXF16 >> 16);
    int normalVelYF16 = (int)((int64_t)(-newVelNormal) * (int64_t)collisionNormalYF16 >> 16);

    elem->angularVelocityF16 = newAngularVel;
    elem->velocityXF16 = newVelXF16 + normalVelXF16;
    elem->velocityYF16 = newVelYF16 + normalVelYF16;
}

void GamePhysics::setEnableLookAhead(bool value)
{
    isEnableLookAhead = value;
}

void GamePhysics::setMinimalScreenWH(int minWH)
{
    // Set camera look-ahead limit based on minimum screen dimension
    cameraLookAheadLimit = (int)(((int64_t)((int)(655360L * (int64_t)(minWH << 16) >> 16)) << 32) / 8388608L >> 16);
}

int GamePhysics::getCamPosX()
{
    if (isEnableLookAhead) {
        camShiftX = (int)(((int64_t)renderCache[0]->velocityXF16 << 32) / 1572864L >> 16) + (int)((int64_t)camShiftX * 57344L >> 16);
    } else {
        camShiftX = 0;
    }

    camShiftX = camShiftX < cameraLookAheadLimit ? camShiftX : cameraLookAheadLimit;
    camShiftX = camShiftX < -cameraLookAheadLimit ? -cameraLookAheadLimit : camShiftX;
    return (renderCache[0]->xF16 + camShiftX) << 2 >> 16;
}

int GamePhysics::getCamPosY()
{
    if (isEnableLookAhead) {
        camShiftY = (int)(((int64_t)renderCache[0]->velocityYF16 << 32) / 1572864L >> 16) + (int)((int64_t)camShiftY * 57344L >> 16);
    } else {
        camShiftY = 0;
    }

    camShiftY = camShiftY < cameraLookAheadLimit ? camShiftY : cameraLookAheadLimit;
    camShiftY = camShiftY < -cameraLookAheadLimit ? -cameraLookAheadLimit : camShiftY;
    return (renderCache[0]->yF16 + camShiftY) << 2 >> 16;
}

int GamePhysics::getRawXDistance()
{
    // Return max X position of wheels, or chassis X if bike is destroyed
    int maxWheelXF16 = renderCache[1]->xF16 < renderCache[2]->xF16 ? renderCache[2]->xF16 : renderCache[1]->xF16;
    return isBikeDestroyed ? levelLoader->getTrackProgressRatio(renderCache[0]->xF16) : levelLoader->getTrackProgressRatio(maxWheelXF16);
}

void GamePhysics::captureRenderSnapshot()
{
    // Synchronize buffer 5 (render buffer) with current physics buffer
    for (int i = 0; i < 6; ++i) {
        motoComponents[i]->stateBuffers[5]->xF16 = motoComponents[i]->stateBuffers[readBufferIndex]->xF16;
        motoComponents[i]->stateBuffers[5]->yF16 = motoComponents[i]->stateBuffers[readBufferIndex]->yF16;
        motoComponents[i]->stateBuffers[5]->angleF16 = motoComponents[i]->stateBuffers[readBufferIndex]->angleF16;
    }

    // Copy chassis velocity and back wheel angular velocity to render buffer
    motoComponents[0]->stateBuffers[5]->velocityXF16 = motoComponents[0]->stateBuffers[readBufferIndex]->velocityXF16;
    motoComponents[0]->stateBuffers[5]->velocityYF16 = motoComponents[0]->stateBuffers[readBufferIndex]->velocityYF16;
    motoComponents[2]->stateBuffers[5]->angularVelocityF16 = motoComponents[2]->stateBuffers[readBufferIndex]->angularVelocityF16;
}

void GamePhysics::prepareRenderCache()
{
    // Copy render buffer (5) to stateBuffers for rendering
    for (int i = 0; i < 6; ++i) {
        renderCache[i]->xF16 = motoComponents[i]->stateBuffers[5]->xF16;
        renderCache[i]->yF16 = motoComponents[i]->stateBuffers[5]->yF16;
        renderCache[i]->angleF16 = motoComponents[i]->stateBuffers[5]->angleF16;
    }

    // Copy chassis velocity and back wheel angular velocity
    renderCache[0]->velocityXF16 = motoComponents[0]->stateBuffers[5]->velocityXF16;
    renderCache[0]->velocityYF16 = motoComponents[0]->stateBuffers[5]->velocityYF16;
    renderCache[2]->angularVelocityF16 = motoComponents[2]->stateBuffers[5]->angularVelocityF16;
}

void GamePhysics::renderEngine(GameCanvas* gameCanvas, int upXF16, int upYF16)
{
    int engineAngle4F16 = MathF16::atan2F16(renderCache[0]->xF16 - renderCache[3]->xF16, renderCache[0]->yF16 - renderCache[3]->yF16);
    int fenderAngle4F16 = MathF16::atan2F16(renderCache[0]->xF16 - renderCache[4]->xF16, renderCache[0]->yF16 - renderCache[4]->yF16);
    int engineXF16 = (renderCache[0]->xF16 >> 1) + (renderCache[3]->xF16 >> 1);
    int engineYF16 = (renderCache[0]->yF16 >> 1) + (renderCache[3]->yF16 >> 1);
    int fenderXF16 = (renderCache[0]->xF16 >> 1) + (renderCache[4]->xF16 >> 1);
    int fenderYF16 = (renderCache[0]->yF16 >> 1) + (renderCache[4]->yF16 >> 1);
    int negYF16 = -upYF16;
    engineXF16 += (int)((int64_t)negYF16 * 65536L >> 16) - (int)((int64_t)upXF16 * 32768L >> 16);
    engineYF16 += (int)((int64_t)upXF16 * 65536L >> 16) - (int)((int64_t)upYF16 * 32768L >> 16);
    fenderXF16 += (int)((int64_t)negYF16 * 65536L >> 16) - (int)((int64_t)upXF16 * 117964L >> 16);
    fenderYF16 += (int)((int64_t)upXF16 * 65536L >> 16) - (int)((int64_t)upYF16 * 131072L >> 16);
    gameCanvas->renderFender(fenderXF16 << 2 >> 16, fenderYF16 << 2 >> 16, fenderAngle4F16);
    gameCanvas->renderEngine(engineXF16 << 2 >> 16, engineYF16 << 2 >> 16, engineAngle4F16);
}

void GamePhysics::renderMotoFork(GameCanvas* canvas)
{
    canvas->setColor(128, 128, 128);
    canvas->drawLineF16(renderCache[3]->xF16, renderCache[3]->yF16, renderCache[1]->xF16, renderCache[1]->yF16);
}

void GamePhysics::renderWheelTires(GameCanvas* canvas)
{
    int8_t backWheelIsThin = 1;
    int8_t forwardWheelIsThin = 1;
    switch (currentLeague) {
    case 1:
        backWheelIsThin = 0;
        break;
    case 2:
    case 3:
        forwardWheelIsThin = 0;
        backWheelIsThin = 0;
    }

    // back wheel
    canvas->drawWheelTires(renderCache[2]->xF16 << 2 >> 16, renderCache[2]->yF16 << 2 >> 16, backWheelIsThin);
    // forward wheel
    canvas->drawWheelTires(renderCache[1]->xF16 << 2 >> 16, renderCache[1]->yF16 << 2 >> 16, forwardWheelIsThin);
}

void GamePhysics::renderWheelSpokes(GameCanvas* gameCanvas)
{
    int wheelRadiusF16;
    int xxxF16 = (int)((int64_t)(wheelRadiusF16 = motoComponents[1]->radiusF16) * 58982L >> 16);
    int yyyF16 = (int)((int64_t)wheelRadiusF16 * 45875L >> 16);
    gameCanvas->setColor(0, 0, 0);
    if (Micro::isInGameMenu) {
        gameCanvas->drawCircle(renderCache[1]->xF16 << 2 >> 16, renderCache[1]->yF16 << 2 >> 16, (wheelRadiusF16 + wheelRadiusF16) << 2 >> 16);
        gameCanvas->drawCircle(renderCache[1]->xF16 << 2 >> 16, renderCache[1]->yF16 << 2 >> 16, (xxxF16 + xxxF16) << 2 >> 16);
        gameCanvas->drawCircle(renderCache[2]->xF16 << 2 >> 16, renderCache[2]->yF16 << 2 >> 16, (wheelRadiusF16 + wheelRadiusF16) << 2 >> 16);
        gameCanvas->drawCircle(renderCache[2]->xF16 << 2 >> 16, renderCache[2]->yF16 << 2 >> 16, (yyyF16 + yyyF16) << 2 >> 16);
    }

    int8_t radialOffsetYF16 = 0; // Radial Y offset for spoke calculation (0 = from center)
    int angle;
    int cosF16 = MathF16::cosF16(angle = renderCache[1]->angleF16);
    int sinF16 = MathF16::sinF16(angle);
    int dxF16 = (int)((int64_t)cosF16 * (int64_t)xxxF16 >> 16) + (int)((int64_t)(-sinF16) * (int64_t)radialOffsetYF16 >> 16);
    int dyF16 = (int)((int64_t)sinF16 * (int64_t)xxxF16 >> 16) + (int)((int64_t)cosF16 * (int64_t)radialOffsetYF16 >> 16);
    angle = 82354;
    cosF16 = MathF16::cosF16(82354);
    sinF16 = MathF16::sinF16(angle);

    int prevDxF16; // Previous DX for rotation matrix calculation
    int i;
    for (i = 0; i < 5; ++i) {
        // forward wheel spokes
        gameCanvas->drawLineF16(renderCache[1]->xF16, renderCache[1]->yF16, renderCache[1]->xF16 + dxF16, renderCache[1]->yF16 + dyF16);
        prevDxF16 = dxF16;
        dxF16 = (int)((int64_t)cosF16 * (int64_t)dxF16 >> 16) + (int)((int64_t)(-sinF16) * (int64_t)dyF16 >> 16);
        dyF16 = (int)((int64_t)sinF16 * (int64_t)prevDxF16 >> 16) + (int)((int64_t)cosF16 * (int64_t)dyF16 >> 16);
    }

    radialOffsetYF16 = 0;
    cosF16 = MathF16::cosF16(angle = renderCache[2]->angleF16);
    sinF16 = MathF16::sinF16(angle);
    dxF16 = (int)((int64_t)cosF16 * (int64_t)xxxF16 >> 16) + (int)((int64_t)(-sinF16) * (int64_t)radialOffsetYF16 >> 16);
    dyF16 = (int)((int64_t)sinF16 * (int64_t)xxxF16 >> 16) + (int)((int64_t)cosF16 * (int64_t)radialOffsetYF16 >> 16);
    angle = 82354;
    cosF16 = MathF16::cosF16(82354);
    sinF16 = MathF16::sinF16(angle);

    for (i = 0; i < 5; ++i) {
        // back wheel spokes
        gameCanvas->drawLineF16(renderCache[2]->xF16, renderCache[2]->yF16, renderCache[2]->xF16 + dxF16, renderCache[2]->yF16 + dyF16);
        prevDxF16 = dxF16;
        dxF16 = (int)((int64_t)cosF16 * (int64_t)dxF16 >> 16) + (int)((int64_t)(-sinF16) * (int64_t)dyF16 >> 16);
        dyF16 = (int)((int64_t)sinF16 * (int64_t)prevDxF16 >> 16) + (int)((int64_t)cosF16 * (int64_t)dyF16 >> 16);
    }

    if (currentLeague > 0) {
        gameCanvas->setColor(255, 0, 0);
        if (currentLeague > 2) {
            gameCanvas->setColor(100, 100, 255);
        }

        gameCanvas->drawCircle(renderCache[2]->xF16 << 2 >> 16, renderCache[2]->yF16 << 2 >> 16, 4);
        gameCanvas->drawCircle(renderCache[1]->xF16 << 2 >> 16, renderCache[1]->yF16 << 2 >> 16, 4);
    }
}

void GamePhysics::renderRider(GameCanvas* gameCanvas, int upXF16, int upYF16, int fwdXF16, int fwdYF16)
{
    int posePhase = 0;
    int lerpFactorF16 = 65536; // 1.0 in 16.16 fixed-point

    // Rider's root anchor point is tied to the main chassis component
    int chassisXF16 = renderCache[0]->xF16;
    int chassisYF16 = renderCache[0]->yF16;

    // Skeletal joint coordinates
    int handlebarXF16 = 0, handlebarYF16 = 0;
    int ankleXF16 = 0, ankleYF16 = 0;
    int footPegXF16 = 0, footPegYF16 = 0;
    int kneeXF16 = 0, kneeYF16 = 0;
    int hipXF16 = 0, hipYF16 = 0;
    int shoulderXF16 = 0, shoulderYF16 = 0;
    int headXF16 = 0, headYF16 = 0;
    int elbowXF16 = 0, elbowYF16 = 0;

    std::vector<std::vector<int>> exactPose, startPose, endPose;

    // Determine which keyframes to interpolate based on lean angle
    if (isRenderBodySprites) {
        // Leaning Back
        if (leanF16 < 32768) {
            startPose = riderPoseLeanBackSprites;
            endPose = riderPoseCenterSprites;
            // 131072L >> 16 is effectively multiplying by 2.0.
            // Normalizes the 0 - 32768 range into a 0 - 65536 (0.0 to 1.0) lerp factor.
            lerpFactorF16 = (int)((int64_t)leanF16 * 131072L >> 16);
        }
        // Leaning Forward
        else if (leanF16 > 32768) {
            posePhase = 1;
            startPose = riderPoseCenterSprites;
            endPose = riderPoseLeanForwardSprites;
            lerpFactorF16 = (int)((int64_t)(leanF16 - 32768) * 131072L >> 16);
        }
        // Perfectly Centered
        else {
            exactPose = riderPoseCenterSprites;
        }
    }

    // Line drawing mode
    else {
        if (leanF16 < 32768) {
            startPose = riderPoseLeanBackLine;
            endPose = riderPoseCenterLine;
            lerpFactorF16 = (int)((int64_t)leanF16 * 131072L >> 16);
        } else if (leanF16 > 32768) {
            posePhase = 1;
            startPose = riderPoseCenterLine;
            endPose = riderPoseLeanForwardLine;
            lerpFactorF16 = (int)((int64_t)(leanF16 - 32768) * 131072L >> 16);
        } else {
            exactPose = riderPoseCenterLine;
        }
    }

    // Interpolate keyframes and apply 2D chassis transformation
    for (std::size_t jointIdx = 0; jointIdx < riderPoseCenterLine.size(); ++jointIdx) {
        int localXF16;
        int localYF16;

        if (!startPose.empty()) {
            // Linear interpolation: startPose * (1.0 - lerp) + endPose * (lerp)
            localXF16 = (int)((int64_t)startPose[jointIdx][0] * (int64_t)(65536 - lerpFactorF16) >> 16) + (int)((int64_t)endPose[jointIdx][0] * (int64_t)lerpFactorF16 >> 16);
            localYF16 = (int)((int64_t)startPose[jointIdx][1] * (int64_t)(65536 - lerpFactorF16) >> 16) + (int)((int64_t)endPose[jointIdx][1] * (int64_t)lerpFactorF16 >> 16);
        } else {
            localXF16 = exactPose[jointIdx][0];
            localYF16 = exactPose[jointIdx][1];
        }

        // Apply 2D rotation matrix from bike chassis
        int worldXF16 = chassisXF16 + (int)((int64_t)fwdXF16 * (int64_t)localXF16 >> 16) + (int)((int64_t)upXF16 * (int64_t)localYF16 >> 16);
        int worldYF16 = chassisYF16 + (int)((int64_t)fwdYF16 * (int64_t)localXF16 >> 16) + (int)((int64_t)upYF16 * (int64_t)localYF16 >> 16);

        // Assign to specific joints
        switch (jointIdx) {
        case 0:
            kneeXF16 = worldXF16;
            kneeYF16 = worldYF16;
            break;
        case 1:
            hipXF16 = worldXF16;
            hipYF16 = worldYF16;
            break;
        case 2:
            shoulderXF16 = worldXF16;
            shoulderYF16 = worldYF16;
            break;
        case 3:
            headXF16 = worldXF16;
            headYF16 = worldYF16;
            break;
        case 4:
            elbowXF16 = worldXF16;
            elbowYF16 = worldYF16;
            break;
        case 5:
            ankleXF16 = worldXF16;
            ankleYF16 = worldYF16;
            break;
        case 6:
            footPegXF16 = worldXF16;
            footPegYF16 = worldYF16;
            break;
        case 7:
            handlebarXF16 = worldXF16;
            handlebarYF16 = worldYF16;
            break;
        }
    }

    // Interpolate the anchor point so the torso sprite "slides" along the spine as the rider leans.
    int torsoAnchorF16 = (int)((int64_t)torsoAnchorOffsets[posePhase][0] * (int64_t)(65536 - lerpFactorF16) >> 16) + (int)((int64_t)torsoAnchorOffsets[posePhase + 1][0] * (int64_t)lerpFactorF16 >> 16);

    // Render the rider
    if (isRenderBodySprites) {
        gameCanvas->renderBodyPart(ankleXF16 << 2, ankleYF16 << 2, kneeXF16 << 2, kneeYF16 << 2, 1);
        gameCanvas->renderBodyPart(kneeXF16 << 2, kneeYF16 << 2, hipXF16 << 2, hipYF16 << 2, 1);
        gameCanvas->renderBodyPart(hipXF16 << 2, hipYF16 << 2, shoulderXF16 << 2, shoulderYF16 << 2, 2, torsoAnchorF16);
        gameCanvas->renderBodyPart(shoulderXF16 << 2, shoulderYF16 << 2, elbowXF16 << 2, elbowYF16 << 2, 0);

        // Calculate helmet angle based on the chassis rotation matrix components
        int helmetAngleF16 = MathF16::atan2F16(upXF16, upYF16);
        if (leanF16 > 32768) {
            helmetAngleF16 += 20588; // Offset helmet if leaning forward
        }

        gameCanvas->drawHelmet(headXF16 << 2 >> 16, headYF16 << 2 >> 16, helmetAngleF16);
    }

    // Line drawing mode
    else {
        gameCanvas->setColor(0, 0, 0);
        gameCanvas->drawLineF16(ankleXF16, ankleYF16, kneeXF16, kneeYF16);
        gameCanvas->drawLineF16(kneeXF16, kneeYF16, hipXF16, hipYF16);

        gameCanvas->setColor(0, 0, 128); // Blue torso
        gameCanvas->drawLineF16(hipXF16, hipYF16, shoulderXF16, shoulderYF16);
        gameCanvas->drawLineF16(shoulderXF16, shoulderYF16, elbowXF16, elbowYF16);
        gameCanvas->drawLineF16(elbowXF16, elbowYF16, handlebarXF16, handlebarYF16);

        int radiusBaseF16 = 65536; // 1.0 radius factor
        gameCanvas->setColor(156, 0, 0); // Red helmet fallback
        gameCanvas->drawCircle(headXF16 << 2 >> 16, headYF16 << 2 >> 16, (radiusBaseF16 + radiusBaseF16) << 2 >> 16);
    }

    // Attachment points (handlebar/foot peg)
    gameCanvas->setColor(0, 0, 0);
    gameCanvas->drawAttachmentPointSprite(handlebarXF16 << 2 >> 16, handlebarYF16 << 2 >> 16);
    gameCanvas->drawAttachmentPointSprite(footPegXF16 << 2 >> 16, footPegYF16 << 2 >> 16);
}

void GamePhysics::renderMotoAsLines(GameCanvas* gameCanvas, int upXF16, int upYF16, int fwdXF16, int fwdYF16)
{
    // Handlebars / Fork Top (Component 2)
    int hbarX = renderCache[2]->xF16;
    int hbarY = renderCache[2]->yF16;
    int hbarLeftX = hbarX + (int)((int64_t)fwdXF16 * 32768 >> 16);
    int hbarLeftY = hbarY + (int)((int64_t)fwdYF16 * 32768 >> 16);
    int hbarRightX = hbarX - (int)((int64_t)fwdXF16 * 32768 >> 16);
    int hbarRightY = hbarY - (int)((int64_t)fwdYF16 * 32768 >> 16);

    // Main Chassis Frame (Component 0)
    int frameRootX = renderCache[0]->xF16;
    int frameRootY = renderCache[0]->yF16;
    int seatPostTopX = frameRootX + (int)((int64_t)upXF16 * 32768 >> 16);
    int seatPostTopY = frameRootY + (int)((int64_t)upYF16 * 32768 >> 16);

    // Bottom of the frame (swung down 2 units from the seat post top)
    int frameBottomX = seatPostTopX - (int)((int64_t)upXF16 * 131072 >> 16);
    int frameBottomY = seatPostTopY - (int)((int64_t)upYF16 * 131072 >> 16);

    // Engine Guard / Lower Frame
    int engGuardX = frameBottomX + (int)((int64_t)fwdXF16 * 65536 >> 16);
    int engGuardY = frameBottomY + (int)((int64_t)fwdYF16 * 65536 >> 16);

    // Fuel Tank / Top Tube area
    int tankX = frameBottomX + (int)((int64_t)upXF16 * 49152 >> 16) + (int)((int64_t)fwdXF16 * 49152 >> 16);
    int tankY = frameBottomY + (int)((int64_t)upYF16 * 49152 >> 16) + (int)((int64_t)fwdYF16 * 49152 >> 16);

    int seatX = frameBottomX + (int)((int64_t)fwdXF16 * 32768 >> 16);
    int seatY = frameBottomY + (int)((int64_t)fwdYF16 * 32768 >> 16);

    // Wheel Mount Points
    int frontWheelX = renderCache[1]->xF16;
    int frontWheelY = renderCache[1]->yF16;
    int rearWheelX = renderCache[3]->xF16;
    int rearWheelY = renderCache[3]->yF16;

    // Rear Swingarm / Exhaust assembly (Component 4)
    int exhaustStartX = renderCache[4]->xF16 - (int)((int64_t)upXF16 * 49152 >> 16);
    int exhaustStartY = renderCache[4]->yF16 - (int)((int64_t)upYF16 * 49152 >> 16);
    int swingarmPivotX = exhaustStartX - (int)((int64_t)fwdXF16 * 32768 >> 16);
    int swingarmPivotY = exhaustStartY - (int)((int64_t)fwdYF16 * 32768 >> 16);
    int exhaustEndX = exhaustStartX - (int)((int64_t)upXF16 * 131072 >> 16) + (int)((int64_t)fwdXF16 * 16384 >> 16);
    int exhaustEndY = exhaustStartY - (int)((int64_t)upYF16 * 131072 >> 16) + (int)((int64_t)fwdYF16 * 16384 >> 16);

    // Rear Axle supports
    int rearSupportX = rearWheelX + (int)((int64_t)fwdXF16 * 32768 >> 16);
    int rearSupportY = rearWheelY + (int)((int64_t)fwdYF16 * 32768 >> 16);
    int sissyBarTopX = rearWheelX + (int)((int64_t)fwdXF16 * 114688 >> 16) - (int)((int64_t)upXF16 * 32768 >> 16);
    int sissyBarTopY = rearWheelY + (int)((int64_t)fwdYF16 * 114688 >> 16) - (int)((int64_t)upYF16 * 32768 >> 16);

    gameCanvas->setColor(50, 50, 50);

    // Draw the seat/frame circle
    gameCanvas->drawCircle(seatX << 2 >> 16, seatY << 2 >> 16, 65536 << 2 >> 16);

    if (!isBikeDestroyed) {
        gameCanvas->drawLineF16(hbarLeftX, hbarLeftY, engGuardX, engGuardY);
        gameCanvas->drawLineF16(hbarRightX, hbarRightY, frameBottomX, frameBottomY);
    }

    // Connect the frame nodes
    gameCanvas->drawLineF16(seatPostTopX, seatPostTopY, frameBottomX, frameBottomY);
    gameCanvas->drawLineF16(seatPostTopX, seatPostTopY, rearWheelX, rearWheelY);
    gameCanvas->drawLineF16(tankX, tankY, rearSupportX, rearSupportY);
    gameCanvas->drawLineF16(rearSupportX, rearSupportY, sissyBarTopX, sissyBarTopY);

    if (!isBikeDestroyed) {
        gameCanvas->drawLineF16(rearWheelX, rearWheelY, frontWheelX, frontWheelY);
        gameCanvas->drawLineF16(sissyBarTopX, sissyBarTopY, frontWheelX, frontWheelY);
    }

    // Connect swingarm/exhaust geometry
    gameCanvas->drawLineF16(engGuardX, engGuardY, swingarmPivotX, swingarmPivotY);
    gameCanvas->drawLineF16(tankX, tankY, exhaustStartX, exhaustStartY);
    gameCanvas->drawLineF16(exhaustStartX, exhaustStartY, exhaustEndX, exhaustEndY);
    gameCanvas->drawLineF16(swingarmPivotX, swingarmPivotY, exhaustEndX, exhaustEndY);
}

void GamePhysics::renderGame(GameCanvas* gameCanvas)
{
    gameCanvas->clearScreenWithWhite();

    // Calculate the vector between front and rear wheels to determine bike tilt
    // Component 3: Rear Wheel, Component 4: Front Wheel
    int upXF16 = renderCache[3]->xF16 - renderCache[4]->xF16;
    int upYF16 = renderCache[3]->yF16 - renderCache[4]->yF16;

    int length = fastVectorLengthF16(upXF16, upYF16);
    if (length != 0) {
        // Normalize the Up Vector
        upXF16 = (int)(((int64_t)upXF16 << 32) / (int64_t)length >> 16);
        upYF16 = (int)(((int64_t)upYF16 << 32) / (int64_t)length >> 16);
    }

    // Derive the perpendicular Forward Vector (rotate Up Vector by 90 degrees)
    int fwdXF16 = -upYF16;
    int fwdYF16 = upXF16;

    if (isBikeDestroyed) {
        // Find the bounding X-range of the crash for camera/level logic
        int frontX = renderCache[4]->xF16;
        int rearX = renderCache[3]->xF16;
        if (rearX >= frontX) {
            levelLoader->gameLevel->setShadowBoundariesHalf(frontX, rearX);
        } else {
            levelLoader->gameLevel->setShadowBoundariesHalf(rearX, frontX);
        }
    }

    if (LevelLoader::isEnabledPerspective) {
        levelLoader->renderTrack3D(gameCanvas, renderCache[0]->xF16, renderCache[0]->yF16);
    }

    // Render mechanical parts
    if (isRenderMotoWithSprites) {
        renderEngine(gameCanvas, upXF16, upYF16);
    }

    if (!Micro::isInGameMenu) {
        renderWheelTires(gameCanvas);
    }

    renderWheelSpokes(gameCanvas);

    gameCanvas->setColor(isRenderMotoWithSprites ? 170 : 50, 0, 0);

    // Draw the front wheel hub/details
    int frontWheelAngle = MathF16::atan2F16(upXF16, upYF16);
    gameCanvas->drawWheelHub(
        renderCache[1]->xF16 << 2 >> 16,
        renderCache[1]->yF16 << 2 >> 16,
        wheelRadiusValuesF16[0] << 2 >> 16,
        frontWheelAngle);

    if (!isBikeDestroyed) {
        renderMotoFork(gameCanvas);
    }

    // Pass the basis vectors (Up and Forward) to the renderers
    renderRider(gameCanvas, upXF16, upYF16, fwdXF16, fwdYF16);

    if (!isRenderMotoWithSprites) {
        renderMotoAsLines(gameCanvas, upXF16, upYF16, fwdXF16, fwdYF16);
    }

    levelLoader->renderTrackCenterline(gameCanvas);
}
