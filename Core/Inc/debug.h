#ifndef TEMPERATURE_METER_DEBUG_H
#define TEMPERATURE_METER_DEBUG_H

#include <stdio.h>
#include <stdarg.h>

#define DEBUG_LEVEL DEBUG_PRINT_TRACE

typedef enum DebugLevel {
    DEBUG_PRINT_NONE,
    DEBUG_PRINT_INFO,
    DEBUG_PRINT_TRACE
} DebugLevel_t;

#define DEVICE_CORE "CORE"
#define DEVICE_ESP "ESP"

void debug(DebugLevel_t level, const char *device, const char *format, ...);

#endif //TEMPERATURE_METER_DEBUG_H
