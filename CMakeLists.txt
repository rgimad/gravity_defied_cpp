cmake_minimum_required(VERSION 3.24)
# Set the project name
project(gravity_defied_cpp C CXX)

if(WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/download_sdl2.cmake)

    download_sdl2(
        "2.26.1" # sdl2
        "2.6.2"  # sdl2_image
        "2.20.1" # sdl2_ttf
    )

    include(${SDL2_SOURCE_DIR}/cmake/sdl2-config.cmake)
    include(${SDL2_IMAGE_SOURCE_DIR}/cmake/sdl2_image-config.cmake)
    include(${SDL2_TTF_SOURCE_DIR}/cmake/sdl2_ttf-config.cmake)

else()
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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

if(WIN32)
    enable_language(RC)
    list(APPEND SOURCES winres/res.rc)
endif()

set(HEADERS
    src/utils/Stream.h
    src/utils/Time.h
    src/utils/String.h
    src/Displayable.h
    src/MathF16.h
    src/GameCanvas.h
    src/GamePhysics.h
    src/TimerOrMotoPartOrMenuElem.h
    src/class_10.h
    src/GameLevel.h
    src/LevelLoader.h
    src/Micro.h
    src/TextRender.h
    src/GameMenu.h
    src/SettingsStringRender.h
    src/MenuManager.h
    src/RecordManager.h
    src/Timer.h
    src/IGameMenuElement.h
    src/lcdui/CanvasImpl.h
    src/lcdui/Canvas.h
    src/lcdui/Graphics.h
    src/lcdui/Image.h
    src/lcdui/Font.h
    src/lcdui/Command.h
    src/lcdui/CommandListener.h
    src/rms/InvalidRecordIDException.h
    src/rms/RecordComparator.h
    src/rms/RecordEnumeration.h
    src/rms/RecordEnumerationImpl.h
    src/rms/RecordFilter.h
    src/rms/RecordStore.h
    src/rms/RecordStoreException.h
    src/rms/RecordStoreNotOpenException.h
    src/tests/test_Graphics.h
    src/tests/test_rms.h
)

# Add an executable with the above sources
add_executable(GravityDefied ${SOURCES})

set_target_properties(GravityDefied PROPERTIES LINK_FLAGS_RELEASE -s)
target_compile_options(GravityDefied PRIVATE -Wall -Wextra -std=c++17)# -g)

if(WIN32)
    get_target_property(SDL2_INCLUDE_DIR SDL2::SDL2-static INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(SDL2_IMAGE_INCLUDE_DIR SDL2_image::SDL2_image-static INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(SDL2_TTF_INCLUDE_DIR SDL2_ttf::SDL2_ttf-static INTERFACE_INCLUDE_DIRECTORIES)

    target_include_directories(GravityDefied PRIVATE 
                                ${SDL2_INCLUDE_DIR}/.. 
                                ${SDL2_IMAGE_INCLUDE_DIR}/.. 
                                ${SDL2_TTF_INCLUDE_DIR}/..)

    target_link_options(GravityDefied PRIVATE -static -mwindows)
    target_link_libraries(GravityDefied SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static)

else()
    target_link_libraries(GravityDefied ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif()

# Copy the font to the build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
