#include "debug.h"

void debug(DebugLevel_t level, const char *device, const char *format, ...) {
    if (DEBUG_LEVEL >= level){
        printf("[%s] ", device);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\r\n");
    }
}
