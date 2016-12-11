#ifndef _T2_INIT_H
#define _T2_INIT_H


#include "main.h"

typedef enum {RED_LIGHT, GREEN_LIGHT, YELLOW_LIGHT, WALK_LIGHT, WALK_LIGHT_FLASH, RED_FLASH, NW_RED, NW_YELLOW, NW_GREEN} states_t;

states_t system_state_g = GREEN_LIGHT;
uint32_t time, flash_timer;


/********* Public Functions ***********/
void Timer_2_ISR();


#endif