#include "CanvasImpl.h"

CanvasImpl::CanvasImpl(Canvas* canvas) : canvas(canvas)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error(IMG_GetError());
    }

    if (TTF_Init() == -1) {
        throw std::runtime_error(TTF_GetError());
    }

    uint32_t windowFlags = SDL_WINDOW_SHOWN;

    if (GlobalSetting::WindowFullscreen) {
        windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    window = SDL_CreateWindow(
        0,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        GlobalSetting::DefaultScreenWidth, GlobalSetting::DefaultScreenHeight,
        windowFlags);

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    zoomLevel = GlobalSetting::DefaultZoomLevel > 0 ? GlobalSetting::DefaultZoomLevel : 100;

    if(SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight) < 0) {
        windowWidth=GlobalSetting::DefaultScreenWidth;
        windowHeight=GlobalSetting::DefaultScreenHeight;
    }
    windowWidth=(windowWidth * zoomLevel) / 100;
    windowHeight=(windowHeight * zoomLevel) / 100;

    texTarget = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window),
        SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, texTarget);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

CanvasImpl::~CanvasImpl()
{
    SDL_DestroyTexture(texTarget);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void CanvasImpl::repaint()
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopyEx(renderer, texTarget, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, texTarget);
}

int CanvasImpl::getZoom()
{
    if (zoomLevel <= 0) {
        return GlobalSetting::DefaultZoomLevel;
    }

    return zoomLevel;
}

int CanvasImpl::getWidth()
{
    if (windowWidth < 0) {
        return GlobalSetting::DefaultScreenWidth;
    }

    return windowWidth;
}

int CanvasImpl::getHeight()
{
    if (windowHeight < 0) {
        return GlobalSetting::DefaultScreenHeight;
    }

    return windowHeight;
}

SDL_Renderer* CanvasImpl::getRenderer()
{
    return renderer;
}

void CanvasImpl::processEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            exit(0); // IMPROVE This is a super dumb way to finish the game, but it works
            break;
        case SDL_KEYDOWN: {
            const Keys keyCode = convertKeyCharToKeyCode(e.key.keysym.sym);

            if (keyCode != Keys::NONE) {
                canvas->publicKeyPressed(keyCode);
            }
        } break;
        case SDL_KEYUP: {
            const Keys keyCode = convertKeyCharToKeyCode(e.key.keysym.sym);

            if (keyCode != Keys::NONE) {
                canvas->publicKeyReleased(keyCode);
            }
        } break;
        default:
            break;
        }
    }
}

Keys CanvasImpl::convertKeyCharToKeyCode(const SDL_Keycode keyCode)
{
    Log::write(Log::LogLevel::Debug, "KEY %d; Menu: %d\n", keyCode, Micro::isInGameMenu);

    if (Micro::isInGameMenu && menuKeyMappings.count(keyCode) > 0) {
        return menuKeyMappings[keyCode];
    }

    if (!Micro::isInGameMenu && gameKeyMappings.count(keyCode) > 0) {
        return gameKeyMappings[keyCode];
    }

    Log::write(Log::LogLevel::Warning, "unknown keyEvent: %d\n", keyCode);
    return Keys::NONE;
}

void CanvasImpl::setWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}