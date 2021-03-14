#include "common.h"
#include "logger.h"

int main( int argc, char* argv[] ) {
    log_debug("argc = %d\n", argc);

    for (int i = 0; i < argc; ++i) {
        log_debug("argv[%d] = %s\n", i, argv[i]);
    }

    return EXIT_SUCCESS;
}
