#include "test_GameCanvas.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <memory>

#define private public

#include "../GameCanvas.h"

void test_GameCanvas() {
    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error(IMG_GetError());
    }

    if (TTF_Init() == -1) {
        throw std::runtime_error(TTF_GetError());
    }

    SDL_Window *window = SDL_CreateWindow(
        0,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        320, 240,
        SDL_WINDOW_SHOWN);

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
    SDL_RenderClear(renderer);

    Graphics g(renderer);

    std::unique_ptr<Font> font {Font::getFont(Font::FACE_SYSTEM, Font::STYLE_BOLD, Font::SIZE_MEDIUM)};
    g.setFont(font.get());

    std::unique_ptr<Micro> micro = std::make_unique<Micro>();
    std::unique_ptr<GameCanvas> gameCanvas = std::make_unique<GameCanvas>(micro.get());
    gameCanvas->loadSprites(1 | 2);

    int width = Graphics::getWidth()/*, height = Graphics::getHeight()*/;
    int stepX = 50, stepY = 50;
    int x = stepX, y = stepY;

    auto updateXY = [&x, &y, stepX, stepY, width](){
        x += stepX;
        if (x >= width) {
            y += stepY;
            x = stepX;
        }
    };

    auto F16 = [](float x){return x * 65536;};

    bool quit = false;
    int angleF16 = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent( &e ) != 0){
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);

        x = stepX, y = stepY;

        gameCanvas->graphics = &g;

        gameCanvas->clearScreenWithWhite();

        gameCanvas->renderEngine(x, y, angleF16);
        updateXY();
        gameCanvas->renderStartFlag(x, y);
        updateXY();
        gameCanvas->renderFinishFlag(x, y);
        updateXY();
        gameCanvas->renderFender(x, y, angleF16);
        updateXY();
        gameCanvas->drawWheelTires(x, y, 0);
        updateXY();
        gameCanvas->drawWheelTires(x, y, 1);
        updateXY();
        gameCanvas->drawForthSpriteByCenter(x, y);
        updateXY();
        gameCanvas->method_146(x, y, angleF16);
        updateXY();
        gameCanvas->renderBodyPart(F16(x - 10), F16(y - 10), F16(x + 10), F16(y + 10), 0);
        updateXY();
        gameCanvas->renderBodyPart(F16(x - 10), F16(y - 10), F16(x + 10), F16(y + 10), 1);
        updateXY();
        gameCanvas->renderBodyPart(F16(x - 10), F16(y - 10), F16(x + 10), F16(y + 10), 2);
        updateXY();
        gameCanvas->drawLine(x - 10, y - 10, x + 10, y + 10);
        updateXY();
        gameCanvas->drawCircle(x, y, 20);
        updateXY();
        gameCanvas->fillRect(x, y, 10, 10);

        gameCanvas->field_184 = 0;
        Micro::field_249 = true;
        gameCanvas->render_160(&g);
        gameCanvas->field_184 = 1;
        Micro::field_249 = false;

        angleF16 += 1000;

        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}
