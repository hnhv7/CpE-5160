#ifndef _SDCARD_H
#define _SDCARD_H

#include "main.h"

#define ILLEGAL_COMMAND 25
#define RECIEVE_ERROR 26
#define RESPONSE_ERROR0 30
#define RESPONSE_ERROR8 38
#define RESPONSE_ERROR58 358
#define VERSION1 100
#define STDCARD 101
#define SDCARD_ERROR 102
#define READ_BLOCK_ERROR 103

uint8_t send_command(uint8_t command, uint32_t arguement);
uint8_t receive_response(uint8_t numbytes, uint8_t *receive_array);
uint8_t SD_Init();
uint8_t read_block(uint16_t number_of_bytes, uint8_t *array);



#endif