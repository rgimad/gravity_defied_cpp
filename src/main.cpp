#include <SDL2/SDL.h>
#include <stdexcept>
#include "lcdui/tests/test_Graphics.h"


int main (int argc, char ** args) {
    try {
        test_Graphics();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
};