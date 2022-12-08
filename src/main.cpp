#include <SDL2/SDL.h>
#include <stdexcept>
#include "lcdui/tests/test_Graphics.h"
#include "tests/test_GameCanvas.h"
#include "tests/test_rms.h"

int main (int argc, char ** args) {
    (void)argc; (void)args;
    try {
        // test_Graphics();
        // test_GameCanvas();
        test_rms();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
};