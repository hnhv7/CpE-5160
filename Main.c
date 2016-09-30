#include <stdio.h>
#include "UART.h"
#include "print_bytes.h"
#include "memory_test.h"
#include "PORT.h"
#include "hardware_delay_TO_ms.h"
#include "LCD.h"



void LED_Flash_Init(void);
void LED_Flash_Change_State(void);
void Delay_Loop_MS(const unsigned int);

uint8_t code LCD_string1[] = "Test LCD!";
uint8_t code LCD_string2[] = "It works!!!";

int main(void)
{
	uint8_t *codememory_ptr;
	uint8_t *xdatamemory_ptr;
			//uint8_t send_value;
	
	
	AUXR = 0x0C;										// Allowing use of all XRAM
	
	// Setting Clock
	if (OSC_PER_INST == 6)
		{CKCON0 = 0x01;}							// X2 Mode
	else 
		if (OSC_PER_INST == 12)
			{CKCON0 = 0x00;}						// Standard Mode
	
		// Silly display, that enabling LEDs to begin before UART_Init
	REDLED = 0;
	hardware_delay(500);
	YELLOWLED = 0;
	hardware_delay(500);
	AMBERLED = 0;
	hardware_delay(500);
	GREENLED = 0;
	hardware_delay(500);
	
	UART_Init();
	hardware_delay(500);
			
	REDLED = 1;
	YELLOWLED = 1;
	AMBERLED = 1;
	GREENLED = 1;
		// Disabling all LEDs to show UART_Init was successful
			
			

	
	codememory_ptr = code_memory_init();
	xdatamemory_ptr = xdata_memory_init();
	
	print_memory(codememory_ptr, 41);
	print_memory(xdatamemory_ptr, 42);


	LCD_Init();
	LCD_Write(command, set_address | line1);
	LCD_Output_Display(10, LCD_string1);
	hardware_delay(1);
	LCD_Write(command, set_address | line2);
	LCD_Output_Display(11, LCD_string2);




	while(1);
	return 0;
}
