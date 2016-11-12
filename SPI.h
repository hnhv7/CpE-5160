#ifndef _SPI_H
#define _SPI_H

#include "main.h"


#define CPOL_value 0
#define CPHA_value 0
#define no_errors 0
#define illegal_clock_rate 21
#define TIMEOUT_ERROR 22
#define SPI_ERROR 23


uint8_t SPI_MASTER_Init(uint32_t clock_rate);
uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value);



#endif