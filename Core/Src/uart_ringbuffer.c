#include "uart_ringbuffer.h"

UART_HandleTypeDef huart1;

#define uart &huart1

#define UART_RINGBUF_TIMEOUT 500
uint16_t uart_ringbuf_timeout;


RingBuf_t rx_buffer = {{0}, 0, 0};
RingBuf_t tx_buffer = {{0}, 0, 0};

RingBuf_t *_rx_buffer;
RingBuf_t *_tx_buffer;


void UART_RingBuf_Init(void) {
    _rx_buffer = &rx_buffer;
    _tx_buffer = &tx_buffer;

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(uart, UART_IT_ERR);

    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(uart, UART_IT_RXNE);
}

void UART_RingBuf_StoreChar(unsigned char sym, RingBuf_t *buffer) {
    int i = (unsigned int) (buffer->head + 1) % UART_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != buffer->tail) {
        buffer->buffer[buffer->head] = sym;
        buffer->head = i;
    }
}

uint8_t UART_RingBuf_GetChar(char *ret_sym) {
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer->head == _rx_buffer->tail) {
        return 1;
    } else {
        unsigned char sym = _rx_buffer->buffer[_rx_buffer->tail];
        _rx_buffer->tail = (unsigned int) (_rx_buffer->tail + 1) % UART_BUFFER_SIZE;
        *ret_sym = sym;
        return 0;
    }
}

/* writes a single character to the uart and increments head
 */
void UART_RingBuf_PutChar(char sym) {
    if ((uint8_t)sym >= 0) {
        int i = (_tx_buffer->head + 1) % UART_BUFFER_SIZE;
        while (i == _tx_buffer->tail);

        _tx_buffer->buffer[_tx_buffer->head] = (uint8_t) sym;
        _tx_buffer->head = i;

        __HAL_UART_ENABLE_IT(uart, UART_IT_TXE); // Enable UART transmission interrupt
    }
}

/* checks if the new data is available in the incoming buffer
 */
int UART_RingBuf_IsDataAvailable(void) {
    return (uint16_t) (UART_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % UART_BUFFER_SIZE;
}

/* sends the string to the uart
 */
void UART_RingBuf_PutString(const char *string) {
    while (*string)
        UART_RingBuf_PutChar(*string++);
}

void UART_RingBuf_Flush(void) {
    memset(_rx_buffer->buffer, '\0', UART_BUFFER_SIZE);
    _rx_buffer->head = 0;
    _rx_buffer->tail = 0;
}



int UART_RingBuf_Peek() {
    if (_rx_buffer->head == _rx_buffer->tail) {
        return -1;
    } else {
        return _rx_buffer->buffer[_rx_buffer->tail];
    }
}

/* must be used after wait_for function
 * get the entered number of characters after the entered string
 */
//int Get_after(char *string, uint8_t numberofchars, char *buffertosave) {
//    for (int indx = 0; indx < numberofchars; indx++) {
//        uart_ringbuf_timeout = UART_RINGBUF_TIMEOUT;
//        while ((!UART_RingBuf_IsDataAvailable()) && uart_ringbuf_timeout);  // wait until some data is available
//        if (uart_ringbuf_timeout == 0) return 0;  // if data isn't available within time, then return 0
//        buffertosave[indx] = UART_RingBuf_GetChar(NULL);  // save the data into the buffer... increments the tail
//    }
//    return 1;
//}

/* Waits for a particular string to arrive in the incoming buffer... It also increments the tail
 * returns 1, if the string is detected
 */
// added timeout feature so the function won't block the processing of the other functions
int UART_RingBuf_WaitForString(char *string) {
    int so_far = 0;
    int len = strlen(string);

    again:
    uart_ringbuf_timeout = UART_RINGBUF_TIMEOUT;
    while ((!UART_RingBuf_IsDataAvailable()) && uart_ringbuf_timeout);  // let's wait for the data to show up
    if (uart_ringbuf_timeout == 0) return 0;
    while (UART_RingBuf_Peek() != string[so_far])  // peek in the rx_buffer to see if we get the string
    {
        if (_rx_buffer->tail != _rx_buffer->head) {
            _rx_buffer->tail = (unsigned int) (_rx_buffer->tail + 1) % UART_BUFFER_SIZE;  // increment the tail
        } else {
            return 0;
        }
    }
    while (UART_RingBuf_Peek() == string[so_far]) // if we got the first letter of the string
    {
        // now we will peek for the other letters too
        so_far++;
        _rx_buffer->tail = (unsigned int) (_rx_buffer->tail + 1) % UART_BUFFER_SIZE;  // increment the tail
        if (so_far == len) return 1;
        uart_ringbuf_timeout = UART_RINGBUF_TIMEOUT;
        while ((!UART_RingBuf_IsDataAvailable()) && uart_ringbuf_timeout);
        if (uart_ringbuf_timeout == 0) return 0;
    }

    if (so_far != len) {
        so_far = 0;
        goto again;
    }

    if (so_far == len) return 1;
    else return 0;
}


void UART_RingBuf_IRQHandler(UART_HandleTypeDef *huart) {
    uint32_t isrflags = READ_REG(huart->Instance->SR);
    uint32_t cr1its = READ_REG(huart->Instance->CR1);

    /* if DR is not empty and the Rx Int is enabled */
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)) {
        /******************
                     *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
                     *          error) and IDLE (Idle line detected) flags are cleared by software
                     *          sequence: a read operation to USART_SR register followed by a read
                     *          operation to USART_DR register.
                     * @note   RXNE flag can be also cleared by a read to the USART_DR register.
                     * @note   TC flag can be also cleared by software sequence: a read operation to
                     *          USART_SR register followed by a write operation to USART_DR register.
                     * @note   TXE flag is cleared only by a write to the USART_DR register.

        *********************/
        huart->Instance->SR;                       /* Read status register */
        unsigned char c = huart->Instance->DR;     /* Read data register */
        UART_RingBuf_StoreChar(c, _rx_buffer);  // store data in buffer
        return;
    }

    /*If interrupt is caused due to Transmit Data Register Empty */
    if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET)) {
        if (tx_buffer.head == tx_buffer.tail) {
            // Buffer empty, so disable interrupts
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

        } else {
            // There is more data in the output buffer. Send the next byte
            unsigned char c = tx_buffer.buffer[tx_buffer.tail];
            tx_buffer.tail = (tx_buffer.tail + 1) % UART_BUFFER_SIZE;

            /******************
            *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
            *          error) and IDLE (Idle line detected) flags are cleared by software
            *          sequence: a read operation to USART_SR register followed by a read
            *          operation to USART_DR register.
            * @note   RXNE flag can be also cleared by a read to the USART_DR register.
            * @note   TC flag can be also cleared by software sequence: a read operation to
            *          USART_SR register followed by a write operation to USART_DR register.
            * @note   TXE flag is cleared only by a write to the USART_DR register.

            *********************/

            huart->Instance->SR;
            huart->Instance->DR = c;

        }
        return;
    }
}
