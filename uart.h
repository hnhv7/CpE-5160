#ifndef _UART_H
#define _UART_H
#include "main.h"
//#define SMOD1(0)
//#define spd(1)

#include "main.h"

// ------ Public function prototypes -------------------------------

void UART_Init(uint16_t baud_rate);
uint8_t UART_transmit(uint8_t send_value);
uint8_t UART_recieve(void);




#endif