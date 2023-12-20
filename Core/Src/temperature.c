#include "temperature.h"

// multiplied by 10
static uint16_t TEMP_VALUE = 0;
static uint8_t TEMP_MEASURE_COUNT = 0;
static uint16_t TEMP_VALUES_BUFF[TEMP_MEASURE_BUFF_SIZE] = {0};

void Temperature_UpdateBuffValue() {
    TEMP_VALUES_BUFF[TEMP_MEASURE_COUNT] = (uint16_t) (ADC_GetValue() * VOLTAGE * TEMPERATURE_MULTIPLIER / ADC_CAPACITY);
    TEMP_VALUES_BUFF[TEMP_MEASURE_COUNT] += LED_STRIP_CORRECTION;
    TEMP_MEASURE_COUNT++;
    if (TEMP_MEASURE_COUNT == TEMP_MEASURE_BUFF_SIZE) {
        Temperature_UpdateValue();
        TEMP_MEASURE_COUNT = 0;
    }
}

void Temperature_UpdateValue() {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < TEMP_MEASURE_BUFF_SIZE; i++) {
        sum += TEMP_VALUES_BUFF[i];
    }
    TEMP_VALUE = sum / TEMP_MEASURE_BUFF_SIZE;
}

uint16_t Temperature_GetFullValue() {
    return TEMP_VALUE;
}

uint8_t Temperature_GetIntValue() {
    return TEMP_VALUE / 10;
}

uint8_t Temperature_GetDecValue() {
    return TEMP_VALUE % 10;
}
