#include "main.h"
#include "PORT.H"
#include "I2C.h"
#include "STA013_Config.h"
#include <stdio.h>



/***********************************************************************



          
************************************************************************/


uint8_t STA013_init(void)
{
   uint8_t error_flag, timeout;
   uint8_t idata send[3], read[3];





   STA013_RESET=0;

   send[0]=0x01;   // address of the ID reg.  
   timeout=50;
	LOGIC_ANALYZER_PIN = 0;
   STA013_RESET=1;    // make STA013 active
   do
   {
      error_flag=I2C_Write(0x43,1,send);
      timeout--;
      if(timeout==0)
			{				AMBERLED=0;
				      printf("Failed to Write.... Received Value: %2.2bX \n\r",read[0]);
			}
   }while((error_flag!=no_errors)&&(timeout!=0));
   if(timeout!=0)
   {
      timeout=50;
	  do
	  {
        error_flag=I2C_Read(0x43,1,read);
        timeout--;
	  }while((error_flag!=no_errors)&&(timeout!=0));
	  if(timeout==0)
	  {
         printf("Read Error - Received Value: %2.2bX \n\r",read[0]);
	  }
   }
   	LOGIC_ANALYZER_PIN = 1;

         printf("Received Value: %2.2bX \n\r",read[0]);



   return error_flag;
}
