#include "main.h"
#include "PORT.H"
#include "I2C.h"
#include <stdio.h>

/********************************************************************

*********************************************************************/
#define I2C_FREQ (25000UL)
#define I2C_RELOAD (65536-((OSC_FREQ)/(OSC_PER_INST*I2C_FREQ*2)))
#define I2C_RELOAD_H (I2C_RELOAD/256)
#define I2C_RELOAD_L (I2C_RELOAD%256)

#define Continue (1)
#define Stop (2)


void I2C_clock_start(void);
void I2C_clock_delay(uint8_t control);

/***********************************************************************



************************************************************************/

uint8_t I2C_Write(uint8_t device_addr,uint8_t num_bytes,uint8_t * send_array)
{
  uint8_t send_val, return_val, num_bits, send_bit, sent_bit;

  SDA=1;
  SCL=1;
  return_val=no_errors;

  if((SCL==1) && (SDA==1))
  {
     I2C_clock_start();
     send_val=device_addr<<1;
		 send_val&=0xFE;           // Send device addr with lsb is 0 for write (Indicates r/w = 0)
	   SDA=0;
		 num_bytes++;
	 do
	 {
	    num_bits=8;
     do
			{
				 I2C_clock_delay(Continue);
				 SCL=0;
				 num_bits--;
				 send_bit=((send_val>>num_bits)&0x01);				      
				 SDA=(bit)send_bit;
				 I2C_clock_delay(Continue);
				 SCL=1;
				 while(SCL!=1);		// verify SCL is set
				 sent_bit=SDA;
				 if(sent_bit!=send_bit) 
					return_val=bus_busy_error;
			 }
		 while((num_bits>0)&&(return_val==no_errors));
			 
		 I2C_clock_delay(Continue);
		 SCL=0;
     SDA=1;     
     num_bytes--;
		 send_val=*(send_array);
		 send_array++;
		 I2C_clock_delay(Continue);
		 SCL=1;
		 while(SCL!=1);
		 sent_bit=SDA;
		 if(sent_bit!=0) 
			 return_val=NACK_error;
   }
	 while((num_bytes>0)&&(return_val==no_errors));
	 
      /****************************
				Stop Condition
				****************/
	  I2C_clock_delay(Continue);
    SCL=0;
	  SDA=0;
	  I2C_clock_delay(Continue);
	  SCL=1;
	  while(SCL!=1);
	  I2C_clock_delay(Stop);
	  SDA=1;
  }
  else
  { 
     return_val=bus_busy_error;
  }

  return return_val;
}

/***********************************************************************


          
************************************************************************/


uint8_t I2C_Read(uint8_t device_addr,uint8_t num_bytes,uint8_t * rec_array)
{
  uint8_t send_val, return_val, num_bits, send_bit, sent_bit,rcv_val;

  SDA=1;
  SCL=1;
  return_val=no_errors;
  if((SCL==1) && (SDA==1))
  {
   I2C_clock_start();
   send_val=device_addr<<1;
	 send_val|=0x01;           // Send device addr with lsb is 1 for write (Indicates r/w = 1)
	 SDA=0;
	 num_bits=8;
		
   do
	 {
			I2C_clock_delay(Continue);
			SCL=0;
			num_bits--;
			send_bit=((send_val>>num_bits)&0x01);
			SDA=(bit)send_bit;
			I2C_clock_delay(Continue);
			SCL=1;
			while(SCL!=1); 		// verify SCL is set
			sent_bit=SDA;
			if(sent_bit!=send_bit)
				return_val=bus_busy_error;
    }
	 while((num_bits>0)&&(return_val==no_errors));
	  I2C_clock_delay(Continue);
	  SCL=0;
    SDA=1;      
	  I2C_clock_delay(Continue);
	  SCL=1;
	  while(SCL!=1);  		// verify SCL is set
	  sent_bit=SDA;
	  if(sent_bit!=0) 
			return_val=NACK_error;
    while((num_bytes>0)&&(return_val==no_errors))
	  {
	     num_bits=8;
	   do
			 {
				 I2C_clock_delay(Continue);
				 SCL=0;
				 SDA=1;
 				 I2C_clock_delay(Continue);
				 SCL=1;
				 while(SCL!=1);  		// verify SCL is set				 
				 num_bits--;
				 rcv_val=rcv_val<<1; 
				 sent_bit=SDA;
				 rcv_val|=sent_bit;
			 }
		 while(num_bits>0);
		 *rec_array=rcv_val;
		 rec_array++;
		 if(num_bytes==1)
		 {
		    send_bit=1;  
		 }
		 else
		 {
		    send_bit=0;  
		 }
		 I2C_clock_delay(Continue);
		 SCL=0;
		 SDA=send_bit;
		 I2C_clock_delay(Continue);
		 SCL=1;
		 while(SCL!=1);
		 num_bytes--;
	  }
      /****************************
				Stop Condition
				****************/
	  I2C_clock_delay(Continue);
    SCL=0;
	  SDA=0;
	  I2C_clock_delay(Continue);
	  SCL=1;
	  while(SCL!=1);
	  I2C_clock_delay(Stop);
	  SDA=1;
  }
  else
  { 
     return_val=bus_busy_error;
  }
  return return_val;
}

/***********************************************************************



************************************************************************/

void I2C_clock_start(void)
{
   TMOD &=0xF0;
   TMOD |=0x01;

   ET0=0;  //disable interrupts

   TH0=I2C_RELOAD_H;
   TL0=I2C_RELOAD_L;

   TF0=0;   // Clear overflow flag
   TR0=1;   // Start Timer
}


/***********************************************************************



************************************************************************/

void I2C_clock_delay(uint8_t control)
{
   if(TR0==1)
   { 
     while(TF0==0);
   }

   TR0=0;
   if(control==Continue)
   {
      TH0=I2C_RELOAD_H;
      TL0=I2C_RELOAD_L;

      TF0=0;   // Clear overflow flag
      TR0=1;   // Start Timer
   }
}


