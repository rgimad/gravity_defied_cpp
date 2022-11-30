#include <SDL2/SDL.h>
#include <stdexcept>
#include "lcdui/tests/test_Graphics.h"
#include "tests/test_GameCanvas.h"

int main (int argc, char ** args) {
    (void)argc; (void)args;
    try {
        // test_Graphics();
        test_GameCanvas();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
};