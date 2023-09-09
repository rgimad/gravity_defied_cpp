#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

#include "Micro.h"

int main(int argc, char** argv)
{
    try {
        std::unique_ptr<Micro> micro = std::make_unique<Micro>();

        if (argc > 1) {
            std::string argv1(argv[1]); 
            if (argv1 == "-h" || argv1 == "--help") {
                micro->showHelp(argv[0]);
                return EXIT_SUCCESS;
            }
            micro->setMrgFilePath(argv1);
        }

        micro->startApp();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};
