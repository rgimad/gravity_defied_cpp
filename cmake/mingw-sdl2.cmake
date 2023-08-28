include(FetchContent)

set(_sdl2_version "2.26.1")
set(_sdl2_ttf_version "2.20.1")
set(_sdl2_image_version "2.6.2")

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(_mingw_arch i686-w64-mingw32)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_mingw_arch x86_64-w64-mingw32)
else()
    message(FATAL_ERROR "Unsupported MinGW architecture!")
endif()

message("Downloading MinGW SDL ...")

FetchContent_Declare(mingw-sdl2
    URL "https://www.libsdl.org/release/SDL2-devel-${_sdl2_version}-mingw.zip"
)

FetchContent_Declare(mingw-sdl2_ttf
    URL "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-${_sdl2_ttf_version}-mingw.zip"
)

FetchContent_Declare(mingw-sdl2_image
    URL "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-${_sdl2_image_version}-mingw.zip"
)

FetchContent_MakeAvailable(mingw-sdl2 
                           mingw-sdl2_ttf
                           mingw-sdl2_image)

message("MinGW SDL download complete!")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${_mingw_arch})

execute_process(
    COMMAND make cross CROSS_PATH=${CMAKE_BINARY_DIR} ARCHITECTURES=${_mingw_arch}
    WORKING_DIRECTORY ${mingw-sdl2_SOURCE_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
    COMMAND make cross CROSS_PATH=${CMAKE_BINARY_DIR} ARCHITECTURES=${_mingw_arch}
    WORKING_DIRECTORY ${mingw-sdl2_ttf_SOURCE_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
    COMMAND make cross CROSS_PATH=${CMAKE_BINARY_DIR} ARCHITECTURES=${_mingw_arch}
    WORKING_DIRECTORY ${mingw-sdl2_image_SOURCE_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)

set(ENV{PKG_CONFIG_PATH} "${CMAKE_BINARY_DIR}/${_mingw_arch}/lib/pkgconfig")

message("MinGW SDL configuration completed!")
