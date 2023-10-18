#ifndef INC_LEDCONTROLLER_H_
#define INC_LEDCONTROLLER_H_
#include "stdio.h"
#include "retarget.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "dma.h"

//Strip configuration
#define MAX_LED 3
#define USE_BRIGHTNESS 0
#define RESERVE 50

#define WS2812_DATA_BIT_WIDTH 24
#define PI 3.14159265
#define MAX_BRIGHTNESS 45
#define MIN_BRIGHTNESS 0
#define RIGHT_ANGLE 90
#define UNFOLDED_CORNER 180
#define SHIFT_FOR_GEEN_LED 16
#define SHIFT_FOR_RED_LED 8
#define TIME_FOR_SENDING_DATA 10
#define MAX_COLOUR_INTENSITY 255
#define BLUE_COLOR_INTENSITY 31
#define GREEN_COLOR_INTENSITY 31
#define MIN_COLOUR_INTENSITY 0
#define AMOUNT_OF_DATA 4
#define NUMBER_OF_LED 0
#define GREEN_INTENSITY 1
#define RED_INTENSITY 2
#define BLUE_INTENSITY 3
#define FIRST_LED 0
#define UNIT_BIT 1
#define UNEVEN_CELL 1
#define FIRST_CELL 1

void LedController_SetLED (int LEDnum, int Red, int Green, int Blue);
void LedController_SetBrightness (int brightness);
void LedController_WS2812Send (void);
void LedController_OnLed (int ledNumber);
void LedController_OffAllLeds (void);
void LedController_Load(void);
void LedController_OffLed (int ledNumber);


#endif /* INC_LEDCONTROLLER_H_ */
