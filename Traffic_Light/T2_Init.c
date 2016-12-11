#include "main.h"
#include "PORT.h"
#include "T2_Init.h"


void Timer_2_Init()
{
	T2CON = 0x04;
	TH2 = Timer_Reload_1ms_THx;
	TL2 = Timer_Reload_1ms_TLx;
	RCAP2H = Timer_Reload_1ms_THx;
	RCAP2L = Timer_Reload_1ms_TLx;
	
	ET2 = 1;
	TR2 = 1;
	
}