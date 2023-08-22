cmake_minimum_required(VERSION 3.24)
project(gravity_defied_cpp C CXX)

if(WIN32)
    include(cmake/download_sdl2.cmake)

    download_sdl2(
        "2.26.1" # sdl2
        "2.6.2"  # sdl2_image
        "2.20.1" # sdl2_ttf
    )

    include(${SDL2_SOURCE_DIR}/cmake/sdl2-config.cmake)
    include(${SDL2_IMAGE_SOURCE_DIR}/cmake/sdl2_image-config.cmake)
    include(${SDL2_TTF_SOURCE_DIR}/cmake/sdl2_ttf-config.cmake)
else()
    list(APPEND CMAKE_MODULE_PATH cmake)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED >=2.0.0)
endif()

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/utils/Time.cpp
    src/main.cpp
    src/MathF16.cpp
    src/GameCanvas.cpp
    src/GamePhysics.cpp
    src/TimerOrMotoPartOrMenuElem.cpp
    src/class_10.cpp
    src/GameLevel.cpp
    src/LevelLoader.cpp
    src/Micro.cpp
    src/TextRender.cpp
    src/GameMenu.cpp
    src/SettingsStringRender.cpp
    src/MenuManager.cpp
    src/RecordManager.cpp
    src/Timer.cpp
    src/lcdui/CanvasImpl.cpp
    src/lcdui/Canvas.cpp
    src/lcdui/Graphics.cpp
    src/lcdui/Image.cpp
    src/lcdui/Font.cpp
    src/lcdui/Command.cpp
    src/rms/RecordEnumerationImpl.cpp
    src/rms/RecordStore.cpp
    src/tests/test_Graphics.cpp
    src/tests/test_rms.cpp
)

include(cmake/CMakeRC.cmake)

cmrc_add_resource_library(Assets
    NAMESPACE assets
    ALIAS Assets::Assets
    assets/bluearm.png
    assets/helmet.png
    assets/sprites.png
    assets/blueleg.png
    assets/bluebody.png
    assets/logo.png
    assets/fender.png
    assets/levels.mrg
    assets/engine.png
    assets/splash.png
    assets/raster.png
    assets/FontSansSerif.ttf
)

# Compile resources when building for Windows.
if(WIN32)
    enable_language(RC)
    list(APPEND SOURCES winres/res.rc)
endif()

add_executable(GravityDefied ${SOURCES})

target_compile_features(GravityDefied PRIVATE cxx_std_17)
target_compile_options(GravityDefied PRIVATE -Wall -Wextra)
set_target_properties(GravityDefied PROPERTIES LINK_FLAGS_RELEASE -s)

if(WIN32)
    get_target_property(SDL2_INCLUDE_DIR SDL2::SDL2-static INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(SDL2_IMAGE_INCLUDE_DIR SDL2_image::SDL2_image-static INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(SDL2_TTF_INCLUDE_DIR SDL2_ttf::SDL2_ttf-static INTERFACE_INCLUDE_DIRECTORIES)

    target_include_directories(GravityDefied PRIVATE 
                                ${SDL2_INCLUDE_DIR}/.. 
                                ${SDL2_IMAGE_INCLUDE_DIR}/.. 
                                ${SDL2_TTF_INCLUDE_DIR}/..)

    target_link_options(GravityDefied PRIVATE -static -mwindows)
    target_link_libraries(GravityDefied SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static Assets::Assets)
else()
    target_link_libraries(GravityDefied ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} Assets::Assets)
endif()

# Copy assets to the build directory
file(COPY assets/levels.mrg DESTINATION ${CMAKE_BINARY_DIR})
