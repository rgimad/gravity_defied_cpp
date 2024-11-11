#include <string>
#include <stdexcept>
#include <iostream>
#include <getopt.h>

#include "utils/Hashing.h"
#include "utils/Log.h"
#include "config.h"
#include "Micro.h"

void showHelp(const char* progName)
{
    Log::write(Log::LogLevel::None, "Usage %s [options]\n", progName);
    Log::write(Log::LogLevel::None, "      -f, --fullscreen         Enable fullscreen\n", progName);
    Log::write(Log::LogLevel::None, "      -w, --width INT          Set screen width in px\n", progName);
    Log::write(Log::LogLevel::None, "      -h, --height INT         Set screen height in px\n", progName);
    Log::write(Log::LogLevel::None, "      -?, --help               Show this help screen\n", progName);
    Log::write(Log::LogLevel::None, "      -v, --verbose INT        Set log level (0-6)\n", progName);
}

bool parseArguments(int argc, char** argv)
{
    int opt;
    static struct option long_options[] = {
        { "fullscreen", no_argument, nullptr, 'f' },
        { "width", optional_argument, nullptr, 'w' },
        { "height", optional_argument, nullptr, 'h' },
        { "verbose", optional_argument, nullptr, 'v' },
        { "help", no_argument, nullptr, '?' },
        { nullptr, 0, nullptr, 0 }
    };

    Log::logLevel = Log::LogLevel::All;

    while ((opt = getopt_long(argc, argv, "fhw:v:?", long_options, nullptr)) != -1) {
        switch (opt) {
        case 'f': {
            Log::write(Log::LogLevel::None, "Setting fullscreen\n");
            GlobalSetting::WindowFullscreen = true;
            break;
        }
        case 'w': {
            GlobalSetting::DefaultScreenWidth = atoi(optarg);
            Log::write(Log::LogLevel::None, "Setting width %d\n", GlobalSetting::DefaultScreenWidth);
            break;
        }
        case 'h': {
            GlobalSetting::DefaultScreenHeight = atoi(optarg);
            Log::write(Log::LogLevel::None, "Setting height %d\n", GlobalSetting::DefaultScreenHeight);
            break;
        }
        case 'v': {
            uint logLevelFromArg = 6 - atoi(optarg);

            if (logLevelFromArg > static_cast<uint>(Log::LogLevel::None)) {
                Log::logLevel = Log::LogLevel::All;
            } else {
                Log::logLevel = static_cast<Log::LogLevel>(logLevelFromArg);
            }

            Log::write(Log::LogLevel::None, "Setting logs verbosity to %u\n", Log::logLevel);
            break;
        }
        case '?':
        default: {
            showHelp(argv[0]);
            return false;
        }
        }
    }

    // Handle any remaining arguments
    if (optind < argc) {
        std::stringstream ss("Remaining arguments: ");

        for (int i = optind; i < argc; i++) {
            ss << argv[i] << " ";
        }

        Log::write(Log::LogLevel::Error, ss.str().c_str());
    }

    Log::write(Log::LogLevel::Info, "Path: %s\n", GlobalSetting::MrgFilePath.c_str());
    GlobalSetting::SavesPrefix = Hashing::HashFileMD5(GlobalSetting::MrgFilePath.string());
    Log::write(Log::LogLevel::Info, "MRG file hash: %s\n", GlobalSetting::SavesPrefix.c_str());
    return true;
}

int main(int argc, char** argv)
{
    if (!parseArguments(argc, argv)) {
        return EXIT_FAILURE;
    }

    try {
        std::unique_ptr<Micro> micro = std::make_unique<Micro>();
        micro->startApp();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};