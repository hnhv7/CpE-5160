#include "stdio.h"
#include "main.h"
#include "PORT.H"
#include "UART.h"
#include "print_bytes.h"
#include "SPI.h"
#include "SDcard.h"
#include "Long_Serial_In.h"
#include "Directory_Functions_struct.h"
#include "File_System.h"
#include "STA013_Config.h"



uint8_t xdata buf1[512];



main()
{
   uint32_t Current_directory, Entry_clus;
   uint16_t i, num_entries, entry_num;
   uint8_t error_flag;
   FS_values_t * Drive_p;

   AMBERLED=OFF;
   YELLOWLED=OFF;
   GREENLED=OFF;
   REDLED = ON;
   STA013_RESET=0;

	
   AUXR=0x0c;   // make all of XRAM available
   if(OSC_PER_INST==6)
   {
      CKCON0=0x01;  // set X2 clock mode
   }
   else if(OSC_PER_INST==12)
   {
    //  CKCON0=0x00;  // set standard clock mode 					// Commented out to get rid of warning...
   } 
   uart_init();
   error_flag=SPI_Master_Init(400000UL);
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }
   printf("SD Card Initialization ... \n\r");
   error_flag=SD_card_init();
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }
   error_flag=SPI_Master_Init(20000000UL);
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }
   for(i=0;i<512;i++)
   {
      buf1[i]=0xff;  
   }
   error_flag=Mount_Drive(buf1);
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }

   
    Drive_p=Export_Drive_values();
   Current_directory=Drive_p->FirstRootDirSec;

    STA013_init();   

   while(1)
   {
     printf("Directory Sector = %lu\n\r",Current_directory);
     num_entries=Print_Directory(Current_directory, buf1);
	   printf("Enter Selection = ");
  	 entry_num=(uint16_t)long_serial_input();
	   if(entry_num<=num_entries)
	   {
	      Entry_clus=Read_Dir_Entry(Current_directory, entry_num, buf1);
		  if(Entry_clus&directory_bit)
		  {
		     Entry_clus&=0x0FFFFFFF;
             Current_directory=First_Sector(Entry_clus);
		  }
 	      else
		  {
		     Open_File(Entry_clus, buf1);
		  }
		  
	   }
	   else
	   {
	      printf("Bad Selection...\n\r");
	   }  
   }
} 




