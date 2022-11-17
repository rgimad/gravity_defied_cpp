#include "test_Graphics.h"

void test_Graphics() {
    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error(IMG_GetError());
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

    // g.fillArc(60, 200, 50, 50, 225, 90);
    // g.fillArc(60, 200, 50, 50, 225, 90);

    // g.fillArc(60, 200, 50, 50, 20, 90);
    // g.fillArc(60, 200, 50, 20, 0, 180);
    // g.fillArc(60, 200, 50, 50, 90, 120);
    // g.fillArc(60, 200, 50, 50, 200, 20);
    g.fillArc(60, 200, 50, 50, 200, 200); // !
    // g.fillArc(60, 200, 50, 50, 100, 250);

    // g.fillArc(60, 200, 50, 50, 0, 360);

    // g.fillArc(60, 200, 50, 30, 100, 250);

    // g.fillArc(60, 200, 30, 50, 100, 250); // !!!
    // g.fillArc(60, 200, 50, 50, 200, 150);

    // TODO: g.fillArc(60, 200, 30, 50, 100, 250); не работает
    // а g.fillArc(60, 200, 50, 30, 100, 250); работает

    // comapre with http://www.java2s.com/Tutorial/Java/0261__2D-Graphics/FillArc.htm
    // http://cs111.wellesley.edu/~cs111/archive/cs111_fall06/public_html/labs/lab12/arc.html

    Image *img1 = Image::createImage("assets/splash.png");
    Image *img2 = Image::createImage("assets/sprites.png");
    g.drawImage(img1, 150, 150, Graphics::HCENTER | Graphics::VCENTER);
    g.drawImage(img1, 0, 0, Graphics::HCENTER | Graphics::VCENTER);
    g.drawImage(img2, 200, 150, Graphics::LEFT | Graphics::TOP);
    delete img2;
    delete img1;

    SDL_RenderPresent(renderer); // Send to screen

    // Event loop
    SDL_Event event;
    bool flag = true;
    while (flag) {
        while (SDL_WaitEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                flag = false;
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
