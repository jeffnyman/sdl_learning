#include "common.h"

// Prototypes
int initialize_display(void);
void setup(void);
void cleanup(void);
void process_input(void);
void update_display(void);
void render_display(void);

/*
The definitions set up the frames per second and then a frame time that is
based on those frames. The frame time is what it sounds like: it's literally
how much time a given frame is going to take to process. So if you want 30
frames per second then that means in that second, each frame will have to
take 33.333 milliseconds.
*/
#define FPS 30
#define FRAME_TIME (1000 / FPS)

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

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
It can be useful to create a structure to hold data that will require
more fields than you might get otherwise. The ball object in this
example is going to a type of SDL_Rect and that provides properties
for x, y, w (width), and h (height). But there may also be a desire
to have other properties, such as velocity.
*/
struct game_object {
    float x;
    float y;
    float w;
    float h;
    float vx;
    float vy;
} ball, paddle;

bool running = false;

// Values for the draw color.
uint8_t r = 0, g = 0, b = 0;

// Milliseconds since the last frame that was called.
uint32_t last_frame_time = 0;

int main( int argc, char* argv[] ) {
    log_debug("argc = %d\n", argc);

    for (int i = 0; i < argc; ++i) {
        log_debug("argv[%d] = %s\n", i, argv[i]);
    }

    printf("\nSDL Example Running ...\n");

    running = initialize_display();

    setup();

    /*
    Any SDL-based program is going to need a loop. That loop will make sure
    that execution takes place in a persistent context.
    */
    printf("Running the game loop ...\n");

    while (running) {
        process_input();
        update_display();
        render_display();
    }

    cleanup();

    return EXIT_SUCCESS;
}

void update_display(void) {
    /*
    This provides a delta time factor. The goal is to focus on how many
    pixels per second as opposed to how many pixels per frame. This is
    done by taking the difference in ticks from the last frame to the
    current one, converted to seconds. This makes the delta time the
    amount of time elapsed since last frame.
    */
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    /*
    SDL measures time in "ticks", which is the number of milliseconds that
    have elapsed since the SDL library initialized. The SDL_GetTicks()
    function provides an usigned 32-bit value representing this measure
    of time.
    */
    last_frame_time = SDL_GetTicks();

    /*
    NOTE: This while loop is not going to be used because it can be
    very expensive for the CPU. The solution following it is what
    should be done.

    Churn a bit (i.e., sleep) until the frame target time has been
    reached. In other words, the logic "sleeps" until it reaches the
    amount of time that each frame will take. The SDL_TICKS_PASSED()
    essentially compares two time durations, in milliseconds, and
    passes when the time duration has passed. So this is saying to
    compare the current time with the time of the last frame plus
    the stated frame time. So basically this is saying "sleep until
    the current frame duration has passed."
    */
    // while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TIME));

    /*
    The time_to_wait variable is storing, frame by frame, how many
    milliseconds to wait. This is based on the FRAME_TIME, which is
    how long each frame should take, minus the difference between
    the ticks now (for the current frame) minus the milliseconds of
    the previous frame. This is variable is basically storing how
    many milliseconds to "churn" or "sleep" until execution should
    resume once again.
    */
    uint32_t time_to_wait = FRAME_TIME - (SDL_GetTicks() - last_frame_time);

    /*
    Here the standard SDL delay is called but only if things are
    moving too fast to process the current frame.
    */
    if (time_to_wait > 0 && time_to_wait <= FRAME_TIME) {
        SDL_Delay(time_to_wait);
    }

    /*
    After polling for events but before rendering anything, the loop
    should update the display. The modifier of delta time means that
    game objects will move correctly regardless of frame rate.
    */
    ball.x += (int)(ball.vx * delta_time);
    ball.y += (int)(ball.vy * delta_time);
}

void render_display(void) {
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
    You can now set the renderer color for the rectangular object, which
    will act as a ball. This new color will now apply when the rectangle
    is drawn.
    */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    /*
    Rendering a shape on the window that can move requires the shape being
    established before the loop. SDL_Rect is a structure that contains the
    definition of a rectangle, with the origin at the upper left. Here the
    SDL_Rect will have the values of the ball structure defined earlier.
    */
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.w,
        (int)ball.h,
    };

    /*
    The rectangle can now be drawn to the screen, in the appropriate
    color. Notice that if you did not want the rectangle filled in,
    you could just do this:
    SDL_RenderDrawRect(renderer, &rect);
    */
    SDL_RenderFillRect(renderer, &ball_rect);

    /*
    You can now render a second object, which will correspond to the paddle.
    As with the ball, this new object has to be rendered and then filled.
    */
    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.w,
        (int)paddle.h,
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

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

void process_input(void) {
    /*
    An "SDL event" is actually a structure. In fact, an SDL_Event is just a
    big collection of structs, like SDL_WindowEvent, SDL_KeyboardEvent, and
    so on.
    */
    SDL_Event event;

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
}

int initialize_display(void) {
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

    return true;
}

void setup(void) {
    ball.x = 20;
    ball.y = 20;
    ball.w = 15;
    ball.h = 15;
    ball.vx = 200;
    ball.vy = 200;

    paddle.w = 100;
    paddle.h = 20;
    paddle.vx = 0;
    paddle.vy = 0;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.w / 2);
    paddle.y = WINDOW_HEIGHT - 40;
}

void cleanup(void) {
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
}
