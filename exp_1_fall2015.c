#include "stdio.h"
#include "main.h"
#include "PORT.H"
#include "UART.h"
#include "print_bytes.h"
#include "memory_test.h"
#include "hardware_delay_1ms.h"
#include "LCD_routines.h"
#include "SPI.h"
#include "Long_Serial_In.h"
#include "SDcard.h"

uint8_t code Line1_string[]="    Hello";
uint8_t code Line2_string[]="Richie Nantz";


void Update_Line1(void);
void Update_Line2(void);

main()
{
   uint8_t *memory1, *memory2;
   uint32_t *block_number;
   uint32_t input_value, input_value2;

   REDLED=0;
   AUXR=0x0c;   // make all of XRAM available, ALE always on
   if(OSC_PER_INST==6)
   {
       CKCON0=0x01;  // set X2 clock mode
   }
   else if(OSC_PER_INST==12)
   {
       CKCON0=0x00;  // set standard clock mode
   }
   uart_init();
	 SPI_MASTER_Init(400000);
	 SD_Init();
	 
	 
	 SPI_MASTER_Init(10000000);
	 
   DELAY_1ms_T1(300);
   REDLED=1;
   memory1=code_memory_init();
   memory2=xdata_memory_init();

   print_memory(memory1, 41);
   putchar(CR);
   putchar(LF);
   print_memory(memory2, 42);
   putchar(CR);
   putchar(LF);
   LCD_Init();  
   Update_Line1();
   Update_Line2();
	 
	 
	 
	 
   while(1)
   {
		 printf("Input Block - " );
		 
		 input_value2=long_serial_input();
	 
		 read_block(input_value2, block_number);
		
		 DELAY_1ms_T1(100);
		 print_memory(block_number,512);

		 
		 
		 

	    input_value=UART_Receive();
      UART_Transmit(input_value);

   } 
}

void Update_Line1(void)
{
   LCD_Write(COMMAND,set_ddram_addr|line1);
   DELAY_1ms_T1(1);
   LCD_Print(0,Line1_string);
}


void Update_Line2(void)
{
   LCD_Write(COMMAND,set_ddram_addr|line2);
   DELAY_1ms_T1(1);
   LCD_Print(12,Line2_string);
}
