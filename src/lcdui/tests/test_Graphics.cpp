#include "test_Graphics.h"

void test_Graphics() {
    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Window *window = SDL_CreateWindow(
        "Graphics test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        320, 240,
        SDL_WINDOW_SHOWN);

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
    SDL_RenderClear(renderer);

    Graphics g(renderer);

    g.setClip(50, 50, 100, 100);

    for (int i = 0; i < 180; i++) {
        g.setColor(0, i/2, i);
        g.drawLine(15, 15, 100, i);
    }

    g.setClip(0, 0, 320, 240);

    for (int i = 50; i < 180; i++) {
        g.setColor(i/2, 0, i/3);
        g.fillRect(i + 120, i, 100, 100);
    }

    g.setColor(255, 0, 0);

    g.drawArc(30, 10, 50, 100, 30, 220);

    g.drawArc(100, 10, 100, 100, 0, 127);

    g.drawArc(200, 200, 30, 30, 0, 360);

    // TODO: test getAnchorX
    // TODO: test getAnchorY

    SDL_RenderPresent(renderer); // Send to screen

    // Event loop
    SDL_Event event;
	while (SDL_WaitEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
            break;
        }
	}

    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
