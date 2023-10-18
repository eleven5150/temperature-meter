#include "led_controller.h"

uint8_t ledData[MAX_LED][AMOUNT_OF_DATA] = {0};

uint16_t pwmData[(WS2812_DATA_BIT_WIDTH * MAX_LED) + RESERVE] = {0};

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
}

void LedController_SetLED(int ledNum, int red, int green, int blue) {
    ledData[ledNum][NUMBER_OF_LED] = ledNum;
    ledData[ledNum][GREEN_INTENSITY] = green;
    ledData[ledNum][RED_INTENSITY] = red;
    ledData[ledNum][BLUE_INTENSITY] = blue;
}


void LedController_WS2812Send(void) {
    uint32_t indx = 0;
    uint32_t color;

    for (int i = 0; i < MAX_LED; i++) {

        color = ((ledData[i][1] << SHIFT_FOR_GEEN_LED) | (ledData[i][2] << SHIFT_FOR_RED_LED) | (ledData[i][3]));

        for (int i = WS2812_DATA_BIT_WIDTH - 1; i >= 0; i--) {
            if (color & (1 << i)) {
                pwmData[indx] = 40;  // 2/3 of 90
            } else {
                pwmData[indx] = 20;  // 1/3 of 90
            }
            indx++;
        }

    }

    for (int i = 0; i < RESERVE; i++) {
        pwmData[indx] = 0;
        indx++;
    }

    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *) pwmData, indx);
}


void LedController_OnLed(int ledNumber) {
    if (ledNumber <= MAX_LED) {
        LedController_SetLED(ledNumber, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY, BLUE_COLOR_INTENSITY);
    }

    LedController_WS2812Send();
    HAL_Delay(TIME_FOR_SENDING_DATA);
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
}


void LedController_OffLed(int ledNumber) {
    if (ledNumber <= MAX_LED) {
        LedController_SetLED(ledNumber, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY);
    }

    LedController_WS2812Send();
    HAL_Delay(TIME_FOR_SENDING_DATA);
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
}


void LedController_Load(void) {
    LedController_OnLed(0);
    for (int m = 1; m < MAX_LED; m++) {
        LedController_OnLed(m);
        LedController_OnLed(m + 1);
        LedController_OffLed(m - 1);
    }
}

void LedController_OffAllLeds(void) {
    for (int m = 0; m < MAX_LED; m++) {
        LedController_SetLED(m, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY, MIN_COLOUR_INTENSITY);
    }
    LedController_WS2812Send();
    HAL_Delay(TIME_FOR_SENDING_DATA);
}
