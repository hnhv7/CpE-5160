#include "T2_inter.h"
#include "main.h"
#include "PORT.h"
#include "hardware_delay_T1_1ms.h"





void Timer_2_ISR() interrupt Timer_2_Overflow
{	
	
	TF2 = 0; // Clear interrupt flag

	switch (system_state_g)
	{
		/*********************************
							Main Program
		*********************************/
		case RED_LIGHT:
		{
			time++;
			REDLED = ON;
			GREENLED = OFF;
			AMBERLED = OFF;
			YELLOWLED = OFF;
			if (time >= 2000)
			{
				system_state_g = WALK_LIGHT;
				time = 0;
			}
		
			break;
		}
		
		case GREEN_LIGHT:
		{
			time++;
			REDLED = OFF;
			GREENLED = ON;
			AMBERLED = OFF;
			YELLOWLED = OFF;
			if (SW1 == ON)
			{
				system_state_g = YELLOW_LIGHT;
				time = 0;
			}
			
			break;
		}
		
		case YELLOW_LIGHT:
		{
			time++;
			REDLED = OFF;
			GREENLED = OFF;
			AMBERLED = OFF;
			YELLOWLED = ON;
			if (time >= 2000)
			{
				system_state_g = RED_LIGHT;
				time = 0;
			}
			
			break;
		}
		
		case WALK_LIGHT:
		{
			time++;
			REDLED = ON;
			GREENLED = OFF;
			AMBERLED = ON;
			YELLOWLED = OFF;
			if (time >= 5000)
			{
				system_state_g = WALK_LIGHT_FLASH;
				time = 0;
			}
			
			break;
		}
		
		case WALK_LIGHT_FLASH:
		{
			time++;
			flash_timer++;

			
			if (flash_timer >= 500)
			{
				flash_timer = 0;
				AMBERLED = ~AMBERLED;
			}
			
			
			if (time >= 15000)
			{
				system_state_g = GREEN_LIGHT;
				time = 0;
			}
			
			break;
		}
	
		/*********************************
						Flashing Red Light
		*********************************/		
			
		case RED_FLASH:
		{
			time++;
			flash_timer++;
			GREENLED = OFF;
			AMBERLED = OFF;
			YELLOWLED = OFF;

			
			if (flash_timer >= 500)
			{
				flash_timer = 0;
				REDLED = ~REDLED;
			}
			
			
			if (SW1 == ON)
			{
				while(SW1 == ON);
				system_state_g = GREEN_LIGHT;
				time = 0;

			}
			
			break;
		}
			
		/*********************************
						No Walk Light
		*********************************/				
		case NW_RED:
		{
			time++;
			REDLED = ON;
			GREENLED = OFF;
			AMBERLED = OFF;
			YELLOWLED = OFF;
			if (time >= 1000)
			{
				system_state_g = NW_GREEN;
				time = 0;
			}
		
			break;
		}
		
		case NW_GREEN:
		{
			time++;
			REDLED = OFF;
			GREENLED = ON;
			AMBERLED = OFF;
			YELLOWLED = OFF;
			if (time >= 1000)
			{
				system_state_g = NW_YELLOW;
				time = 0;
			}
			
			break;
		}
		
		case NW_YELLOW:
		{
			time++;
			REDLED = OFF;
			GREENLED = OFF;
			AMBERLED = OFF;
			YELLOWLED = ON;
			if (time >= 1000)
			{
				system_state_g = NW_RED;
				time = 0;
			}
			
			break;
		}
			
	}
		/*********************************
						Switch Conditions
		*********************************/
	
		if (SW2 == ON)
			{
				system_state_g = RED_FLASH;
				time = 0;
			}
		if (SW3 == ON)
		{
			system_state_g = NW_GREEN;
			time = 0;
		}
		if (SW4 == ON)
		{
			system_state_g = GREEN_LIGHT;
			time = 0;
		}
		
}
