#include "GameCanvas.h"
#include "MathF16.h"
#include "GamePhysics.h"
#include "MenuManager.h"
#include "lcdui/Image.h"
#include "lcdui/FontStorage.h"

#include <memory>
#include <vector>

GameCanvas::GameCanvas(Micro* micro)
{
    splashImage = std::make_unique<Image>("splash.png");
    logoImage = std::make_unique<Image>("logo.png");

    // repaint();
    this->micro = micro;
    updateSizeAndRepaint();
    font = FontStorage::getFont(Font::STYLE_BOLD, Font::SIZE_MEDIUM);
    auto defaultFont = FontStorage::getFont(Font::STYLE_PLAIN, Font::SIZE_MEDIUM);

    helmetImage = std::make_unique<Image>("helmet.png");

    helmetSpriteWidth = helmetImage->getWidth() / 6;
    helmetSpriteHeight = helmetImage->getHeight() / 6;

    spritesImage = std::make_unique<Image>("sprites.png");

    dx = 0;
    dy = height2;

    commandMenu = new Command("Menu", 1, 1);
    defaultFontWidth00 = defaultFont->stringWidth("00") + 3;
}

void GameCanvas::drawSprite(Graphics* g, int spriteNo, int x, int y)
{
    if (spritesImage) {
        g->setClip(x, y, spriteSizeX[spriteNo], spriteSizeY[spriteNo]);
        g->drawImage(spritesImage.get(), x - spriteOffsetX[spriteNo], y - spriteOffsetY[spriteNo], 20);
        g->setClip(0, 0, getWidth(), getHeight());
    }
}

void GameCanvas::requestRepaint(int state)
{
    loadingScreenState = state;
    if (state == 0) {
        splashImage = nullptr;
        logoImage = nullptr;
    } else {
        repaint();
        serviceRepaints();
    }
}

void GameCanvas::setInputConfigEnabled(bool enabled)
{
    unknown_bool = enabled; // Set but never read - possibly dead code
    updateSizeAndRepaint();
}

void GameCanvas::updateSizeAndRepaint()
{
    width = getWidth();
    height = height2 = getHeight();

    repaint();
}

int GameCanvas::loadSprites(int flags)
{
    if (flags & 1) {
        if (!fenderImage) {
            fenderImage = std::make_unique<Image>("fender.png");
            fenderSpriteWidth = fenderImage->getWidth() / 6;
            fenderSpriteHeight = fenderImage->getHeight() / 6;
        }
        if (!engineImage) {
            engineImage = std::make_unique<Image>("engine.png");
            engineSpriteWidth = engineImage->getWidth() / 6;
            engineSpriteHeight = engineImage->getHeight() / 6;
        }
    } else {
        fenderImage = nullptr;
        engineImage = nullptr;
    }

    if (flags & 2) {
        if (!bodyPartsImages[1]) {
            bodyPartsImages[1] = std::make_unique<Image>("blueleg.png");
        }

        bodyPartsSpriteWidth[1] = bodyPartsImages[1]->getWidth() / 6;
        bodyPartsSpriteHeight[1] = bodyPartsImages[1]->getHeight() / 3;

        bodyPartsImages[0] = std::make_unique<Image>("bluearm.png");

        bodyPartsSpriteWidth[0] = bodyPartsImages[0]->getWidth() / 6;
        bodyPartsSpriteHeight[0] = bodyPartsImages[0]->getHeight() / 3;

        bodyPartsImages[2] = std::make_unique<Image>("bluebody.png");

        bodyPartsSpriteWidth[2] = bodyPartsImages[2]->getWidth() / 6;
        bodyPartsSpriteHeight[2] = bodyPartsImages[2]->getHeight() / 3;
    } else {
        bodyPartsImages[0] = nullptr;
        bodyPartsImages[1] = nullptr;
        bodyPartsImages[2] = nullptr;
    }

    return flags;
}

void GameCanvas::reset()
{
    resetActiveKeys();
}

void GameCanvas::setViewPosition(int dx, int dy)
{
    this->dx = dx;
    this->dy = dy;
    gamePhysics->setRenderMinMaxX(-dx, -dx + width);
}

int GameCanvas::getDx()
{
    return dx;
}

int GameCanvas::addDx(int x)
{
    return x + dx;
}

int GameCanvas::addDy(int y)
{
    return -y + dy;
}

void GameCanvas::drawLine(int x, int y, int x2, int y2)
{
    graphics->drawLine(addDx(x), addDy(y), addDx(x2), addDy(y2));
}

void GameCanvas::drawLineF16(int x, int y, int x2, int y2)
{
    graphics->drawLine(addDx(x << 2 >> 16), addDy(y << 2 >> 16), addDx(x2 << 2 >> 16), addDy(y2 << 2 >> 16));
}

void GameCanvas::renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo)
{
    renderBodyPart(x1F16, y1F16, x2F16, y2F16, bodyPartNo, 32768);
}

void GameCanvas::renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo, int tF16)
{
    // t is the parameter of the linear interpolation
    // t == 0(0.0f)     => (x, y) == (x1, y1)
    // t == 65536(1.0f) => (x, y) == (x2, y2)
    // t == 32768(0.5f) => (x, y) == ((x1 + x2) / 2, (y1 + y2) / 2)
    int x = addDx(((int)((int64_t)x2F16 * (int64_t)tF16 >> 16) + (int)((int64_t)x1F16 * (int64_t)(65536 - tF16) >> 16)) >> 16);
    int y = addDy(((int)((int64_t)y2F16 * (int64_t)tF16 >> 16) + (int)((int64_t)y1F16 * (int64_t)(65536 - tF16) >> 16)) >> 16);

    int angleFP16 = MathF16::atan2F16(x2F16 - x1F16, y2F16 - y1F16);
    int spriteNo = calcSpriteNo(angleFP16, 0, 205887, 16, false);

    if (bodyPartsImages[bodyPartNo]) {
        x -= bodyPartsSpriteWidth[bodyPartNo] / 2;
        y -= bodyPartsSpriteHeight[bodyPartNo] / 2;
        graphics->setClip(x, y, bodyPartsSpriteWidth[bodyPartNo], bodyPartsSpriteHeight[bodyPartNo]);
        graphics->drawImage(bodyPartsImages[bodyPartNo].get(), x - bodyPartsSpriteWidth[bodyPartNo] * (spriteNo % 6), y - bodyPartsSpriteHeight[bodyPartNo] * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }
}

void GameCanvas::drawWheelHub(int centerX, int centerY, int radius, int angleF16)
{
    // Draw arc representing wheel hub/spokes at specified position and angle
    ++radius; // Slight radius adjustment
    int screenX = addDx(centerX - radius);
    int screenY = addDy(centerY + radius);
    int diameter = radius << 1;
    // Convert angle from fixed-point to degrees and draw 90-degree arc
    if ((angleF16 = -((int)(((int64_t)((int)((int64_t)angleF16 * 11796480L >> 16)) << 32) / 205887L >> 16))) < 0) {
        angleF16 += 360;
    }

    graphics->drawArc(screenX, screenY, diameter, diameter, (angleF16 >> 16) + 170, 90);
}

void GameCanvas::drawCircle(int x, int y, int size)
{
    int radius = size / 2;
    int localX = addDx(x - radius);
    int localY = addDy(y + radius);
    graphics->drawArc(localX, localY, size, size, 0, 360);
}

void GameCanvas::fillRect(int x, int y, int w, int h)
{
    int screenX = addDx(x);
    int screenY = addDy(y);
    graphics->fillRect(screenX, screenY, w, h);
}

void GameCanvas::drawAttachmentPointSprite(int x, int y)
{
    int halfSizeX = spriteSizeX[4] / 2;
    int halfSizeY = spriteSizeY[4] / 2;
    drawSprite(graphics, 4, addDx(x - halfSizeX), addDy(y + halfSizeY));
}

void GameCanvas::drawHelmet(int x, int y, int angleF16)
{
    int spriteNo = calcSpriteNo(angleF16, -102943, 411774, 32, true);
    if (helmetImage != nullptr) {
        int screenX = addDx(x) - helmetSpriteWidth / 2;
        int screenY = addDy(y) - helmetSpriteHeight / 2;
        graphics->setClip(screenX, screenY, helmetSpriteWidth, helmetSpriteHeight);
        graphics->drawImage(helmetImage.get(), screenX - helmetSpriteWidth * (spriteNo % 6), screenY - helmetSpriteHeight * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }
}

void GameCanvas::drawTime(int64_t time10Ms)
{
    int seconds = (int)(time10Ms / 100L);
    int time10MsPart = (int)(time10Ms % 100L);
    if (timeInSeconds != seconds || stringWithTime.empty()) {
        std::string zeroPadding;
        if (seconds % 60 >= 10) {
            zeroPadding = "";
        } else {
            zeroPadding = "0";
        }

        stringWithTime = std::to_string(seconds / 60) + ":" + zeroPadding + std::to_string(seconds % 60) + ".";
        timeInSeconds = seconds;
    }

    if (time10MsToStringCache[time10MsPart].empty()) {
        std::string zeroPadding;
        if (time10MsPart >= 10) {
            zeroPadding = "";
        } else {
            zeroPadding = "0";
        }

        time10MsToStringCache[time10MsPart] = zeroPadding + std::to_string(time10Ms % 100L);
    }

    if (time10Ms > 3600000L) {
        setColor(0, 0, 0);
        graphics->drawString("0:00.", width - defaultFontWidth00, height2 - 5, 40);
        graphics->drawString("00", width - defaultFontWidth00, height2 - 5, 36);
    } else {
        setColor(0, 0, 0);
        graphics->drawString(stringWithTime, width - defaultFontWidth00, height2 - 5, 40);
        graphics->drawString(time10MsToStringCache[time10MsPart], width - defaultFontWidth00, height2 - 5, 36);
    }
}

void GameCanvas::triggerTimerIfMatching(int timerId)
{
    if (this->timerId == timerId) {
        timerTriggered = true;
    }
}

void GameCanvas::flagAnimation()
{
    // Update flag animation phase (fixed-point, incremented by 655 each frame)
    flagAnimationPhase += 655;
    int amplitude = 32768 + ((MathF16::sinF16(flagAnimationPhase) < 0 ? -MathF16::sinF16(flagAnimationPhase) : MathF16::sinF16(flagAnimationPhase)) >> 1);
    flagAnimationTime += (int)(6553L * (int64_t)amplitude >> 16);
}

void GameCanvas::renderStartFlag(int x, int y)
{
    if (flagAnimationTime > 229376) {
        flagAnimationTime = 0;
    }

    setColor(0, 0, 0);
    drawLine(x, y, x, y + 32);
    drawSprite(graphics, startFlagAnimationTimeToSpriteNo[flagAnimationTime >> 16], addDx(x), addDy(y) - 32);
}

void GameCanvas::renderFinishFlag(int x, int y)
{
    if (flagAnimationTime > 229376) {
        flagAnimationTime = 0;
    }

    setColor(0, 0, 0);
    drawLine(x, y, x, y + 32);
    drawSprite(graphics, finishFlagAnumationTimeToSpriteNo[flagAnimationTime >> 16], addDx(x), addDy(y) - 32);
}

void GameCanvas::drawWheelTires(int x, int y, int wheelIsThin)
{
    int spriteNo;
    if (wheelIsThin == 1) {
        spriteNo = 0;
    } else {
        spriteNo = 1;
    }

    int spriteHalfX = spriteSizeX[spriteNo] / 2;
    int spriteHalfY = spriteSizeY[spriteNo] / 2;
    int centerX = addDx(x - spriteHalfX);
    int centerY = addDy(y + spriteHalfY);
    drawSprite(graphics, spriteNo, centerX, centerY);
}

int GameCanvas::calcSpriteNo(int angleF16, int angleOffset, int angleRange, int spriteCount, bool flipDirection)
{
    // Calculate sprite number from angle for animated sprites
    for (angleF16 += angleOffset; angleF16 < 0; angleF16 += angleRange) {
    }

    while (angleF16 >= angleRange) {
        angleF16 -= angleRange;
    }

    if (flipDirection) {
        angleF16 = angleRange - angleF16;
    }

    int spriteIndex;
    return (spriteIndex = (int)((int64_t)((int)(((int64_t)angleF16 << 32) / (int64_t)angleRange >> 16)) * (int64_t)(spriteCount << 16) >> 16)) >> 16 < spriteCount - 1 ? spriteIndex >> 16 : spriteCount - 1;
}

void GameCanvas::renderEngine(int x, int y, int angleF16)
{
    int spriteNo = calcSpriteNo(angleF16, -247063, 411774, 32, true);
    int centerX = addDx(x) - engineSpriteWidth / 2;
    int centerY = addDy(y) - engineSpriteHeight / 2;
    if (engineImage != nullptr) {
        graphics->setClip(centerX, centerY, engineSpriteWidth, engineSpriteHeight);
        graphics->drawImage(engineImage.get(), centerX - engineSpriteWidth * (spriteNo % 6), centerY - engineSpriteHeight * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }
}

void GameCanvas::renderFender(int x, int y, int angleF16)
{
    int spriteNo = calcSpriteNo(angleF16, -185297, 411774, 32, true);
    if (fenderImage != nullptr) {
        int centerX = addDx(x) - fenderSpriteWidth / 2;
        int centerY = addDy(y) - fenderSpriteHeight / 2;
        graphics->setClip(centerX, centerY, fenderSpriteWidth, fenderSpriteHeight);
        graphics->drawImage(fenderImage.get(), centerX - fenderSpriteWidth * (spriteNo % 6), centerY - fenderSpriteHeight * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }
}

void GameCanvas::clearScreenWithWhite()
{
    graphics->setColor(255, 255, 255);
    graphics->fillRect(0, 0, width, height2);
}

void GameCanvas::setColor(int red, int green, int blue)
{
    if (Micro::isInGameMenu) {
        red += 128;
        green += 128;
        blue += 128;
        if (red > 240) {
            red = 240;
        }

        if (green > 240) {
            green = 240;
        }

        if (blue > 240) {
            blue = 240;
        }
    }

    graphics->setColor(red, green, blue);
}

void GameCanvas::drawGame(Graphics* g)
{
    // synchronized (objectForSyncronization) {
    if (Micro::isGameLoopRunning && !micro->isAboutToExit) {
        graphics = g;

        int progress;
        if (loadingScreenState != 0) {
            if (loadingScreenState == 1) {
                // Logo screen
                graphics->setColor(255, 255, 255);
                graphics->fillRect(0, 0, getWidth(), getHeight());
                if (logoImage != nullptr) {
                    graphics->drawImage(logoImage.get(), getWidth() / 2, getHeight() / 2, 3);
                    drawSprite(graphics, 16, getWidth() - spriteSizeX[16] - 5, getHeight() - spriteSizeY[16] - 7);
                    drawSprite(graphics, 17, getWidth() - spriteSizeX[17] - 4, getHeight() - spriteSizeY[17] - spriteSizeY[16] - 9);
                }
            } else {
                // Splash screen
                graphics->setColor(255, 255, 255);
                graphics->fillRect(0, 0, getWidth(), getHeight());
                if (splashImage != nullptr) {
                    graphics->drawImage(splashImage.get(), getWidth() / 2, getHeight() / 2, 3);
                }
            }

            progress = (int)(((int64_t)(Micro::gameLoadingStateStage << 16) << 32) / 655360L >> 16);
            drawProgressBar(progress, true);
        } else {
            if (height != getHeight()) {
                updateSizeAndRepaint();
            }

            gamePhysics->prepareRenderCache();
            // Apply camera offsets for view adjustment
            setViewPosition(-gamePhysics->getCamPosX() + cameraOffsetX + width / 2, gamePhysics->getCamPosY() + cameraOffsetY + height2 / 2);
            gamePhysics->renderGame(this);
            if (isDrawingTime) {
                drawTime(micro->gameTimeMs / 10L);
            }

            if (!timerMessage.empty()) {
                setColor(0, 0, 0);
                graphics->setFont(font);
                if (height2 <= 128) {
                    graphics->drawString(timerMessage, width / 2, 1, 17);
                } else {
                    graphics->drawString(timerMessage, width / 2, height2 / 4, 33);
                }

                if (timerTriggered) {
                    timerTriggered = false;
                    timerMessage = "";
                }
            }

            // print fps to screen
            // setColor(0, 0, 0);
            // graphics->setFont(font);
            // graphics->drawString("FPS: " + std::to_string(fps), defaultFontWidth00, height2 - 5, 36);

            progress = gamePhysics->getRawXDistance();
            drawProgressBar(progress, false);
        }

        graphics = nullptr;
    }
    // }
}

void GameCanvas::drawProgressBar(int progress, bool mode)
{
    // Draw progress bar (loading or distance)
    // progress: 0-65536 fixed-point value
    int canvasHeight = mode ? height : height2;
    setColor(0, 0, 0);
    graphics->fillRect(1, canvasHeight - 4, width - 2, 3);
    setColor(255, 255, 255);
    graphics->fillRect(2, canvasHeight - 3, (int)((int64_t)((width - 4) << 16) * (int64_t)progress >> 16) >> 16, 1);
}

void GameCanvas::setInputConfigIndex(int configIndex)
{
    // Set input configuration index (0, 1, or 2) - selects key-to-direction mapping
    inputConfigIndex = configIndex;
}

void GameCanvas::paint(Graphics* graphics)
{
    // static int64_t time = 0;
    // int64_t now = Time::currentTimeMillis();
    // int64_t delta = now - time;
    // time = now;
    // if (delta != 0) {
    //     fps = 1000 / delta;
    //     // std::cout << "FPS: " << fps <<  std::endl;
    //     //setWindowTitle(std::string("Gravity Defied. FPS: ") + std::to_string(fps));
    // }

    processTimers(); // We need to call this function as often as we can. It might be better to move this call somewhere.
    if (Micro::isInGameMenu && menuManager != nullptr) {
        menuManager->renderMenuOverGame(graphics);
    } else {
        drawGame(graphics);
    }
}

void GameCanvas::resetActiveKeys()
{
    int i;
    for (i = 0; i < 10; ++i) {
        activeKeys[i] = false;
    }

    for (i = 0; i < 7; ++i) {
        activeActions[i] = false;
    }
}

void GameCanvas::handleUpdatedInput()
{
    int upDown = 0;
    int leftRight = 0;
    int configIndex = inputConfigIndex;

    int i;
    for (i = 0; i < 10; ++i) {
        if (activeKeys[i]) {
            upDown += keyDirectionMap[configIndex][i][0];
            leftRight += keyDirectionMap[configIndex][i][1];
        }
    }

    for (i = 0; i < 7; ++i) {
        if (activeActions[i]) {
            upDown += actionDirectionMap[i][0];
            leftRight += actionDirectionMap[i][1];
        }
    }

    gamePhysics->updateInputs(upDown, leftRight);
}

void GameCanvas::processTimers()
{
    for (auto i = timers.begin(); i != timers.end();) {
        if (i->ready()) {
            triggerTimerIfMatching(i->getId());
            i = timers.erase(i);
        } else {
            i++;
        }
    }
}

void GameCanvas::processKeyPressed(int keyCode)
{
    int action = getGameAction(keyCode);
    int numKey;
    // KEY_NUM0 - KEY_NUM10 is 48-58
    if ((numKey = keyCode - 48) >= 0 && numKey < 10) {
        activeKeys[numKey] = true;
    } else if (action >= 0 && action < 7) {
        activeActions[action] = true;
    }

    handleUpdatedInput();
}

void GameCanvas::processKeyReleased(int keyCode)
{
    int action = getGameAction(keyCode);
    int numKey;
    if ((numKey = keyCode - 48) >= 0 && numKey < 10) {
        activeKeys[numKey] = false;
    } else if (action >= 0 && action < 7) {
        activeActions[action] = false;
    }

    handleUpdatedInput();
}

void GameCanvas::init(GamePhysics* gamePhysics)
{
    this->gamePhysics = gamePhysics;
    gamePhysics->setMinimalScreenWH(width < height2 ? width : height2);
}

void GameCanvas::scheduleGameTimerTask(const std::string& message, int delayMs)
{
    timerTriggered = false;
    ++timerId;
    timerMessage = message;
    timers.push_back(Timer(timerId, delayMs));
}

void GameCanvas::setMenuManager(MenuManager* menuManager)
{
    this->menuManager = menuManager;
}

void GameCanvas::handleMenuCommand(Command* command, Displayable* displayable)
{
    (void)displayable;
    if (command == commandMenu) {
        menuManager->isMenuRenderingBlocked = true; // Signal menu manager to show menu
        micro->gameToMenu(); // Transition from game to menu state
    }
}

void GameCanvas::keyPressed(int keyCode)
{
    if (Micro::isInGameMenu && menuManager != nullptr) {
        menuManager->processKey(keyCode);
    }

    processKeyPressed(keyCode);
}

void GameCanvas::keyReleased(int keyCode)
{
    processKeyReleased(keyCode);
}

void GameCanvas::commandAction(Command* command, Displayable* displayable)
{
    if (Micro::isInGameMenu && menuManager != nullptr) {
        menuManager->handleCommand(command, displayable);
    } else {
        handleMenuCommand(command, displayable);
    }
}

void GameCanvas::removeMenuCommand()
{
    removeCommand(commandMenu);
}

void GameCanvas::addMenuCommand()
{
    addCommand(commandMenu);
}
