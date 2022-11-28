#include <SDL2/SDL.h>
#include <stdexcept>
#include "lcdui/tests/test_Graphics.h"
#include "lcdui/tests/test_GameCanvas.h"

int main (int argc, char ** args) {
    (void)argc; (void)args;
    try {
        // test_Graphics();
        test_GameCanvas();
    } catch (std::exception &e) {
        // std::cout << "Exception: " << e.what() << std::endl;
        SDL_LogError(0, e.what());
    }

    return 0;
};