cmake_minimum_required(VERSION 3.5)

# Set the project name
project (gravity_defied_cpp C CXX)

if (WIN32)
    set(SDL2_DIR "C:/Users/rgimad/SDL2")
    set(SDL2_IMAGE_DIR "C:/Users/rgimad/SDL2_Image")
    set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/include;${SDL2_DIR}/include/SDL2")
    set(SDL2_IMAGE_INCLUDE_DIRS "${SDL2_IMAGE_DIR}/include")
    set(SDL2_LIBRARIES_DIR "${SDL2_DIR}/lib")
    set(SDL2_IMAGE_LIBRARIES_DIR "${SDL2_IMAGE_DIR}/lib")
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED >=2.0.0)
endif()

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

if (WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        -lmingw32
        ${SDL2_LIBRARIES_DIR}/libSDL2main.a
        ${SDL2_LIBRARIES_DIR}/libSDL2.dll.a
        ${SDL2_IMAGE_LIBRARIES_DIR}/libSDL2_image.dll.a
        -mwindows
    )
else()
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
endif()

