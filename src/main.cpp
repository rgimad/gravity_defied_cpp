#include <SDL2/SDL.h>

// Example code

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main (int argc, char ** args) {
    
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        return 1;
    }

    SDL_Surface* screen_surface = NULL;

    SDL_Window* window = NULL;

    window = SDL_CreateWindow("Hello, SDL 2!",SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
    SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        return 1;
    }

    screen_surface = SDL_GetWindowSurface(window);

    SDL_FillRect(screen_surface, NULL, SDL_MapRGB( screen_surface->format, 0, 255, 0));

    SDL_UpdateWindowSurface(window);

    SDL_Delay(2000);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
};