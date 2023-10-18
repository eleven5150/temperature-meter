#ifndef INC_LEDCONTROLLER_H_
#define INC_LEDCONTROLLER_H_

#include "stdio.h"
#include "retarget.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "dma.h"


#define NUMBER_OF_LEDS 144
#define RESERVE_TAIL_NUMBER_OF_LEDS 50

#define WS2812_DATA_BIT_WIDTH 24
#define SHIFT_FOR_GEEN_LED 16
#define SHIFT_FOR_RED_LED 8
#define TIME_FOR_SENDING_DATA_TO_STRIP 10
#define MAX_COLOUR_INTENSITY 255
#define BLUE_COLOR_INTENSITY 31
#define GREEN_COLOR_INTENSITY 31
#define ZERO_COLOUR_INTENSITY 0
#define AVERAGE_COLOR_INTENSITY 31
#define AMOUNT_OF_DATA 4
#define NUMBER_OF_LED 0
#define GREEN_INTENSITY 1
#define RED_INTENSITY 2
#define BLUE_INTENSITY 3

typedef struct LedData {
    uint8_t green_intense;
    uint8_t red_intense;
    uint8_t blue_intense;
} LedData_t;


static void LedController_SetLedData(uint8_t led_num, LedData_t *led_data);

static void LedController_SendDataToStrip(void);

void LedController_Init();

void LedController_OnLed(int led_number, LedData_t *led_data);

void LedController_OffLed(int led_number);

void LedController_OffAllLeds(void);

void LedController_LoadingEffect(void);


static LedData_t LEDS_DATA[NUMBER_OF_LEDS];

#endif /* INC_LEDCONTROLLER_H_ */
