#include "main.h"
#include "PORT.H"
#include "hardware_delay_T1_1ms.h"
#include "T2_Init.h"
#include "T2_inter.h"


main()
{
  if(OSC_PER_INST==6)
		CKCON0 = 0x01;				// Set clock to x2 
	
	GREENLED = ON;					// 
	AMBERLED = ON;					//  
	YELLOWLED = ON;					//  
	REDLED = ON;						// Test
	DELAY_1ms_T1(1000);			// all
	AMBERLED = OFF;					// the 
	YELLOWLED = OFF;				// lights
	GREENLED = OFF;					//
	REDLED = OFF;						// 
	
	// Press any combination of 2 buttons to begin program
//	while( ((SW1 == OFF) || (SW2 == OFF)) && ((SW1 == OFF) || (SW3 == OFF)) && ((SW1 == OFF) || (SW4 == OFF)) && ((SW2 == OFF) || (SW3 == OFF)) && ((SW2 == OFF) || (SW4 == OFF))&& ((SW4 == OFF) || (SW3 == OFF)));
//	while((SW2 == ON) || (SW1 == ON) || (SW3 == ON) || (SW4 == ON));
	// Waits for no buttons to be pressed 
	// to ensure it begins with the main program

	
	Timer_2_Init();
	EA = 1; // Global interrupts enabled
	
	while(1)
	{}
	

}



