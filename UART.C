#include "main.h"
#include "UART.h"


void UART_Init()
{

	PCON &= 0x3f;
		if (SMOD1 == 1)
		{
			PCON = 0x80;
		}
	SCON = 0x50;
		
		// Initialize Baud Rate Generator 
	BDRCON = 0;												// BDRCON must be turned off to set BRL
	BRL = BRL_Value;									// Setting BRL
	BDRCON = (0x1C | (SPD << 1));			// Re-enabling BDRCON for usage
	TI = 1; 													// Enable transmit interupts to allow for putchar()
	ES = 0;														// Disable serial interupts

}

/*
uint8_t UART_transmit(uint8_t send_value)
{
	while (TI == 0);
	SBUF = send_value;
	return 0;
}

uint8_t UART_recieve(void)
{
	uint8_t recieved_value;
	while (RI == 0);
	recieved_value = SBUF;
	return recieved_value;
	
	
}
*/