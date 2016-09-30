#ifndef _hardware_delay_TO_ms_H
#define _hardware_delay_TO_ms_H

#include "main.h"

#define time_ms 1
#define Preload1 (65536-(uint16_t)((OSC_FREQ*time_ms)/(1020*OSC_PER_INST)))
#define Preload_H (Preload1 >> 8)
#define Preload_L (Preload1 & 0xFF)


void hardware_delay(uint16_t timeout_ms);

#endif