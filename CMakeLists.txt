cmake_minimum_required(VERSION 3.5)

# Set the project name
project (gravity_defied_cpp C CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED >=2.0.0)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/main.cpp
    src/lcdui/Graphics.cpp
    src/lcdui/Image.cpp
    src/lcdui/tests/test_Graphics.cpp
)

set(HEADERS
    src/lcdui/Graphics.h
    src/lcdui/Image.h
    src/lcdui/tests/test_Graphics.h
)

# Add an executable with the above sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Set the directories that should be included in the build command for this target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

