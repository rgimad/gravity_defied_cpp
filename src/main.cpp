#include <memory>
#include <stdexcept>
#include <iostream>

#include "Micro.h"

int main(int argc, char** args)
{
    (void)argc;
    (void)args;
    try {
        std::unique_ptr<Micro> micro = std::make_unique<Micro>();
        micro->startApp();
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
};
