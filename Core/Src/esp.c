#include "esp.h"



void ESP_Init() {
    ESP_Send("AT");
    HAL_Delay(100);
    ESP_Send("AT+CWMODE=1");
    HAL_Delay(100);
    ESP_Send("AT+CIPMUX=1");
    HAL_Delay(100);
    ESP_Send("AT+CIFSR");
    HAL_Delay(100);
    ESP_Send("AT+CIPSERVER=1,80");
    HAL_Delay(100);
}


void ESP_Receive() {
    char buff[256] = {0};

    uint8_t it = 0;
    char sym;
    while (UART_RingBuf_IsDataAvailable()) {
        if (!UART_RingBuf_GetChar(&sym)) {
            HAL_Delay(1);

            buff[it] = sym;

            it++;

            if (it == 255 || sym == '\n') {
                break;
            }
        }
    }
    ESP_CheckRequest(buff);
    debug(DEBUG_PRINT_TRACE, DEVICE_ESP, "%s", buff);
}


void ESP_Send(char *string) {
    char *msg = calloc(strlen(string) + 3, sizeof(char));
    sprintf(msg, "%s\r\n", string);
    debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "-> [%s]: %s", DEVICE_ESP, string);
    UART_RingBuf_PutString(msg);
    free(msg);
}


void ESP_CheckRequest(char *buff) {
    char *header_ptr = strstr(buff, REQUEST_HEADER);
    if (header_ptr) {
        ulong channel_num = strtoul(&header_ptr[5], NULL, 10);
        debug(DEBUG_PRINT_TRACE, DEVICE_CORE, "Channel -> %lu", channel_num);

        char response_data[1024] = {0};
        sprintf(response_data, HTTP_HEADER);
        char tmp[64] = {};
        sprintf(tmp, HTTP_CORE, Temperature_GetIntValue(), Temperature_GetDecValue());
        strcat(response_data, tmp);
        strcat(response_data, HTTP_FOOTER);
        ESP_SendToClient(channel_num, response_data);
    }
}

void ESP_SendToClient(ulong channel_num, char *data) {
    uint32_t data_len = strlen(data);
    char cmd[32];
    sprintf(cmd, "AT+CIPSEND=%lu,%lu\r\n", channel_num, data_len);
    ESP_Send(cmd);
    ESP_Send(data);
    ESP_Send("AT+CIPCLOSE=5");
}
