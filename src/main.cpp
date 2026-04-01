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
    Log::write(Log::LogLevel::None, R"usage(
Usage %s [options]
      -f, --fullscreen         Enable fullscreen
      --fps                    Show FPS
      --width INT              Set screen width in px (default: 640)
      --height INT             Set screen height in px (default: 480)
      --zoom INT               Set render size in percents (default: 100)
      -h, --help               Show this help screen
      -v, --verbose INT        Set log level [0-6] (default: 2)
      --font-mult              Set font multiplier (default: 4)
      --logo-mult              Set logo multiplier (default: 1)
)usage", progName);
}

bool parseArguments(int argc, char** argv)
{
    int opt;
    static struct option long_options[] = {
        { "fullscreen", no_argument, nullptr, 'f' },
        { "verbose", optional_argument, nullptr, 'v' },
        { "help", no_argument, nullptr, 'h' },
        { "width", required_argument, nullptr, 0 },
        { "height", required_argument, nullptr, 0 },
        { "zoom", required_argument, nullptr, 0 },
        { "fps", no_argument, nullptr, 0 },
        { "font-mult", required_argument, nullptr, 0 },
        { "logo-mult", required_argument, nullptr, 0 },
        { nullptr, 0, nullptr, 0 }
    };

    int option_index = 0;
    Log::logLevel = Log::LogLevel::Info;

    while ((opt = getopt_long(argc, argv, "fhv:", long_options, &option_index)) != -1) {
        switch (opt) {
        case 0: {
            if (long_options[option_index].flag != 0)
                break;

            Log::write(Log::LogLevel::Debug, "Got parameter %d %s %s\n", option_index, long_options[option_index].name, optarg);

            if (strncmp(long_options[option_index].name, "fps", 3) == 0) {
                GlobalSetting::ShowFPS = true;
                Log::write(Log::LogLevel::None, "Showing FPS\n");
            } else if (strncmp(long_options[option_index].name, "width", 5) == 0) {
                GlobalSetting::DefaultScreenWidth = atoi(optarg);
                Log::write(Log::LogLevel::None, "Setting width %d\n", GlobalSetting::DefaultScreenWidth);
            } else if (strncmp(long_options[option_index].name, "height", 6) == 0) {
                GlobalSetting::DefaultScreenHeight = atoi(optarg);
                Log::write(Log::LogLevel::None, "Setting height %d\n", GlobalSetting::DefaultScreenHeight);
            } else if (strncmp(long_options[option_index].name, "zoom", 4) == 0) {
                GlobalSetting::DefaultZoomLevel = atoi(optarg);
                Log::write(Log::LogLevel::None, "Setting render size %d%%\n", GlobalSetting::DefaultZoomLevel);
            } else if (strncmp(long_options[option_index].name, "font-mult", 9) == 0) {
                GlobalSetting::FontMultiplier = atoi(optarg);
                Log::write(Log::LogLevel::None, "Setting font multiplier to %d\n", GlobalSetting::FontMultiplier);
            } else if (strncmp(long_options[option_index].name, "logo-mult", 9) == 0) {
                const int value = atoi(optarg);
                GlobalSetting::LogoMultiplier = value;
                GlobalSetting::SplashMultiplier = value;
                Log::write(Log::LogLevel::None, "Setting logo multiplier to %d\n", GlobalSetting::LogoMultiplier);
            }

            break;
        }
        case 'f': {
            Log::write(Log::LogLevel::None, "Setting fullscreen\n");
            GlobalSetting::WindowFullscreen = true;
            break;
        }
        case 'v': {
            const uint logLevelFromArg = 6 - atoi(optarg);

            if (logLevelFromArg > static_cast<uint>(Log::LogLevel::None)) {
                Log::logLevel = Log::LogLevel::All;
            } else {
                Log::logLevel = static_cast<Log::LogLevel>(logLevelFromArg);
            }

            Log::write(Log::LogLevel::None, "Setting logs verbosity to %u\n", Log::logLevel);
            break;
        }
        case 'h':
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