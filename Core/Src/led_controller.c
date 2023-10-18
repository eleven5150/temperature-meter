#include "led_controller.h"

uint16_t pwmData[(WS2812_DATA_BIT_WIDTH * NUMBER_OF_LEDS) + RESERVE_TAIL_NUMBER_OF_LEDS] = {0};

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
//    if (htim->Instance == TIM2) {
        HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
//    }
}

// private -------------------------------------------------------------------------------------------------------------

static void LedController_SetLedData(uint8_t led_num, LedData_t *led_data) {
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> start", __FUNCTION__);
    memcpy(&LEDS_DATA[led_num], led_data, sizeof(LedData_t));
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> end", __FUNCTION__);
}

static void LedController_SendDataToStrip(void) {
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> start", __FUNCTION__);
    uint32_t indx = 0;
    uint32_t color;

    for (uint16_t i = 0; i < NUMBER_OF_LEDS; i++) {
        debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> i -> %u", __FUNCTION__, i);
        color = (
                (LEDS_DATA[i].green_intense << SHIFT_FOR_GEEN_LED) |
                (LEDS_DATA[i].red_intense << SHIFT_FOR_RED_LED) |
                (LEDS_DATA[i].blue_intense)
                );

        for (uint8_t j = WS2812_DATA_BIT_WIDTH - 1; j >= 0; j--) {
            if (color & (1 << j)) {
                pwmData[indx] = 40;  // 2/3 of 90
            } else {
                pwmData[indx] = 20;  // 1/3 of 90
            }
            indx++;
        }

    }

    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> for loop end", __FUNCTION__);

    // TODO: refactor to memset
    for (int i = 0; i < RESERVE_TAIL_NUMBER_OF_LEDS; i++) {
        pwmData[indx] = 0;
        indx++;
    }

    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *) pwmData, indx);
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> HAL_TIM_PWM_Start_DMA end", __FUNCTION__);
    HAL_Delay(TIME_FOR_SENDING_DATA_TO_STRIP);
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> end", __FUNCTION__);
}

// public --------------------------------------------------------------------------------------------------------------

void LedController_Init() {
    memset(LEDS_DATA, 0, sizeof(LedData_t) * NUMBER_OF_LEDS);
}

void LedController_OnLed(int led_number, LedData_t *led_data) {
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> start", __FUNCTION__);
    if (led_number <= NUMBER_OF_LEDS) {
        LedController_SetLedData(led_number, led_data);
    }

    LedController_SendDataToStrip();
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "%s -> end", __FUNCTION__);
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
    LedData_t off_led = {0};
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        LedController_SetLedData(i,&off_led);
    }

    LedController_SendDataToStrip();
}
