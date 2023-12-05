#ifndef TEMPERATURE_METER_ESP_H
#define TEMPERATURE_METER_ESP_H
#include <stdlib.h>
#include <string.h>
#include "retarget.h"
#include "uart_ringbuffer.h"
#include "debug.h"

void ESP_Receive();

void ESP_Send(char *string);

#endif //TEMPERATURE_METER_ESP_H
