function(download_sdl2 sdl2_version sdl2_image_version sdl2_ttf_version)
    message("The following verions of SDL libs will be downloaded:")
    message("SDL2: ${sdl2_version}")
    message("SDL2_IMAGE: ${sdl2_image_version}")
    message("SDL2_TTF: ${sdl2_ttf_version}")
    message("Downloading SDL libs...")

    include(FetchContent)
    cmake_policy(SET CMP0135 NEW)

    FetchContent_Declare(sdl2
        URL "https://www.libsdl.org/release/SDL2-devel-${sdl2_version}-mingw.zip"
    )
    FetchContent_Declare(sdl2_image
        URL "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-${sdl2_image_version}-mingw.zip"
    )

    FetchContent_Declare(sdl2_ttf
        URL "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-${sdl2_ttf_version}-mingw.zip"
    )

    FetchContent_MakeAvailable(
        sdl2
        sdl2_image
        sdl2_ttf
    )

    set(SDL2_SOURCE_DIR ${sdl2_SOURCE_DIR} PARENT_SCOPE)
    set(SDL2_IMAGE_SOURCE_DIR ${sdl2_image_SOURCE_DIR} PARENT_SCOPE)
    set(SDL2_TTF_SOURCE_DIR ${sdl2_ttf_SOURCE_DIR} PARENT_SCOPE)

    message("Downloading SDL libs done")
endfunction(download_sdl2)