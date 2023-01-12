#include "GameCanvas.h"

#include "MathF16.h"
#include "GamePhysics.h"
#include "MenuManager.h"
#include "utils/Time.h"
#include "lcdui/CanvasImpl.h"

GameCanvas::GameCanvas(Micro *micro) {
    try {
        this->splashImage = Image::createImage("assets/splash.png");
        this->logoImage = Image::createImage("assets/logo.png");
    } catch (std::exception &e) {
    }

    // repaint();
    this->micro = micro;
    updateSizeAndRepaint();
    font = Font::getFont(64, 1, 0);

    try {
        this->helmetImage = Image::createImage("assets/helmet.png");
    } catch (std::exception &e) {
        this->helmetImage = Image::createImage(1, 1);
    }

    helmetSpriteWidth = helmetImage->getWidth() / 6;
    helmetSpriteHeight = helmetImage->getHeight() / 6;

    try {
        spritesImage = Image::createImage("assets/sprites.png");
    } catch (std::exception &e) {
        spritesImage = onePixImage;
    }

    dx = 0;
    dy = height2;
    commandMenu = new Command("Menu", 1, 1);
    defaultFontWidth00 = Font::getDefaultFont()->stringWidth("00") + 3;
}

void GameCanvas::drawSprite(Graphics *g, int spriteNo, int x, int y) {
    if (spritesImage) {
        g->setClip(x, y, spriteSizeX[spriteNo], spriteSizeY[spriteNo]);
        g->drawImage(spritesImage, x - spriteOffsetX[spriteNo], y - spriteOffsetY[spriteNo], 20);
        g->setClip(0, 0, getWidth(), getHeight());
    }
}

void GameCanvas::requestRepaint(int var1) {
    field_184 = var1;
    if (var1 == 0) {
        splashImage = nullptr;
        logoImage = nullptr;
    } else {
        repaint();
        serviceRepaints();
    }
}

void GameCanvas::method_124(bool var1) {
    field_205 = var1;
    updateSizeAndRepaint();
}

void GameCanvas::updateSizeAndRepaint() {
    width = getWidth();
    height = height2 = getHeight();

    repaint();
}

int GameCanvas::loadSprites(int var1) {
    // synchronized (objectForSyncronization) {
        if ((var1 & 1) != 0) {
            try {
                if (fenderImage == nullptr) {
                    fenderImage = Image::createImage("assets/fender.png");
                    fenderSpriteWidth = fenderImage->getWidth() / 6;
                    fenderSpriteHeight = fenderImage->getHeight() / 6;
                }

                if (engineImage == nullptr) {
                    engineImage = Image::createImage("assets/engine.png");
                    engineSpriteWidth = engineImage->getWidth() / 6;
                    engineSpriteHeight = engineImage->getHeight() / 6;
                }
            } catch (std::exception &e) {
                fenderImage = engineImage = nullptr;
                var1 &= -2;
            }
        } else {
            engineImage = fenderImage = nullptr;
        }

        if ((var1 & 2) != 0) {
            try {
                if (bodyPartsImages[1] == nullptr) {
                    bodyPartsImages[1] = std::shared_ptr<Image>(Image::createImage("assets/blueleg.png"));
                }
            } catch (std::exception &e) {
                bodyPartsImages[1] = nullptr;
                bodyPartsImages[0] = nullptr;
                bodyPartsImages[2] = nullptr;
                var1 &= -3;
                return var1;
            }

            bodyPartsSpriteWidth[1] = bodyPartsImages[1]->getWidth() / 6;
            bodyPartsSpriteHeight[1] = bodyPartsImages[1]->getHeight() / 3;

            try {
                bodyPartsImages[0] = std::shared_ptr<Image>(Image::createImage("assets/bluearm.png"));
            } catch (std::exception &e) {
                bodyPartsImages[0] = bodyPartsImages[1];
            }

            bodyPartsSpriteWidth[0] = bodyPartsImages[0]->getWidth() / 6;
            bodyPartsSpriteHeight[0] = bodyPartsImages[0]->getHeight() / 3;

            try {
                bodyPartsImages[2] = std::shared_ptr<Image>(Image::createImage("assets/bluebody.png"));
            } catch (std::exception &e) {
                bodyPartsImages[2] = bodyPartsImages[1];
            }

            bodyPartsSpriteWidth[2] = bodyPartsImages[2]->getWidth() / 6;
            bodyPartsSpriteHeight[2] = bodyPartsImages[2]->getHeight() / 3;
        } else {
            bodyPartsImages[1] = bodyPartsImages[2] = bodyPartsImages[0] = nullptr;
        }

        return var1;
    // }
}

void GameCanvas::method_129() {
    method_164();
}

Image* GameCanvas::loadImage(std::string imgName) {
    Image *img = nullptr;

    try {
        img = Image::createImage(imgName.c_str());
    } catch (std::exception &e) {
    }

    return img;
}

void GameCanvas::setViewPosition(int dx, int dy) {
    this->dx = dx;
    this->dy = dy;
    gamePhysics->setRenderMinMaxX(-dx, -dx + width);
}

int GameCanvas::getDx() {
    return dx;
}

int GameCanvas::addDx(int x) {
    return x + dx;
}

int GameCanvas::addDy(int y) {
    return -y + dy;
}

void GameCanvas::drawLine(int x, int y, int x2, int y2) {
    graphics->drawLine(addDx(x), addDy(y), addDx(x2), addDy(y2));
}

void GameCanvas::drawLineF16(int x, int y, int x2, int y2) {
    graphics->drawLine(addDx(x << 2 >> 16), addDy(y << 2 >> 16), addDx(x2 << 2 >> 16), addDy(y2 << 2 >> 16));
}

void GameCanvas::renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo) {
    renderBodyPart(x1F16, y1F16, x2F16, y2F16, bodyPartNo, 32768);
}

void GameCanvas::renderBodyPart(int x1F16, int y1F16, int x2F16, int y2F16, int bodyPartNo, int tF16) {
    // t is the parameter of the linear interpolation
    // t == 0(0.0f)     => (x, y) == (x1, y1)
    // t == 65536(1.0f) => (x, y) == (x2, y2)
    // t == 32768(0.5f) => (x, y) == ((x1 + x2) / 2, (y1 + y2) / 2)
    int x = addDx(((int) ((int64_t) x2F16 * (int64_t) tF16 >> 16) + (int) ((int64_t) x1F16 * (int64_t) (65536 - tF16) >> 16)) >> 16);
    int y = addDy(((int) ((int64_t) y2F16 * (int64_t) tF16 >> 16) + (int) ((int64_t) y1F16 * (int64_t) (65536 - tF16) >> 16)) >> 16);

    int angleFP16 = MathF16::atan2F16(x2F16 - x1F16, y2F16 - y1F16);
    int spriteNo = calcSpriteNo(angleFP16, 0, 205887, 16, false);

    if (bodyPartsImages[bodyPartNo] != nullptr) {
        x -= bodyPartsSpriteWidth[bodyPartNo] / 2;
        y -= bodyPartsSpriteHeight[bodyPartNo] / 2;
        graphics->setClip(x, y, bodyPartsSpriteWidth[bodyPartNo], bodyPartsSpriteHeight[bodyPartNo]);
        graphics->drawImage(bodyPartsImages[bodyPartNo].get(), x - bodyPartsSpriteWidth[bodyPartNo] * (spriteNo % 6), y - bodyPartsSpriteHeight[bodyPartNo] * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }
}

void GameCanvas::method_142(int var1, int var2, int var3, int var4) {
    ++var3;
    int var5 = addDx(var1 - var3);
    int var6 = addDy(var2 + var3);
    int var7 = var3 << 1;
    if ((var4 = -((int) (((int64_t) ((int) ((int64_t) var4 * 11796480L >> 16)) << 32) / 205887L >> 16))) < 0) {
        var4 += 360;
    }

    graphics->drawArc(var5, var6, var7, var7, (var4 >> 16) + 170, 90);
}

void GameCanvas::drawCircle(int x, int y, int size) {
    int radius = size / 2;
    int localX = addDx(x - radius);
    int localY = addDy(y + radius);
    graphics->drawArc(localX, localY, size, size, 0, 360);
}

void GameCanvas::fillRect(int x, int y, int w, int h) {
    int var5 = addDx(x);
    int var6 = addDy(y);
    graphics->fillRect(var5, var6, w, h);
}

void GameCanvas::drawForthSpriteByCenter(int centerX, int centerY) {
    int halfSizeX = spriteSizeX[4] / 2;
    int halfSizeY = spriteSizeY[4] / 2;
    drawSprite(graphics, 4, addDx(centerX - halfSizeX), addDy(centerY + halfSizeY));
}

void GameCanvas::drawHelmet(int x, int y, int angleF16) {
    int var4 = calcSpriteNo(angleF16, -102943, 411774, 32, true);
    if (helmetImage != nullptr) {
        int var5 = addDx(x) - helmetSpriteWidth / 2;
        int var6 = addDy(y) - helmetSpriteHeight / 2;
        graphics->setClip(var5, var6, helmetSpriteWidth, helmetSpriteHeight);
        graphics->drawImage(helmetImage, var5 - helmetSpriteWidth * (var4 % 6), var6 - helmetSpriteHeight * (var4 / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }

}

void GameCanvas::drawTime(int64_t time10Ms) {
    int seconds = (int) (time10Ms / 100L);
    int time10MsPart = (int) (time10Ms % 100L);
    if (timeInSeconds != seconds  || stringWithTime.empty()) {
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

void GameCanvas::method_150(int var1) {
    if (timerId == var1) {
        timerTriggered = true;
    }
}

void GameCanvas::method_151() {
    field_226 += 655;
    int var0 = 32768 + ((MathF16::sinF16(field_226) < 0 ? -MathF16::sinF16(field_226) : MathF16::sinF16(field_226)) >> 1);
    flagAnimationTime += (int) (6553L * (int64_t) var0 >> 16);
}

void GameCanvas::renderStartFlag(int x, int y) {
    if (flagAnimationTime > 229376) {
        flagAnimationTime = 0;
    }

    setColor(0, 0, 0);
    drawLine(x, y, x, y + 32);
    drawSprite(graphics, startFlagAnimationTimeToSpriteNo[flagAnimationTime >> 16], addDx(x), addDy(y) - 32);
}

void GameCanvas::renderFinishFlag(int x, int y) {
    if (flagAnimationTime > 229376) {
        flagAnimationTime = 0;
    }

    setColor(0, 0, 0);
    drawLine(x, y, x, y + 32);
    drawSprite(graphics, finishFlagAnumationTimeToSpriteNo[flagAnimationTime >> 16], addDx(x), addDy(y) - 32);
}

void GameCanvas::drawWheelTires(int x, int y, int wheelIsThin) {
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

int GameCanvas::calcSpriteNo(int angleF16, int var2, int var3, int var4, bool var5) {
    for (angleF16 += var2; angleF16 < 0; angleF16 += var3) {
    }

    while (angleF16 >= var3) {
        angleF16 -= var3;
    }

    if (var5) {
        angleF16 = var3 - angleF16;
    }

    int var6;
    return (var6 = (int) ((int64_t) ((int) (((int64_t) angleF16 << 32) / (int64_t) var3 >> 16)) * (int64_t) (var4 << 16) >> 16)) >> 16 < var4 - 1 ? var6 >> 16 : var4 - 1;
}

void GameCanvas::renderEngine(int x, int y, int angleF16) {
    int spriteNo = calcSpriteNo(angleF16, -247063, 411774, 32, true);
    int centerX = addDx(x) - engineSpriteWidth / 2;
    int centerY = addDy(y) - engineSpriteHeight / 2;
    if (engineImage != nullptr) {
        graphics->setClip(centerX, centerY, engineSpriteWidth, engineSpriteHeight);
        graphics->drawImage(engineImage, centerX - engineSpriteWidth * (spriteNo % 6), centerY - engineSpriteHeight * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }
}

void GameCanvas::renderFender(int x, int y, int angleF16) {
    int spriteNo = calcSpriteNo(angleF16, -185297, 411774, 32, true);
    if (fenderImage != nullptr) {
        int centerX = addDx(x) - fenderSpriteWidth / 2;
        int centerY = addDy(y) - fenderSpriteHeight / 2;
        graphics->setClip(centerX, centerY, fenderSpriteWidth, fenderSpriteHeight);
        graphics->drawImage(fenderImage, centerX - fenderSpriteWidth * (spriteNo % 6), centerY - fenderSpriteHeight * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, getHeight());
    }
}

void GameCanvas::clearScreenWithWhite() {
    graphics->setColor(255, 255, 255);
    graphics->fillRect(0, 0, width, height2);
}

void GameCanvas::setColor(int red, int green, int blue) {
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

void GameCanvas::drawGame(Graphics *g) {
    // synchronized (objectForSyncronization) {
        if (Micro::field_249 && !micro->field_242) {
            graphics = g;

            int var3;
            if (field_184 != 0) {
                if (field_184 == 1) {
                    graphics->setColor(255, 255, 255);
                    graphics->fillRect(0, 0, getWidth(), getHeight());
                    if (logoImage != nullptr) {
                        graphics->drawImage(logoImage, getWidth() / 2, getHeight() / 2, 3);
                        drawSprite(graphics, 16, getWidth() - spriteSizeX[16] - 5, getHeight() - spriteSizeY[16] - 7);
                        drawSprite(graphics, 17, getWidth() - spriteSizeX[17] - 4, getHeight() - spriteSizeY[17] - spriteSizeY[16] - 9);
                    }
                } else {
                    graphics->setColor(255, 255, 255);
                    graphics->fillRect(0, 0, getWidth(), getHeight());
                    if (splashImage != nullptr) {
                        graphics->drawImage(splashImage, getWidth() / 2, getHeight() / 2, 3);
                    }
                }

                var3 = (int) (((int64_t) (Micro::gameLoadingStateStage << 16) << 32) / 655360L >> 16);
                method_161(var3, true);
            } else {
                if (height != getHeight()) {
                    updateSizeAndRepaint();
                }

                gamePhysics->setMotoComponents();
                setViewPosition(-gamePhysics->getCamPosX() + field_178 + width / 2, gamePhysics->getCamPosY() + field_179 + height2 / 2);
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

                var3 = gamePhysics->method_52();
                method_161(var3, false);
            }

            graphics = nullptr;
        }
    // }
}

void GameCanvas::method_161(int var1, bool mode) {
    int h = mode ? height : height2;
    setColor(0, 0, 0);
    graphics->fillRect(1, h - 4, width - 2, 3);
    setColor(255, 255, 255);
    graphics->fillRect(2, h - 3, (int) ((int64_t) ((width - 4) << 16) * (int64_t) var1 >> 16) >> 16, 1);
}

void GameCanvas::method_163(int var1) {
    field_232 = var1;
}

void GameCanvas::paint(Graphics *graphics) {
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
        menuManager->method_202(graphics);
    } else {
        drawGame(graphics);
    }
}


void GameCanvas::method_164() {
    int var1;
    for (var1 = 0; var1 < 10; ++var1) {
        activeKeys[var1] = false;
    }

    for (var1 = 0; var1 < 7; ++var1) {
        activeActions[var1] = false;
    }
}

void GameCanvas::handleUpdatedInput() {
    int var1 = 0;
    int var2 = 0;
    int var3 = field_232;

    int var4;
    for (var4 = 0; var4 < 10; ++var4) {
        if (activeKeys[var4]) {
            var1 += field_231[var3][var4][0];
            var2 += field_231[var3][var4][1];
        }
    }

    for (var4 = 0; var4 < 7; ++var4) {
        if (activeActions[var4]) {
            var1 += field_230[var4][0];
            var2 += field_230[var4][1];
        }
    }

    gamePhysics->method_30(var1, var2);
}

void GameCanvas::processTimers() {
    for (auto i = timers.begin(); i != timers.end(); ) {
        if (i->ready()) {
            method_150(i->getId());
            i = timers.erase(i);
        } else {
            i++;
        }
    }
}

void GameCanvas::processKeyPressed(int keyCode) {
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

void GameCanvas::processKeyReleased(int keyCode) {
    int action = getGameAction(keyCode);
    int numKey;
    if ((numKey = keyCode - 48) >= 0 && numKey < 10) {
        activeKeys[numKey] = false;
    } else if (action >= 0 && action < 7) {
        activeActions[action] = false;
    }

    handleUpdatedInput();
}

void GameCanvas::init(GamePhysics *gamePhysics) {
    this->gamePhysics = gamePhysics;
    gamePhysics->setMinimalScreenWH(width < height2 ? width : height2);
}

void GameCanvas::scheduleGameTimerTask(std::string timerMessage, int delayMs) {
    timerTriggered = false;
    ++timerId;
    this->timerMessage = timerMessage;
    timers.push_back(Timer(timerId, delayMs));
}

void GameCanvas::setMenuManager(MenuManager *menuManager) {
    this->menuManager = menuManager;
}

void GameCanvas::method_168(Command *var1, Displayable *var2) {
    (void)var2;
    if (var1 == commandMenu) {
        menuManager->field_377 = true;
        micro->gameToMenu();
    }
}

void GameCanvas::keyPressed(int var1) {
    if (Micro::isInGameMenu && menuManager != nullptr) {
        menuManager->processKeyCode(var1);
    }

    processKeyPressed(var1);
}

void GameCanvas::keyReleased(int var1) {
    processKeyReleased(var1);
}

void GameCanvas::commandAction(Command *var1, Displayable *var2) {
    if (Micro::isInGameMenu && menuManager != nullptr) {
        menuManager->method_206(var1, var2);
    } else {
        method_168(var1, var2);
    }
}

void GameCanvas::removeMenuCommand() {
    removeCommand(commandMenu);
}

void GameCanvas::addMenuCommand() {
    addCommand(commandMenu);
}