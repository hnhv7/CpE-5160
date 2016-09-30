#include "main.h"
#include "PORT.h"
#include "hardware_delay_TO_ms.h"

void hardware_delay(uint16_t timeout_ms)
{
	uint16_t time;
	TMOD &= 0xF0;				//Clears just Timer 0
	TMOD |= 0x01;				//Sets Timer 0 to Mode 1
	ET0 = 0;						//Disable Interupts
	TR0 = 0;						//Ensure Timer 0 is off
	
	for(time = 0l; time < timeout_ms; time++)
		{
			TH0 = Preload_H;//Loading values into
			TL0 = Preload_L;// timer ... 
			TF0 = 0;				// Clearing Timer 0 overflow flag
			TR0 = 1;				// Starting Timer 0
				while (TF0 == 0);
			TR0 = 0; // Stopping Timer 0 
			
			
		}
	
	
}