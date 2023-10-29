#ifndef TEMPERATURE_METER_TEMPERATURE_H
#define TEMPERATURE_METER_TEMPERATURE_H

#include <stdint.h>
#include "adc.h"
#include "led_controller.h"

#define VOLTAGE 3.3
#define TEMPERATURE_MULTIPLIER 1000

#define LED_STRIP_CORRECTION 41

uint16_t Temperature_GetFullValue();

void Temperature_UpdateValue();

uint8_t Temperature_GetIntValue();

uint8_t Temperature_GetDecValue();

#endif //TEMPERATURE_METER_TEMPERATURE_H
