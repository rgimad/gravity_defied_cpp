#include "GameCanvas.h"

#include "MathF16.h"

void GameCanvas::repaint() {
    // TODO
}

void GameCanvas::serviceRepaints() {
    // TODO
}

bool GameCanvas::hasPointerEvents() {
    // TODO
    return false;
}

GameCanvas::GameCanvas(Micro *micro) {
    // TODO: looks like we don't need the code below
    // String platform;
    // String var3;
    // if ((platform = System.getProperty("microedition.platform")).startsWith("SonyEricsson") && ((var3 = platform.substring(13)).startsWith("T610") || var3.startsWith("T630") || var3.startsWith("Z600"))) {
    //     try {
    //         this.screenBuffer = Image.createImage(this.getWidth(), this.getHeight());
    //         this.screenBufferGraphics = this.screenBuffer.getGraphics();
    //         this.useScreenBuffer = true;
    //     } catch (Throwable var7) {
    //     }
    // }

    try {
        this->splashImage = Image::createImage("assets/splash.png");
        this->logoImage = Image::createImage("assets/logo.png");
    } catch (std::exception &e) {
    }

    repaint();
    isHasPinterEvents = hasPointerEvents();
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
        g->setClip(0, 0, Graphics::getWidth(), Graphics::getHeight());
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
    width = Graphics::getWidth();
    height = height2 = Graphics::getHeight();
    if (isHasPinterEvents && field_205) {
        height2 -= 80;
    }

    repaint();
}

/*

// TODO

// $FF: renamed from: a (javax.microedition.lcdui.Image, int, int) javax.microedition.lcdui.Image[]
public Image[] splitImageToSprites(Image img, int spritesByW, int spritesByH) {
    int spriteW = img.getWidth() / spritesByW;
    int spriteH = img.getHeight() / spritesByH;
    Image[] result = new Image[spritesByW * spritesByH];

    for (int i = 0; i < spritesByW * spritesByH; ++i) {
        result[i] = Image.createImage(spriteW, spriteH);
        result[i].getGraphics().drawImage(img, -spriteW * (i % spritesByW), -spriteH * (i / spritesByW), 20);
    }

    return result;
}

*/

int GameCanvas::loadSprites(int var1) {
    // synchronized (this.objectForSyncronization) { // TODO
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
            // System.gc(); // TODO
        }

        if ((var1 & 2) != 0) {
            try {
                if (bodyPartsImages[1] == nullptr) {
                    bodyPartsImages[1] = Image::createImage("assets/blueleg.png");
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
                bodyPartsImages[0] = Image::createImage("assets/bluearm.png");
            } catch (std::exception &e) {
                bodyPartsImages[0] = bodyPartsImages[1];
            }

            bodyPartsSpriteWidth[0] = bodyPartsImages[0]->getWidth() / 6;
            bodyPartsSpriteHeight[0] = bodyPartsImages[0]->getHeight() / 3;

            try {
                bodyPartsImages[2] = Image::createImage("assets/bluebody.png");
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

/*

// TODO

// $FF: renamed from: a (java.lang.String, int, int) javax.microedition.lcdui.Image[]
public Image[] loadsSprites(String imageName, int spitesByW, int spritesByH) {
    Image img;
    try {
        img = Image.createImage(imageName);
    } catch (IOException var8) {
        Image[] var6 = new Image[spitesByW * spritesByH];

        for (int i = 0; i < spitesByW * spritesByH; ++i) {
            var6[i] = this.onePixImage;
        }

        return var6;
    }

    return this.splitImageToSprites(img, spitesByW, spritesByH);
}

*/

void GameCanvas::method_129() {
    // gamePhysics->processPointerReleased(); // TODO
    method_164();
    pointerX = 0;
    pointerY = 0;
}

// TODO
// void GameCanvas::init(GamePhysics *gamePhysics) {
//     gamePhysics = gamePhysics;
//     gamePhysics->setMinimalScreenWH(width < height2 ? width : height2);
// }

/*

// $FF: renamed from: a (m) void
public void setMenuManager(MenuManager menuManager) {
    this.menuManager = menuManager;
}

*/

Image* GameCanvas::loadImage(std::string imgName) {
    Image *img = nullptr;

    try {
        img = Image::createImage(imgName.c_str());
    } catch (std::exception &e) {
    }

    return img;
}

void GameCanvas::setViewPosition(int dx, int dy) {
    dx = dx;
    dy = dy;
    // gamePhysics->setRenderMinMaxX(-dx, -dx + width); // TODO
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

void GameCanvas::renderGreyCircles() {
    graphics->setColor(255, 255, 255);
    graphics->fillRect(0, height2, width, 80);
    int size = 35;
    int centerX = width / 2;
    int var3 = height2 + 40;
    graphics->setColor(150, 0, 0);
    if (pointerX != 0 || pointerY != 0) {
        int var10000 = (int) (((long) ((int) ((long) field_206 * 11796480L >> 16)) << 32) / 205887L >> 16) >> 16; // TODO
        int var4 = var10000 - var10000 % 45;
        var4 -= 90;
        graphics->fillArc(centerX - size, var3 - size, 2 * size, 2 * size, var4 - 22, 45);
    }

    graphics->setColor(0, 0, 0);
    graphics->drawArc(centerX - size, var3 - size, 2 * size, 2 * size, 0, 360);
    size = 2;
    graphics->fillArc(centerX - size, var3 - size, 2 * size, 2 * size, 0, 360);
}

void GameCanvas::drawLine(int x, int y, int x2, int y2) {
    graphics->drawLine(addDx(x), addDy(y), addDx(x2), addDy(y2));
}

void GameCanvas::drawLineF16(int x, int y, int x2, int y2) {
    graphics->drawLine(addDx(x << 2 >> 16), addDy(y << 2 >> 16), addDx(x2 << 2 >> 16), addDy(y2 << 2 >> 16));
}

void GameCanvas::renderBodyPart(int var1, int var2, int var3, int var4, int var5) {
    renderBodyPart(var1, var2, var3, var4, var5, 32768);
}

void GameCanvas::renderBodyPart(int var1, int var2, int var3, int var4, int bodyPartNo, int var6) {
    int x = addDx((int) ((long) var3 * (long) var6 >> 16) + (int) ((long) var1 * (long) (65536 - var6) >> 16) >> 16); // TODO
    int y = addDy((int) ((long) var4 * (long) var6 >> 16) + (int) ((long) var2 * (long) (65536 - var6) >> 16) >> 16); // TODO
    int angleFP16 = MathF16::atan2F16(var3 - var1, var4 - var2);
    int spriteNo = 0;
    switch (bodyPartNo) {
        case 0:
            spriteNo = calcSpriteNo(angleFP16, 0, 205887, 16, false);
            break;
        case 1:
            spriteNo = calcSpriteNo(angleFP16, 0, 205887, 16, false);
            break;
        case 2:
            spriteNo = calcSpriteNo(angleFP16, 0, 205887, 16, false);
    }

    if (bodyPartsImages[bodyPartNo] != nullptr) {
        int centerX = x - bodyPartsSpriteWidth[bodyPartNo] / 2;
        int centerY = y - bodyPartsSpriteHeight[bodyPartNo] / 2;
        graphics->setClip(centerX, centerY, bodyPartsSpriteWidth[bodyPartNo], bodyPartsSpriteHeight[bodyPartNo]);
        graphics->drawImage(bodyPartsImages[bodyPartNo], centerX - bodyPartsSpriteWidth[bodyPartNo] * (spriteNo % 6), centerY - bodyPartsSpriteHeight[bodyPartNo] * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, Graphics::getHeight());
    }
}

void GameCanvas::method_142(int var1, int var2, int var3, int var4) {
    ++var3;
    int var5 = addDx(var1 - var3);
    int var6 = addDy(var2 + var3);
    int var7 = var3 << 1;
    if ((var4 = -((int) (((long) ((int) ((long) var4 * 11796480L >> 16)) << 32) / 205887L >> 16))) < 0) { // TODO
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

void GameCanvas::method_146(int var1, int var2, int var3) {
    int var4 = calcSpriteNo(var3, -102943, 411774, 32, true);
    if (helmetImage != nullptr) {
        int var5 = addDx(var1) - helmetSpriteWidth / 2;
        int var6 = addDy(var2) - helmetSpriteHeight / 2;
        graphics->setClip(var5, var6, helmetSpriteWidth, helmetSpriteHeight);
        graphics->drawImage(helmetImage, var5 - helmetSpriteWidth * (var4 % 6), var6 - helmetSpriteHeight * (var4 / 6), 20);
        graphics->setClip(0, 0, width, Graphics::getHeight());
    }

}

void GameCanvas::drawTime(long time10Ms) {
    int seconds = (int) (time10Ms / 100L);
    int time10MsPart = (int) (time10Ms % 100L);
    if (timeInSeconds != seconds /* || stringWithTime == null*/ ) { // TODO
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

/*

// $FF: renamed from: a (java.lang.String, int) void
public void scheduleGameTimerTask(String var1, int delayMs) {
    this.field_182 = false;
    ++this.countOfScheduledTimers;
    this.field_210 = var1;
    this.timer.schedule(new TimerOrMotoPartOrMenuElem(this.countOfScheduledTimers, this.micro), (long) delayMs);
}

*/

// $FF: renamed from: try (int) void
void GameCanvas::method_150(int var1) {
    if (countOfScheduledTimers == var1) {
        field_182 = true;
    }
}

void GameCanvas::method_151() {
    field_226 += 655;
    int var0 = '耀' + ((MathF16::sinF16(field_226) < 0 ? -MathF16::sinF16(field_226) : MathF16::sinF16(field_226)) >> 1); // TODO
    flagAnimationTime += (int) (6553L * (long) var0 >> 16);
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
    return (var6 = (int) ((long) ((int) (((long) angleF16 << 32) / (long) var3 >> 16)) * (long) (var4 << 16) >> 16)) >> 16 < var4 - 1 ? var6 >> 16 : var4 - 1; // TODO
}

void GameCanvas::renderEngine(int x, int y, int angleF16) {
    int spriteNo = calcSpriteNo(angleF16, -247063, 411774, 32, true);
    int centerX = addDx(x) - engineSpriteWidth / 2;
    int centerY = addDy(y) - engineSpriteHeight / 2;
    if (engineImage != nullptr) {
        graphics->setClip(centerX, centerY, engineSpriteWidth, engineSpriteHeight);
        graphics->drawImage(engineImage, centerX - engineSpriteWidth * (spriteNo % 6), centerY - engineSpriteHeight * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, Graphics::getHeight());
    }
}

void GameCanvas::renderFender(int x, int y, int angleF16) {
    int spriteNo = calcSpriteNo(angleF16, -185297, 411774, 32, true);
    if (fenderImage != nullptr) {
        int centerX = addDx(x) - fenderSpriteWidth / 2;
        int centerY = addDy(y) - fenderSpriteHeight / 2;
        graphics->setClip(centerX, centerY, fenderSpriteWidth, fenderSpriteHeight);
        graphics->drawImage(fenderImage, centerX - fenderSpriteWidth * (spriteNo % 6), centerY - fenderSpriteHeight * (spriteNo / 6), 20);
        graphics->setClip(0, 0, width, Graphics::getHeight());
    }
}

void GameCanvas::clearScreenWithWhite() {
    graphics->setColor(255, 255, 255);
    graphics->fillRect(0, 0, width, height2);
}

void GameCanvas::setColor(int red, int green, int blue) {
    /*
    // TODO
    if (Micro.isInGameMenu) {
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
    */

    graphics->setColor(red, green, blue);
}

void GameCanvas::render_160(Graphics *g) {
    // synchronized (this.objectForSyncronization) { // TODO
        if (Micro::field_249 && !micro->field_242) {
            graphics = g;

            int var3;
            if (field_184 != 0) {
                if (field_184 == 1) {
                    graphics->setColor(255, 255, 255);
                    graphics->fillRect(0, 0, Graphics::getWidth(), Graphics::getHeight());
                    if (logoImage != nullptr) {
                        graphics->drawImage(logoImage, Graphics::getWidth() / 2, Graphics::getHeight() / 2, 3);
                        drawSprite(graphics, 16, Graphics::getWidth() - spriteSizeX[16] - 5, Graphics::getHeight() - spriteSizeY[16] - 7);
                        drawSprite(graphics, 17, Graphics::getWidth() - spriteSizeX[17] - 4, Graphics::getHeight() - spriteSizeY[17] - spriteSizeY[16] - 9);
                    }
                } else {
                    graphics->setColor(255, 255, 255);
                    graphics->fillRect(0, 0, Graphics::getWidth(), Graphics::getHeight());
                    if (splashImage != nullptr) {
                        graphics->drawImage(splashImage, Graphics::getWidth() / 2, Graphics::getHeight() / 2, 3);
                    }
                }

                var3 = (int) (((long) (Micro::gameLoadingStateStage << 16) << 32) / 655360L >> 16); // TODO
                method_161(var3, true);
            } else {
                if (height != Graphics::getHeight()) {
                    updateSizeAndRepaint();
                }

                // gamePhysics->setMotoComponents(); // TODO
                // setViewPosition(-gamePhysics->getCamPosX() + field_178 + width / 2, gamePhysics->getCamPosY() + field_179 + height2 / 2); // TODO
                // gamePhysics->renderGame(this); // TODO
                if (isDrawingTime) {
                    drawTime(micro->gameTimeMs / 10L);
                }

                // TODO
                // if (field_210 != null) {
                //     setColor(0, 0, 0);
                //     graphics->setFont(font);
                //     if (height2 <= 128) {
                //         graphics->drawString(field_210, width / 2, 1, 17);
                //     } else {
                //         graphics->drawString(field_210, width / 2, height2 / 4, 33);
                //     }

                //     if (field_182) {
                //         field_182 = false;
                //         field_210 = null;
                //     }
                // }

                // var3 = gamePhysics->method_52(); // TODO
                method_161(var3, false);
                if (isHasPinterEvents && field_205) {
                    renderGreyCircles();
                }
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
    graphics->fillRect(2, h - 3, (int) ((long) (width - 4 << 16) * (long) var1 >> 16) >> 16, 1); // TODO
}

/*

// TODO
// Looks like this part adds the touchscreen support

// $FF: renamed from: if (int, int) void
private void processPointerDragged(int x, int y) {
    if (!Micro.isInGameMenu) {
        byte var3 = 0;
        byte var4 = 0;
        this.pointerX = x;
        this.pointerY = y;
        int var5 = x << 16;
        int var6 = y << 16;
        int var7 = this.width / 2 << 16;
        int var8 = this.height2 + 40 << 16;
        if (this.isHasPinterEvents && this.field_205) {
            int angle = MathF16.atan2(var5 - var7, var6 - var8);

            for (angle += 25735; angle < 0; angle += 411774) {
            }

            while (angle > 411774) {
                angle -= 411774;
            }

            this.field_206 = angle;
            char var10 = '줏';
            if (51471 >= angle) {
                var3 = -1;
            } else if (angle < (int) ((long) var10 * 131072L >> 16)) {
                var3 = -1;
                var4 = 1;
            } else if (angle < (int) ((long) var10 * 196608L >> 16)) {
                var4 = 1;
            } else if (angle < (int) ((long) var10 * 262144L >> 16)) {
                var3 = 1;
                var4 = 1;
            } else if (angle < (int) ((long) var10 * 327680L >> 16)) {
                var3 = 1;
            } else if (angle < (int) ((long) var10 * 393216L >> 16)) {
                var3 = 1;
                var4 = -1;
            } else if (angle < (int) ((long) var10 * 458752L >> 16)) {
                var4 = -1;
            } else if (angle < (int) ((long) var10 * 524288L >> 16)) {
                var3 = -1;
                var4 = -1;
            }

            this.gamePhysics.method_30(var3, var4);
        }
    }
}

public void pointerPressed(int x, int y) {
    if (!Micro.isInGameMenu) {
        this.processPointerDragged(x, y);
    }
}

public void pointerReleased(int var1, int var2) {
    if (!Micro.isInGameMenu) {
        this.pointerX = 0;
        this.pointerY = 0;
        this.gamePhysics.processPointerReleased();
    }
}

public void pointerDragged(int x, int y) {
    if (!Micro.isInGameMenu) {
        this.processPointerDragged(x, y);
    }
}

*/

void GameCanvas::method_163(int var1) {
    field_232 = var1;
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

    // gamePhysics->method_30(var1, var2); // TODO
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

int GameCanvas::getGameAction(int key) {
    return 0; // TODO
}

/*


// $FF: renamed from: a (javax.microedition.lcdui.Command, javax.microedition.lcdui.Displayable) void
public void method_168(Command var1, Displayable var2) {
    if (var1 == this.commandMenu) {
        this.menuManager.field_377 = true;
        this.micro.gameToMenu();
    }
}

*/

void GameCanvas::hideNotify() {
    if (!Micro::isInGameMenu) {
        Micro::isPaused = true;
        micro->gameToMenu();
    }
}

void GameCanvas::showNotify() {
    Micro::isPaused = false;
}

/*

protected void keyRepeated(int var1) {
    if (Micro.isInGameMenu && this.menuManager != null) {
        this.menuManager.processNonFireKeyCode(var1);
    }
}

protected synchronized void keyPressed(int var1) {
    if (Micro.isInGameMenu && this.menuManager != null) {
        this.menuManager.processKeyCode(var1);
    }

    this.processKeyPressed(var1);
}

protected synchronized void keyReleased(int var1) {
    if (Micro.isInGameMenu) {
        MenuManager var10000 = this.menuManager;
    }

    this.processKeyReleased(var1);
}

public void paint(Graphics graphics) {
    if (Micro.isInGameMenu && this.menuManager != null) {
        this.menuManager.method_202(graphics);
    } else {
        this.render_160(graphics);
    }
}

public void commandAction(Command var1, Displayable var2) {
    if (Micro.isInGameMenu && this.menuManager != null) {
        this.menuManager.method_206(var1, var2);
    } else {
        this.method_168(var1, var2);
    }
}

// $FF: renamed from: for () void
public void removeMenuCommand() {
    this.removeCommand(this.commandMenu);
}

// $FF: renamed from: byte () void
public void addMenuCommand() {
    this.addCommand(this.commandMenu);
}

*/