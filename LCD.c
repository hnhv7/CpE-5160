#include "main.h"
#include "LCD.h"
#include "PORT.h"
#include "hardware_delay_TO_ms.h"


void LCD_Write(bit RS_Value, uint8_t Data_Value)
{
	RS = RS_Value;			// 0 is a command 	1 is DDRAM (Data Display RAM)
	E = 1;							// E is enabled to allow writing
	LCD_Port = Data_Value;
	E = 0;							// E is disabled
	LCD_Port = 0xFF;	// Return Port 0 to passive pull-up
											// (Allows for another device to use this port)	
}



/****************************************
*****************************************
Instructions for specific LCD Display:

POWER ON -> WAIT (30ms) -> FUNCTION SET -> WAIT (4.1ms) -> 
FUNCTION SET -> WAIT (100us) -> FUNCTION SET -> WAIT (39us) ->
FUNCTION SET -> WAIT (39us) -> DISPLAY CONTROL -> WAIT (39us) ->
DISPLAY CLEAR -> WAIT (1.5ms) -> ENTRY MODE -> WAIT (39us) -> END

******************************************
******************************************/
void LCD_Init()
{
	LCD_Write(command, function_set);							// Function Set
	hardware_delay(30);
	LCD_Write(command, function_set);							// Function Set
	hardware_delay(5);														// Need 4.1, so round UP
	LCD_Write(command, function_set);							// Function Set
	hardware_delay(1);														// Delay timer only goes to ms so round UP
	LCD_Write(command, function_set);							// Function Set
	hardware_delay(1);
	LCD_Write(command, display_on);								// Display Control
	hardware_delay(1);
	LCD_Write(command, display_clear);						//Display Clear
	hardware_delay(1);				
	LCD_Write(command, dec_mode);									//Entry Mode
	hardware_delay(1);
	
}

void LCD_Output_Display(uint8_t number_bytes, uint8_t *array)
{
	uint8_t index;
	for(index = 0; index < number_bytes; index++)
		{
			LCD_Write(1, *(array + index));
		}
	
}
