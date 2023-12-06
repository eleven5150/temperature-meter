#ifndef UARTRINGBUFFER_H_
#define UARTRINGBUFFER_H_

#include <string.h>
#include "stm32f1xx_hal.h"

/* change the size of the buffer */
#define UART_BUFFER_SIZE 512

typedef struct {
    unsigned char buffer[UART_BUFFER_SIZE];
    volatile unsigned int head;
    volatile unsigned int tail;
} RingBuf_t;


void UART_RingBuf_StoreChar(unsigned char sym, RingBuf_t *buffer);

/* Initialize the ring buffer */
void UART_RingBuf_Init(void);

/* reads the data in the rx_buffer and increment the tail count in rx_buffer */
uint8_t UART_RingBuf_GetChar(char *ret_sym);

/* writes the data to the tx_buffer and increment the head count in tx_buffer */
void UART_RingBuf_PutChar(char sym);

/* function to send the string to the uart */
void UART_RingBuf_PutString(const char *string);

int UART_RingBuf_Peek();

/* Checks if the data is available to read in the rx_buffer */
int UART_RingBuf_IsDataAvailable(void);

/* Resets the entire ring buffer, the new data will start from position 0 */
void UART_RingBuf_Flush(void);

/* Wait until a paricular string is detected in the Rx Buffer
* Return 1 on success and -1 otherwise
* USAGE: while (!(UART_RingBuf_WaitForString("some string")));
*/
int UART_RingBuf_WaitForString(char *string);

/* the ISR for the uart. put it in the IRQ handler */
void UART_RingBuf_IRQHandler(UART_HandleTypeDef *huart);

#endif /* UARTRINGBUFFER_H_ */
