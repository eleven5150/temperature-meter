#include "led_controller.h"

static LedData_t TEMPERATURE_SCALE[PHYSICAL_NUM_OF_LEDS] = {
        {0, 0, 20},
        {0, 0, 18},
        {0, 0, 16},
        {0, 0, 14},
        {0, 0, 12},
        {0, 1, 10},
        {0, 2, 9},
        {0, 3, 8},
        {0, 4, 7},
        {0, 5, 6},
        {0, 6, 5},
        {0, 7, 4},
        {0, 8, 3},
        {0, 9, 2},
        {0, 10, 1},
        {0, 12, 0},
        {0, 14, 0},
        {0, 16, 0},
        {0, 18, 0},
        {0, 20, 0},
};


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
    }
}

// private -------------------------------------------------------------------------------------------------------------

static void LedController_SetLedData(uint8_t led_num, LedData_t *led_data) {
    memcpy(&LEDS_DATA[led_num], led_data, sizeof(LedData_t));
}

static void LedController_ClearLedData() {
    memset(LEDS_DATA, 0, sizeof(LedData_t) * NUMBER_OF_LEDS);
}

static void LedController_SendDataToStrip(void) {
    uint32_t bit_number = 0;
    uint32_t color;

    uint16_t *leds_data_encoded = (uint16_t *) calloc(
            (WS2812_DATA_BIT_WIDTH * NUMBER_OF_LEDS) + RESERVE_TAIL_BITS,
            sizeof(uint16_t)
    );

    for (uint16_t i = 0; i < NUMBER_OF_LEDS; i++) {
        color = (
                (LEDS_DATA[i].green_intense << SHIFT_FOR_GEEN_LED) |
                (LEDS_DATA[i].red_intense << SHIFT_FOR_RED_LED) |
                (LEDS_DATA[i].blue_intense)
        );

        for (int8_t j = WS2812_DATA_BIT_WIDTH - 1; j >= 0; j--) {
            if (color & (1 << j)) {
                leds_data_encoded[bit_number] = PWM_ENCODED_1;
            } else {
                leds_data_encoded[bit_number] = PWM_ENCODED_0;
            }
            bit_number++;
        }
    }

    HAL_TIM_PWM_Start_DMA(
            &htim2,
            TIM_CHANNEL_1,
            (uint32_t *) leds_data_encoded,
            bit_number + RESERVE_TAIL_BITS
    );
    HAL_Delay(TIME_FOR_SENDING_DATA_TO_STRIP);
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
    free(leds_data_encoded);
}

// public --------------------------------------------------------------------------------------------------------------

void LedController_OnLed(int led_number, LedData_t *led_data) {
    if (led_number <= NUMBER_OF_LEDS) {
        LedController_SetLedData(led_number, led_data);
    }

    LedController_SendDataToStrip();
}

void LedController_OffLed(int led_number) {
    LedData_t off_led = {0};
    if (led_number <= NUMBER_OF_LEDS) {
        LedController_SetLedData(led_number, &off_led);
    }

    LedController_SendDataToStrip();
}

void LedController_LoadingEffect(void) {
    LedData_t blue_led = {
            ZERO_COLOUR_INTENSITY,
            ZERO_COLOUR_INTENSITY,
            AVERAGE_COLOR_INTENSITY
    };
    LedController_OnLed(0, &blue_led);
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        LedController_OnLed(i, &blue_led);
        LedController_OnLed(i + 1, &blue_led);
        LedController_OffLed(i - 1);
    }
}

void LedController_OffAllLeds(void) {
    LedController_ClearLedData();
    LedController_SendDataToStrip();
}

void LedController_OnLedsFromStart(uint8_t num_of_leds) {
    LedController_ClearLedData();

    for (uint8_t i = 0; i < num_of_leds; i++) {
        LedController_SetLedData(i, &TEMPERATURE_SCALE[i]);
    }
    LedController_SendDataToStrip();
}
