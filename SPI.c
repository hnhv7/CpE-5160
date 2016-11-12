#include <stdio.h>
#include "main.h"
#include "SPI.h"
#include "PORT.H"
#include "hardware_delay_1ms.h"

uint8_t SPI_MASTER_Init(uint32_t clock_rate)
{
	uint8_t  clock_divider;
	uint8_t return_value = no_errors;
	
	clock_divider = (uint8_t) ((OSC_FREQ * 6)/(OSC_PER_INST * clock_rate));
	
	if (clock_divider <= 2)
		{SPCON = 0x70 | CPOL_value << 3 | CPHA_value << 2;}

	else if (clock_divider <= 4)
		{	SPCON = 0x71 | CPOL_value << 3 | CPHA_value << 2;}

	else if (clock_divider <= 8)
		{SPCON = 0x72 | CPOL_value << 3 | CPHA_value << 2;}

	else if (clock_divider <= 16)
		{SPCON = 0x73 | CPOL_value << 3 | CPHA_value << 2;}

	else if (clock_divider <= 32)
		{SPCON = 0xF0 | CPOL_value << 3 | CPHA_value << 2;}

	else if (clock_divider <= 64)
		{SPCON = 0xF1 | CPOL_value << 3 | CPHA_value << 2;}

	else if (clock_divider <= 128)
		{SPCON = 0xF2 | CPOL_value << 3 | CPHA_value << 2;}

	else 
		{return_value = illegal_clock_rate;}

	return return_value;
	
	
}


uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value)
{
	uint8_t timeout = 0;
	uint8_t return_value = no_errors;
	uint8_t status_value = SPSTA;


		SPDAT = send_value;


		do 
			{
				status_value = SPSTA;
				timeout++;
			}
		while (((status_value & 0xF0) == 0) && (timeout != 0));
			
		if (timeout == 0)
			{
				*received_value = 0xFF;
				return_value = TIMEOUT_ERROR;
			}
		else 
			{
				if ((status_value &0x70) == 0)
				{
					*received_value = SPDAT;
					return_value = no_errors;
				}
				else
				{
					return_value = SPI_ERROR;
					*received_value = 0xFF;
				}
			}
		return return_value;
}
	
	
	
	

