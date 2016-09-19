#include "main.h"

void UART_Init(uint16_t baud_rate)
{


}


uint8_t UART_transmit(uint8_t send_value)
{
	while (TI == 0);
	SBUF = send_value;
	
}

uint8_t UART_recieve(void)
{
	uint8_t recieved_value;
	while (RI == 0);
	recieved_value = SBUF;
	return recieved_value;
	
	
}