#pragma once

#include "lcdui/Graphics.h"
#include "GamePhysics.h"

class GameCanvas { // extends Canvas implements CommandListener
private:
    Graphics *graphics = nullptr;
    int dx;
    int dy;
    int engineSpriteWidth;
    int engineSpriteHeight;
    int fenderSpriteWidth;
    int fenderSpriteHeight;
    GamePhysics *gamePhysics = nullptr;
    MenuManager *menuManager = nullptr;
    int field_178 = 0;
    int field_179 = 0;
    Micro *micro = nullptr;
    Font *font = nullptr;
    bool field_182 = false;
    bool isDrawingTime = true;
    int field_184 = 1;
    Image *splashImage = nullptr;
    Image *logoImage = nullptr;


public:
    static const int field_163 = 0;
    static const int field_164 = 1;
    static const int field_165 = 2;
    int width;
    int height2;
    int height;
    // static const int field_187 = 16;

    
};