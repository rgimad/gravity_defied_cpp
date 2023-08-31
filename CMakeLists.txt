cmake_minimum_required(VERSION 3.24)
project(gravity_defied_cpp C CXX)

unset(ENV{PKG_CONFIG_PATH})

if(WIN32)
    include(cmake/mingw-sdl2.cmake)
endif()

find_package(PkgConfig REQUIRED)

pkg_search_module(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_search_module(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)
pkg_search_module(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)

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

target_include_directories(GravityDefied PRIVATE
                           ${SDL2_INCLUDE_DIRS}
                           ${SDL2_TTF_INCLUDE_DIRS}
                           ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_directories(GravityDefied PRIVATE
                        ${SDL2_LIBRARY_DIRS}
                        ${SDL2_TTF_LIBRARY_DIRS}
                        ${SDL2_IMAGE_LIBRARY_DIRS})

set_target_properties(GravityDefied PROPERTIES LINK_FLAGS_RELEASE -s)

if(WIN32)
    target_link_options(GravityDefied PRIVATE -static -mwindows)
    target_link_libraries(GravityDefied 
                         ${SDL2_STATIC_LIBRARIES} 
                         ${SDL2_TTF_STATIC_LIBRARIES}
                         ${SDL2_IMAGE_STATIC_LIBRARIES} 
                         Assets::Assets)
else()
    target_link_libraries(GravityDefied 
                          ${SDL2_LIBRARIES}
                          ${SDL2_TTF_LIBRARIES}
                          ${SDL2_IMAGE_LIBRARIES}
                          Assets::Assets)
endif()

# Copy assets to the build directory
file(COPY assets/levels.mrg DESTINATION ${CMAKE_BINARY_DIR})
