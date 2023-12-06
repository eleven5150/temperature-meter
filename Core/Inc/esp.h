#ifndef TEMPERATURE_METER_ESP_H
#define TEMPERATURE_METER_ESP_H
#include <stdlib.h>
#include <string.h>
#include "retarget.h"
#include "uart_ringbuffer.h"
#include "debug.h"
#include "temperature.h"

#define REQUEST_HEADER "+IPD,"

#define HTTP_HEADER "<!DOCTYPE html>\n<html>\n  <head>\n    <meta http-equiv=\"refresh\" content=\"0.5\" />\n    <title>TEMPERATURE METER</title>\n    <style>\n      html {\n        font-family: Helvetica;\n        display: inline-block;\n        margin: 0px auto;\n        text-align: center;\n      }\n      h1 {\n        color: #444444;\n        margin: 50px auto 30px;\n        font-size: 60px;\n      }\n      .button {\n        width: 600px;\n        height: 200px;\n        background-color: rgb(27, 173, 235);\n        color: rgb(0, 0, 0);\n        padding: 13px 30px;\n        text-decoration: none;\n        text-align: center;\n        font-size: 120px;\n        border-radius: 16px;\n      }\n    </style>\n  </head>\n  <body>\n    <h1>TEMPERATURE METER</h1>"
#define HTTP_CORE "<a class=\"button\" href=\"/ledoff\">%u.%u C</a>"
#define HTTP_FOOTER "</body>\n  <footer>\n    <p style=\"font-size: 20px; color: #888\">Kutaev Kirill ICS4-31M, BMSTU 2023</p>\n  </footer>\n</html>"

void ESP_Receive();

void ESP_Send(char *string);

void ESP_CheckRequest(char *buff);

void ESP_SendToClient(ulong channel_num, char *data);

#endif //TEMPERATURE_METER_ESP_H
