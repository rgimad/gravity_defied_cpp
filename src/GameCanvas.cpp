#include "GameCanvas.h"

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
        this->splashImage = Image::createImage("assets/splash.png")
        this->logoImage = Image::createImage("assets/logo.png");
    } catch (std::exception &e) {
    }

    repaint();
    isHasPinterEvents = hasPointerEvents();
    this->micro = micro;
    updateSizeAndRepaint();
    font = Font::getFont(64, 1, 0);

    try {
        this->helmetImage = Image::createImage("assets/helmet.png")
    } catch (std::exception &e) {
        this->helmetImage = Image::createImage(1, 1)
    }

    helmetSpriteWidth = helmetImage->getWidth() / 6;
    helmetSpriteHeight = helmetImage->getHeight() / 6;

    try {
        spritesImage = Image::createImage("assets/sprites.png");
    } catch (std::exception &e) {
        spritesImage = onePixImage;
    }

    dx = 0;
    dy = this.height2;
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
    if (isHasPinterEvents && field_205) {
        height2 -= 80;
    }

    repaint();
}

/*

TODO

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
    // synchronized (this.objectForSyncronization) { TODO
        if ((var1 & 1) != 0) {
            try {
                if (this.fenderImage == nullptr) {
                    fenderImage = Image::createImage("assets/fender.png");
                    fenderSpriteWidth = fenderImage.getWidth() / 6;
                    fenderSpriteHeight = fenderImage.getHeight() / 6;
                }

                if (engineImage == nullptr) {
                    engineImage = Image::createImage("assets/engine.png");
                    engineSpriteWidth = engineImage.getWidth() / 6;
                    engineSpriteHeight = engineImage.getHeight() / 6;
                }
            } catch (std::exception &e) {
                fenderImage = engineImage = nullptr;
                var1 &= -2;
            }
        } else {
            engineImage = fenderImage = nullptr;
            System.gc();
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

            bodyPartsSpriteWidth[1] = bodyPartsImages[1].getWidth() / 6;
            bodyPartsSpriteHeight[1] = bodyPartsImages[1].getHeight() / 3;

            try {
                bodyPartsImages[0] = Image::createImage("assets/bluearm.png");
            } catch (std::exception &e) {
                bodyPartsImages[0] = bodyPartsImages[1];
            }

            bodyPartsSpriteWidth[0] = bodyPartsImages[0].getWidth() / 6;
            bodyPartsSpriteHeight[0] = bodyPartsImages[0].getHeight() / 3;

            try {
                bodyPartsImages[2] = Image::createImage("assets/bluebody.png");
            } catch (std::exception &e) {
                bodyPartsImages[2] = bodyPartsImages[1];
            }

            bodyPartsSpriteWidth[2] = bodyPartsImages[2].getWidth() / 6;
            bodyPartsSpriteHeight[2] = bodyPartsImages[2].getHeight() / 3;
        } else {
            bodyPartsImages[1] = bodyPartsImages[2] = bodyPartsImages[0] = nullptr;
        }

        return var1;
    // }
}

/*

// $FF: renamed from: int (int) int
public int loadSprites(int var1) {
    synchronized (this.objectForSyncronization) {
        if ((var1 & 1) != 0) {
            try {
                if (this.fenderImage == null) {
                    this.fenderImage = Image.createImage("/fender.png");
                    this.fenderSpriteWidth = this.fenderImage.getWidth() / 6;
                    this.fenderSpriteHeight = this.fenderImage.getHeight() / 6;
                }

                if (this.engineImage == null) {
                    this.engineImage = Image.createImage("/engine.png");
                    this.engineSpriteWidth = this.engineImage.getWidth() / 6;
                    this.engineSpriteHeight = this.engineImage.getHeight() / 6;
                }
            } catch (Throwable var7) {
                this.fenderImage = this.engineImage = null;
                var1 &= -2;
            }
        } else {
            this.engineImage = this.fenderImage = null;
            System.gc();
        }

        if ((var1 & 2) != 0) {
            try {
                if (this.bodyPartsImages[1] == null) {
                    this.bodyPartsImages[1] = Image.createImage("/blueleg.png");
                }
            } catch (Throwable ex) {
                this.bodyPartsImages[1] = null;
                this.bodyPartsImages[0] = null;
                this.bodyPartsImages[2] = null;
                var1 &= -3;
                return var1;
            }

            this.bodyPartsSpriteWidth[1] = this.bodyPartsImages[1].getWidth() / 6;
            this.bodyPartsSpriteHeight[1] = this.bodyPartsImages[1].getHeight() / 3;

            try {
                this.bodyPartsImages[0] = Image.createImage("/bluearm.png");
            } catch (Throwable ex) {
                this.bodyPartsImages[0] = this.bodyPartsImages[1];
            }

            this.bodyPartsSpriteWidth[0] = this.bodyPartsImages[0].getWidth() / 6;
            this.bodyPartsSpriteHeight[0] = this.bodyPartsImages[0].getHeight() / 3;

            try {
                this.bodyPartsImages[2] = Image.createImage("/bluebody.png");
            } catch (Throwable var5) {
                this.bodyPartsImages[2] = this.bodyPartsImages[1];
            }

            this.bodyPartsSpriteWidth[2] = this.bodyPartsImages[2].getWidth() / 6;
            this.bodyPartsSpriteHeight[2] = this.bodyPartsImages[2].getHeight() / 3;
        } else {
            this.bodyPartsImages[1] = this.bodyPartsImages[2] = this.bodyPartsImages[0] = null;
        }

        return var1;
    }
}

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

// $FF: renamed from: case () void
public void method_129() {
    this.gamePhysics.processPointerReleased();
    this.method_164();
    this.pointerX = 0;
    this.pointerY = 0;
}

// $FF: renamed from: a (b) void
public void init(GamePhysics gamePhysics) {
    this.gamePhysics = gamePhysics;
    this.gamePhysics.setMinimalScreenWH(this.width < this.height2 ? this.width : this.height2);
}

// $FF: renamed from: a (m) void
public void setMenuManager(MenuManager menuManager) {
    this.menuManager = menuManager;
}

// $FF: renamed from: a (java.lang.String) javax.microedition.lcdui.Image
public Image loadImage(String imgName) {
    Image img = null;

    try {
        img = Image.createImage(imgName);
    } catch (IOException var3) {
    }

    return img;
}

// $FF: renamed from: do (int, int) void
public void setViewPosition(int dx, int dy) {
    this.dx = dx;
    this.dy = dy;
    this.gamePhysics.setRenderMinMaxX(-dx, -dx + this.width);
}

// $FF: renamed from: char () int
public int getDx() {
    return this.dx;
}

// $FF: renamed from: q (int) int
private int addDx(int x) {
    return x + this.dx;
}

// $FF: renamed from: r (int) int
private int addDy(int y) {
    return -y + this.dy;
}

// $FF: renamed from: new () void
public void renderGreyCircles() {
    this.graphics.setColor(255, 255, 255);
    this.graphics.fillRect(0, this.height2, this.width, 80);
    byte size = 35;
    int centerX = this.width / 2;
    int var3 = this.height2 + 40;
    this.graphics.setColor(150, 0, 0);
    if (this.pointerX != 0 || this.pointerY != 0) {
        int var10000 = (int) (((long) ((int) ((long) this.field_206 * 11796480L >> 16)) << 32) / 205887L >> 16) >> 16;
        int var4 = var10000 - var10000 % 45;
        var4 -= 90;
        this.graphics.fillArc(centerX - size, var3 - size, 2 * size, 2 * size, var4 - 22, 45);
    }

    this.graphics.setColor(0, 0, 0);
    this.graphics.drawArc(centerX - size, var3 - size, 2 * size, 2 * size, 0, 360);
    size = 2;
    this.graphics.fillArc(centerX - size, var3 - size, 2 * size, 2 * size, 0, 360);
}

// $FF: renamed from: a (int, int, int, int) void
public void drawLine(int x, int y, int x2, int y2) {
    this.graphics.drawLine(this.addDx(x), this.addDy(y), this.addDx(x2), this.addDy(y2));
}

// $FF: renamed from: do (int, int, int, int) void
public void drawLineF16(int x, int y, int x2, int y2) {
    this.graphics.drawLine(this.addDx(x << 2 >> 16), this.addDy(y << 2 >> 16), this.addDx(x2 << 2 >> 16), this.addDy(y2 << 2 >> 16));
}

// $FF: renamed from: a (int, int, int, int, int) void
public void renderBodyPart(int var1, int var2, int var3, int var4, int var5) {
    this.renderBodyPart(var1, var2, var3, var4, var5, 32768);
}

// $FF: renamed from: a (int, int, int, int, int, int) void
public void renderBodyPart(int var1, int var2, int var3, int var4, int bodyPartNo, int var6) {
    int x = this.addDx((int) ((long) var3 * (long) var6 >> 16) + (int) ((long) var1 * (long) (65536 - var6) >> 16) >> 16);
    int y = this.addDy((int) ((long) var4 * (long) var6 >> 16) + (int) ((long) var2 * (long) (65536 - var6) >> 16) >> 16);
    int angleFP16 = MathF16.atan2(var3 - var1, var4 - var2);
    int spriteNo = 0;
    switch (bodyPartNo) {
        case 0:
            spriteNo = this.calcSpriteNo(angleFP16, 0, 205887, 16, false);
            break;
        case 1:
            spriteNo = this.calcSpriteNo(angleFP16, 0, 205887, 16, false);
            break;
        case 2:
            spriteNo = this.calcSpriteNo(angleFP16, 0, 205887, 16, false);
    }

    if (this.bodyPartsImages[bodyPartNo] != null) {
        int centerX = x - this.bodyPartsSpriteWidth[bodyPartNo] / 2;
        int centerY = y - this.bodyPartsSpriteHeight[bodyPartNo] / 2;
        this.graphics.setClip(centerX, centerY, this.bodyPartsSpriteWidth[bodyPartNo], this.bodyPartsSpriteHeight[bodyPartNo]);
        this.graphics.drawImage(this.bodyPartsImages[bodyPartNo], centerX - this.bodyPartsSpriteWidth[bodyPartNo] * (spriteNo % 6), centerY - this.bodyPartsSpriteHeight[bodyPartNo] * (spriteNo / 6), 20);
        this.graphics.setClip(0, 0, this.width, this.getHeight());
    }
}

// $FF: renamed from: if (int, int, int, int) void
public void method_142(int var1, int var2, int var3, int var4) {
    ++var3;
    int var5 = this.addDx(var1 - var3);
    int var6 = this.addDy(var2 + var3);
    int var7 = var3 << 1;
    if ((var4 = -((int) (((long) ((int) ((long) var4 * 11796480L >> 16)) << 32) / 205887L >> 16))) < 0) {
        var4 += 360;
    }

    this.graphics.drawArc(var5, var6, var7, var7, (var4 >> 16) + 170, 90);
}

// $FF: renamed from: do (int, int, int) void
public void drawCircle(int x, int y, int size) {
    int radius = size / 2;
    int localX = this.addDx(x - radius);
    int localY = this.addDy(y + radius);
    this.graphics.drawArc(localX, localY, size, size, 0, 360);
}

// $FF: renamed from: for (int, int, int, int) void
public void fillRect(int x, int y, int w, int h) {
    int var5 = this.addDx(x);
    int var6 = this.addDy(y);
    this.graphics.fillRect(var5, var6, w, h);
}

// $FF: renamed from: a (int, int) void
public void drawForthSpriteByCenter(int centerX, int centerY) {
    int halfSizeX = spriteSizeX[4] / 2;
    int halfSizeY = spriteSizeY[4] / 2;
    this.drawSprite(this.graphics, 4, this.addDx(centerX - halfSizeX), this.addDy(centerY + halfSizeY));
}

// $FF: renamed from: new (int, int, int) void
public void method_146(int var1, int var2, int var3) {
    int var4 = this.calcSpriteNo(var3, -102943, 411774, 32, true);
    if (this.helmetImage != null) {
        int var5 = this.addDx(var1) - this.helmetSpriteWidth / 2;
        int var6 = this.addDy(var2) - this.helmetSpriteHeight / 2;
        this.graphics.setClip(var5, var6, this.helmetSpriteWidth, this.helmetSpriteHeight);
        this.graphics.drawImage(this.helmetImage, var5 - this.helmetSpriteWidth * (var4 % 6), var6 - this.helmetSpriteHeight * (var4 / 6), 20);
        this.graphics.setClip(0, 0, this.width, this.getHeight());
    }

}

// $FF: renamed from: if (int, int, int, int, int, int) void
public void method_147(int var1, int var2, int var3, int var4, int var5, int var6) {
}

// $FF: renamed from: a (long) void
public void drawTime(long time10Ms) {
    int seconds = (int) (time10Ms / 100L);
    int time10MsPart = (int) (time10Ms % 100L);
    if (this.timeInSeconds != seconds || stringWithTime == null) {
        String zeroPadding;
        if (seconds % 60 >= 10) {
            zeroPadding = "";
        } else {
            zeroPadding = "0";
        }

        stringWithTime = "" + seconds / 60 + ":" + zeroPadding + seconds % 60 + ".";
        this.timeInSeconds = seconds;
    }

    if (time10MsToStringCache[time10MsPart] == null) {
        String zeroPadding;
        if (time10MsPart >= 10) {
            zeroPadding = "";
        } else {
            zeroPadding = "0";
        }

        time10MsToStringCache[time10MsPart] = zeroPadding + time10Ms % 100L;
    }

    if (time10Ms > 3600000L) {
        this.setColor(0, 0, 0);
        this.graphics.drawString("0:00.", this.width - defaultFontWidth00, this.height2 - 5, 40);
        this.graphics.drawString("00", this.width - defaultFontWidth00, this.height2 - 5, 36);
    } else {
        this.setColor(0, 0, 0);
        this.graphics.drawString(stringWithTime, this.width - defaultFontWidth00, this.height2 - 5, 40);
        this.graphics.drawString(time10MsToStringCache[time10MsPart], this.width - defaultFontWidth00, this.height2 - 5, 36);
    }
}

// $FF: renamed from: a (java.lang.String, int) void
public void scheduleGameTimerTask(String var1, int delayMs) {
    this.field_182 = false;
    ++this.countOfScheduledTimers;
    this.field_210 = var1;
    this.timer.schedule(new TimerOrMotoPartOrMenuElem(this.countOfScheduledTimers, this.micro), (long) delayMs);
}

// $FF: renamed from: try (int) void
public void method_150(int var1) {
    if (this.countOfScheduledTimers == var1) {
        this.field_182 = true;
    }
}

// $FF: renamed from: do () void
public static void method_151() {
    field_226 += 655;
    int var0 = '耀' + ((MathF16.sinF16(field_226) < 0 ? -MathF16.sinF16(field_226) : MathF16.sinF16(field_226)) >> 1);
    flagAnimationTime += (int) (6553L * (long) var0 >> 16);
}

// $FF: renamed from: for (int, int) void
public void renderStartFlag(int x, int y) {
    if (flagAnimationTime > 229376) {
        flagAnimationTime = 0;
    }

    this.setColor(0, 0, 0);
    this.drawLine(x, y, x, y + 32);
    this.drawSprite(this.graphics, this.startFlagAnimationTimeToSpriteNo[flagAnimationTime >> 16], this.addDx(x), this.addDy(y) - 32);
}

// $FF: renamed from: int (int, int) void
public void renderFinishFlag(int x, int y) {
    if (flagAnimationTime > 229376) {
        flagAnimationTime = 0;
    }

    this.setColor(0, 0, 0);
    this.drawLine(x, y, x, y + 32);
    this.drawSprite(this.graphics, this.finishFlagAnumationTimeToSpriteNo[flagAnimationTime >> 16], this.addDx(x), this.addDy(y) - 32);
}

// $FF: renamed from: for (int, int, int) void
public void drawWheelTires(int x, int y, int wheelIsThin) {
    byte spriteNo;
    if (wheelIsThin == 1) {
        spriteNo = 0;
    } else {
        spriteNo = 1;
    }

    int spriteHalfX = spriteSizeX[spriteNo] / 2;
    int spriteHalfY = spriteSizeY[spriteNo] / 2;
    int centerX = this.addDx(x - spriteHalfX);
    int centerY = this.addDy(y + spriteHalfY);
    this.drawSprite(this.graphics, spriteNo, centerX, centerY);
}

// $FF: renamed from: a (int, int, int, int, boolean) int
int calcSpriteNo(int angleF16, int var2, int var3, int var4, boolean var5) {
    for (angleF16 += var2; angleF16 < 0; angleF16 += var3) {
    }

    while (angleF16 >= var3) {
        angleF16 -= var3;
    }

    if (var5) {
        angleF16 = var3 - angleF16;
    }

    int var6;
    return (var6 = (int) ((long) ((int) (((long) angleF16 << 32) / (long) var3 >> 16)) * (long) (var4 << 16) >> 16)) >> 16 < var4 - 1 ? var6 >> 16 : var4 - 1;
}

// $FF: renamed from: int (int, int, int) void
public void renderEngine(int x, int y, int angleF16) {
    int spriteNo = this.calcSpriteNo(angleF16, -247063, 411774, 32, true);
    int centerX = this.addDx(x) - this.engineSpriteWidth / 2;
    int centerY = this.addDy(y) - this.engineSpriteHeight / 2;
    if (this.engineImage != null) {
        this.graphics.setClip(centerX, centerY, this.engineSpriteWidth, this.engineSpriteHeight);
        this.graphics.drawImage(this.engineImage, centerX - this.engineSpriteWidth * (spriteNo % 6), centerY - this.engineSpriteHeight * (spriteNo / 6), 20);
        this.graphics.setClip(0, 0, this.width, this.getHeight());
    }
}

// $FF: renamed from: a (int, int, int) void
public void renderFender(int x, int y, int angleF16) {
    int spriteNo = this.calcSpriteNo(angleF16, -185297, 411774, 32, true);
    if (this.fenderImage != null) {
        int centerX = this.addDx(x) - this.fenderSpriteWidth / 2;
        int centerY = this.addDy(y) - this.fenderSpriteHeight / 2;
        this.graphics.setClip(centerX, centerY, this.fenderSpriteWidth, this.fenderSpriteHeight);
        this.graphics.drawImage(this.fenderImage, centerX - this.fenderSpriteWidth * (spriteNo % 6), centerY - this.fenderSpriteHeight * (spriteNo / 6), 20);
        this.graphics.setClip(0, 0, this.width, this.getHeight());
    }
}

// $FF: renamed from: try () void
public void clearScreenWithWhite() {
    this.graphics.setColor(255, 255, 255);
    this.graphics.fillRect(0, 0, this.width, this.height2);
}

// $FF: renamed from: if (int, int, int) void
public void setColor(int red, int green, int blue) {
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

    this.graphics.setColor(red, green, blue);
}

// $FF: renamed from: a (javax.microedition.lcdui.Graphics) void
protected void render_160(Graphics g) {
    synchronized (this.objectForSyncronization) {
        if (Micro.field_249 && !this.micro.field_242) {
            if (this.useScreenBuffer) {
                this.graphics = this.screenBufferGraphics;
            } else {
                this.graphics = g;
            }

            int var3;
            if (this.field_184 != 0) {
                if (this.field_184 == 1) {
                    this.graphics.setColor(255, 255, 255);
                    this.graphics.fillRect(0, 0, this.getWidth(), this.getHeight());
                    if (this.logoImage != null) {
                        this.graphics.drawImage(this.logoImage, this.getWidth() / 2, this.getHeight() / 2, 3);
                        this.drawSprite(this.graphics, 16, this.getWidth() - spriteSizeX[16] - 5, this.getHeight() - spriteSizeY[16] - 7);
                        this.drawSprite(this.graphics, 17, this.getWidth() - spriteSizeX[17] - 4, this.getHeight() - spriteSizeY[17] - spriteSizeY[16] - 9);
                    }
                } else {
                    this.graphics.setColor(255, 255, 255);
                    this.graphics.fillRect(0, 0, this.getWidth(), this.getHeight());
                    if (this.splashImage != null) {
                        this.graphics.drawImage(this.splashImage, this.getWidth() / 2, this.getHeight() / 2, 3);
                    }
                }

                var3 = (int) (((long) (Micro.gameLoadingStateStage << 16) << 32) / 655360L >> 16);
                this.method_161(var3, true);
            } else {
                if (this.height != this.getHeight()) {
                    this.updateSizeAndRepaint();
                }

                this.gamePhysics.setMotoComponents();
                this.setViewPosition(-this.gamePhysics.getCamPosX() + this.field_178 + this.width / 2, this.gamePhysics.getCamPosY() + this.field_179 + this.height2 / 2);
                this.gamePhysics.renderGame(this);
                if (this.isDrawingTime) {
                    this.drawTime(this.micro.gameTimeMs / 10L);
                }

                if (this.field_210 != null) {
                    this.setColor(0, 0, 0);
                    this.graphics.setFont(this.font);
                    if (this.height2 <= 128) {
                        this.graphics.drawString(this.field_210, this.width / 2, 1, 17);
                    } else {
                        this.graphics.drawString(this.field_210, this.width / 2, this.height2 / 4, 33);
                    }

                    if (this.field_182) {
                        this.field_182 = false;
                        this.field_210 = null;
                    }
                }

                var3 = this.gamePhysics.method_52();
                this.method_161(var3, false);
                if (this.isHasPinterEvents && this.field_205) {
                    this.renderGreyCircles();
                }
            }

            this.graphics = null;
            if (this.useScreenBuffer) {
                g.drawImage(this.screenBuffer, 0, 0, 20);
            }
        }
    }
}

// $FF: renamed from: a (int, boolean) void
public void method_161(int var1, boolean mode) {
    int h = mode ? this.height : this.height2;
    this.setColor(0, 0, 0);
    this.graphics.fillRect(1, h - 4, this.width - 2, 3);
    this.setColor(255, 255, 255);
    this.graphics.fillRect(2, h - 3, (int) ((long) (this.width - 4 << 16) * (long) var1 >> 16) >> 16, 1);
}

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

// $FF: renamed from: a (int) void
public void method_163(int var1) {
    this.field_232 = var1;
}

// $FF: renamed from: a () void
private void method_164() {
    int var1;
    for (var1 = 0; var1 < 10; ++var1) {
        this.activeKeys[var1] = false;
    }

    for (var1 = 0; var1 < 7; ++var1) {
        this.activeActions[var1] = false;
    }
}

// $FF: renamed from: xa () void
private void handleUpdatedInput() {
    int var1 = 0;
    int var2 = 0;
    int var3 = this.field_232;

    int var4;
    for (var4 = 0; var4 < 10; ++var4) {
        if (this.activeKeys[var4]) {
            var1 += this.field_231[var3][var4][0];
            var2 += this.field_231[var3][var4][1];
        }
    }

    for (var4 = 0; var4 < 7; ++var4) {
        if (this.activeActions[var4]) {
            var1 += this.field_230[var4][0];
            var2 += this.field_230[var4][1];
        }
    }

    this.gamePhysics.method_30(var1, var2);
}

// $FF: renamed from: if (int) void
protected void processKeyPressed(int keyCode) {
    int action = this.getGameAction(keyCode);
    int numKey;
    // KEY_NUM0 - KEY_NUM10 is 48-58
    if ((numKey = keyCode - 48) >= 0 && numKey < 10) {
        this.activeKeys[numKey] = true;
    } else if (action >= 0 && action < 7) {
        this.activeActions[action] = true;
    }

    this.handleUpdatedInput();
}

// $FF: renamed from: for (int) void
protected void processKeyReleased(int keyCode) {
    int action = this.getGameAction(keyCode);
    int numKey;
    if ((numKey = keyCode - 48) >= 0 && numKey < 10) {
        this.activeKeys[numKey] = false;
    } else if (action >= 0 && action < 7) {
        this.activeActions[action] = false;
    }

    this.handleUpdatedInput();
}

// $FF: renamed from: a (javax.microedition.lcdui.Command, javax.microedition.lcdui.Displayable) void
public void method_168(Command var1, Displayable var2) {
    if (var1 == this.commandMenu) {
        this.menuManager.field_377 = true;
        this.micro.gameToMenu();
    }
}

protected void hideNotify() {
    if (!Micro.isInGameMenu) {
        Micro.isPaused = true;
        this.micro.gameToMenu();
    }
}

protected void showNotify() {
    Micro.isPaused = false;
}

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