#ifndef _LCD_H
#define _UART_H
#include "main.h"
#include "PORT.h"
#define command 0
#define set_address 0x80
#define line1 0x00
#define line2 0x40
#define function_set 0x3C			// 2 line mode and display on
	// Display Control
#define display_on 0x0C
#define display_off 0x08			// Turns off any display function
#define cursor_on 0x0A
#define blink_on 0x09
#define display_clear 0x01
	// Entry Mode Set
#define dec_mode 0x06					// Right to left
#define inc_mode 0x04					// Left to right
#define shift_on 0x05




void LCD_Write(bit RS_Value, uint8_t Data_Value);
void LCD_Init(void);
void LCD_Output_Display(uint8_t number_bytes, uint8_t *array);

#endif