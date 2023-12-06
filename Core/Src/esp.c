#include "esp.h"



void ESP_Receive() {
    char buff[256] = {0};

    uint8_t it = 0;
    char sym;
    while (UART_RingBuf_IsDataAvailable()) {
        if (!UART_RingBuf_GetChar(&sym)) {
            HAL_Delay(30);
//            debug(DEBUG_PRINT_TRACE, DEVICE_ESP, "sym -> %c", sym);

            buff[it] = sym;

            it++;

            if (it == 255 || sym == '\n') {
                break;
            }
        }
    }
    debug(DEBUG_PRINT_TRACE, DEVICE_ESP, "%s", buff);
}


void ESP_Send(char *string) {
    char *msg = calloc(strlen(string) + 3, sizeof(char));
    sprintf(msg, "%s\r\n", string);
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "-> [%s]: %s", DEVICE_ESP, string);
    UART_RingBuf_PutString(msg);
    free(msg);
}