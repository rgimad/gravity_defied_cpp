#include <memory>
#include <stdexcept>
#include <iostream>

#include "Micro.h"

int main(int argc, char** argv)
{
    try {
        std::unique_ptr<Micro> micro = std::make_unique<Micro>();

        if (argc == 2) {
            micro->setMrgFilePath(argv[1]);
        }

        micro->startApp();
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
};
