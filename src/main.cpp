#include <string>
#include <stdexcept>
#include <iostream>

#include "Micro.h"

int main(int argc, char** argv)
{
    try {
        std::unique_ptr<Micro> micro = std::make_unique<Micro>();
        micro->startApp(argc, argv);
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};
