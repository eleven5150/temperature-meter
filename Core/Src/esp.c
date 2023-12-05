#include "esp.h"

void ESP_Receive() {
    uint16_t buff_size = 2;
    char *buff = (char *)calloc(buff_size, sizeof(char));
    while (IsDataAvailable())
    {
        char sym = (char)Uart_read();
        HAL_Delay(200);
        if ((uint8_t)sym == 0xFF) {
            continue;
        }
        buff[buff_size - 2] = sym;

        if (sym == '\n') {
            break;
        }

        buff_size++;
        buff = (char *)realloc(buff, buff_size);
    }
    buff[buff_size - 1] = '\0';
    debug(DEBUG_PRINT_TRACE, DEVICE_ESP, "%s", buff);
    free(buff);
}


void ESP_Send(char *string) {
    char *msg = calloc(strlen(string) + 1, sizeof(char));
    sprintf(msg, "%s\r\n", string);
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "-> [%s]: %s", DEVICE_ESP, string);
    Uart_sendstring(msg);
    free(msg);
}