#include "temperature.h"

// multiplied by 10
static uint16_t TEMP_VALUE = 0;

void Temperature_UpdateValue() {
    TEMP_VALUE = (uint16_t)(ADC_GetValue()*VOLTAGE*TEMPERATURE_MULTIPLIER/ADC_CAPACITY);
}

uint16_t Temperature_GetFullValue() {
    return TEMP_VALUE;
}

uint8_t Temperature_GetIntValue() {
    return TEMP_VALUE/10;
}

uint8_t Temperature_GetDecValue() {
    return TEMP_VALUE % 10;
}
