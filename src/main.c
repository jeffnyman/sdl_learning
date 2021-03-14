#include "common.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main( int argc, char* argv[] ) {
    log_debug("argc = %d\n", argc);

    for (int i = 0; i < argc; ++i) {
        log_debug("argv[%d] = %s\n", i, argv[i]);
    }

    /*
    You need to create an object that can be rendered to and then create a
    renderer. Note that you will also see examples that use SDL_Surface.
    SDL_Renderer is hardware accelerated on most systems and falls back to
    software rendering when hardware rendering isn't supported. SDL_Surface
    is always software rendered. Another way to frame this is that surfaces
    are basically image data that is operated on by the CPU and stored in
    normal system RAM. The renderer focuses more on textures and these are
    image data that the GPU can work with and are stored in GPU RAM.
    */
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    printf("\nSDL Example Running ...\n");

    printf("Initializing SDL ...\n");

    /*
    You can't call any SDL functions without initializing SDL first. If all
    you care about is using SDL's video subsystem, you can just pass the
    SDL_INIT_VIDEO flag. If you are planning to use everything that SDL
    provides, you can use SDL_INIT_EVERYTHING. If there's an error during
    initializing, SDL_Init() returns -1.
    */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    printf("Creating app window ...\n");

    /*
    The function to create an SDL window will return the window that was
    created or NULL if there was a failure doing so. The flag called
    SDL_WINDOW_SHOWN is actually ignored by the SDL_CreateWindow() call.
    Any SDL_Window is implicitly shown unless the SDL_WINDOW_HIDDEN flag
    is set.
    */
    window = SDL_CreateWindow(
        "SDL Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if ( window == NULL ) {
        fprintf(stderr, "Window could not be created. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    printf("Creating app renderer ...\n");

    /*
    The renderer provides a 2D context in which graphics can be displayed.
    The function returns a valid rendering context or NULL if there was an
    error doing so.
    */
    renderer = SDL_CreateRenderer(window, -1, 0);

    if ( renderer == NULL ) {
        fprintf(stderr, "Renderer could not be created. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    printf("All looks good...\n");

    /*
    Any SDL-based program is going to need a loop. That loop will make sure
    that execution takes place in a persistent context. Until there is such
    a loop, a delay, measured in millisecond, has to be introduced.
    */
    SDL_Delay(3000);

    printf("Shutting down SDL ...\n");

    /*
    It's generally a good idea to clean up everything that was created,
    destroying everything in the reverse order that it was instantiated.
    Then you can have SDL quit such that it cleans up any of the systems
    that it initialized.
    */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
