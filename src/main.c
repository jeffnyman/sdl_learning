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

    /*
    An "SDL event" is actually a structure. In fact, an SDL_Event is just a
    big collection of structs, like SDL_WindowEvent, SDL_KeyboardEvent, and
    so on.
    */
    SDL_Event event;

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

    printf("Running the game loop ...\n");

    bool running = true;

    // Values for the draw color.
    uint8_t r = 0, g = 0, b = 0;

    /*
    Any SDL-based program is going to need a loop. That loop will make sure
    that execution takes place in a persistent context.
    */
    while (running) {
        /*
        A delay such as the one below, from previous iterations of this code,
        is no longer needed.
        */
        // SDL_Delay(3000);

        /*
        Polling events means removing the next event from the event queue.
        If there are no events on the queue SDL_PollEvent returns 0 and if
        there are events it returns 1. This makes it easy for a loop to be
        used to process each event in turn. The SDL_PollEvent() function
        takes a pointer to an SDL_Event structure that is filled with event
        information. If SDL_PollEvent() removes an event from the queue
        then the event information will be placed in the event structure.
        The type of event will be placed in the type member of event. This
        makes it easy to handle each event type separately by using a switch
        statement or a series of if blocks.
        */
        while (SDL_PollEvent(&event) > 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        default:
                            r = (uint8_t)(rand() % 255);
                            g = (uint8_t)(rand() % 255);
                            b = (uint8_t)(rand() % 255);
                    }
            }
        }

        /*
        Whatever is rendered to the window will be a certain color. You can
        set the draw color of the renderer and this color will impact whatever
        is rendered next. Note that the last argument is called the "alpha
        value." A value of 255 refers to SDL_ALPHA_OPAQUE. A value of 0 would
        refer to SDL_ALPHA_TRANSPARENT.
        */
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        /*
        Now you can clear the current rendering target. This will "clear" it
        with whatever the current drawing color is.
        */
        SDL_RenderClear(renderer);

        /*
        An important thing to know about rendering is that just because
        something has been drawn to the window doesn't mean that whatever was
        drawn will be seen. It's required to update the window so that
        everything is shown. What this is doing is updating the window with
        any rendering that was performed since the previous call to
        SDL_RenderPresent().
        */
        SDL_RenderPresent(renderer);
    }

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
