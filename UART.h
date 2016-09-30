#ifndef _UART_H
#define _UART_H
#include "main.h"
#define SMOD1 0
#define SPD 1
#define BAUD_RATE 9600
#define BRL_Value ((uint8_t) (256-(((1+(5*SPD))*(1+(1*SMOD1))*OSC_FREQ)/(32*OSC_PER_INST*(uint32_t)BAUD_RATE))))
#define Fperiph (OSC_FREQ*6/OSC_PER_INST)
#define TH1_Reload ((uint8_t) (256-(((1+(1*SMOD1))*OSC_FREQ)/(32*OSC_PER_INST*(uint32_t)BAUD_RATE))))




// ------ Public function prototypes -------------------------------

void UART_Init(void);
/**********************************
To get rid of the warnings...

uint8_t UART_transmit(uint8_t send_value);
uint8_t UART_recieve(void);
**********************************/



#endif