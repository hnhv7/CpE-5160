#include <stdio.h>
#include "main.h"
#include "PORT.H"
#include "hardware_delay_1ms.h"
#include "sdcard.h"
#include "SPI.h"



uint8_t send_command(uint8_t command, uint32_t arguement)
{
	uint8_t error_flag, send_value;
	uint8_t return_value;						
	
	
	if(command < 64)
	{
		send_value = 0x40 | command;
		error_flag = SPI_Transfer(send_value, &return_value);
		
		send_value = (uint8_t)(arguement >> 24);
		error_flag = SPI_Transfer(send_value, &return_value);

		send_value = (uint8_t)(arguement >> 16);
		error_flag = SPI_Transfer(send_value, &return_value);
		
		send_value = (uint8_t)(arguement >> 8);
		error_flag = SPI_Transfer(send_value, &return_value);
				
		send_value = (uint8_t)(arguement);
		error_flag = SPI_Transfer(send_value, &return_value);
		
		if (command == 0)
		{send_value = 0x95;}
		else if (command == 8)
		{send_value = 0x87;}
		else
		{send_value = 0x01;}
		
		error_flag = SPI_Transfer(send_value, &return_value);
		return_value = no_errors;
	}
	else 				// Command >= 64  => Illegal Comamand
	{return_value = ILLEGAL_COMMAND;}

return return_value;	
}


uint8_t receive_response(uint8_t numbytes, uint8_t *receive_array)
{
	
	
	uint8_t timeout = 0;
	uint8_t receive_value, return_value, error_flag;
	uint8_t send_value;							// Junk value that is discarded
	
	
	do
	{
		send_value = 0xFF;
		error_flag = SPI_Transfer(send_value, &receive_value);
		timeout++;		
	}
	while ((receive_value == 0xFF) && (timeout != 0));
	
	if (timeout == 0)
	{ return_value = TIMEOUT_ERROR;}
	else if (error_flag != no_errors)
	{ return_value = RECIEVE_ERROR;}
	else
	{
		*receive_array = receive_value;
		if((receive_value == 0x01) || (receive_value == 0x00))
		{
			if(numbytes > 1)
			{
				uint8_t index;
				for (index = 1; index < numbytes; index++)
				{
					error_flag = SPI_Transfer(0xFF, &receive_value);
					receive_array[index] = receive_value;
				}
			}
			return_value = no_errors;
		}
	}
	
	error_flag = SPI_Transfer(0xFF, &receive_value);
	return return_value;
}


uint8_t SD_Init()
{
	uint8_t index, return_value, error_flag, error_status;
	uint8_t receive_array[10];
	uint8_t timeout = 0;
	
	
	printf("Initializing SD Card...");
	printf("\n");
	
	ncs = 1;							// SD_Select
	for (index = 0; index < 10; index++)
	{SPI_Transfer(0xFF, &return_value);}
	
/***********************************************************
	Command 0
	
************************************************************/
	ncs = 0; 
	error_flag = send_command(0,0);
	printf("Command 0 sent...");
	printf("\n");


	error_flag = receive_response(1, receive_array);
	ncs = 1;
	
	if (error_flag == no_errors)
	{
		if(receive_array[0] == 0x01)
		{error_status = no_errors;}
		else 
		{error_status = RESPONSE_ERROR0;}
	}
	else 
	{error_status = error_flag;}
	
	printf("Response - 0x");
	printf("%2.2Bx", receive_array[0]);
	printf("\n");
	printf("\n");
	DELAY_1ms_T1(1000);
/***********************************************************
	Command 8
	
************************************************************/
	if(error_flag == no_errors)
	{
		ncs = 0; 
		error_flag = send_command(8,0x000001AA);
		printf("Command 8 sent...");
		printf("\n");
		
		error_flag = receive_response(5, receive_array);
		ncs = 1;

		printf("Response - 0x");
		printf("%2.2Bx", receive_array[0]);
		printf("\n");	
		printf("\n");

		if (receive_array[0] == 0x05)
			{	
				printf("Version 1 Card is NOT supported.\n");
				error_status = VERSION1;
			}
		else
		{
			if (receive_array[0] == 0x01)
			{
				printf("\nVersion 2 Card detected. \n\n");
				error_status = no_errors;
			}
			else
			{error_status = RESPONSE_ERROR8;}
		}
		

		
		/*		printf("%2.2Bx", receive_array[1]);
		printf("\n");	
				printf("%2.2Bx", receive_array[2]);
		printf("\n");	
				printf("%2.2Bx", receive_array[3]);
		printf("\n");	
				printf("%2.2Bx", receive_array[4]);
		printf("\n");	
*/
	}

	   DELAY_1ms_T1(1000);
/***********************************************************
	Command 58
	
************************************************************/
	if(error_flag == no_errors)
	{
		ncs = 0; 
		error_flag = send_command(58,0);
		printf("Command 58 sent...");
		printf("\n");	
		
		error_flag = receive_response(5, receive_array);
		ncs = 1;	
		
		if (receive_array[0] == 0x01)
		{error_status == no_errors;}
		else
		{error_status = RESPONSE_ERROR58;}
		
		printf("Response - 0x");
		printf("%2.2Bx", receive_array[0]);
		printf("\n");	
		printf("\n");
		
			/*			printf("%2.2Bx", receive_array[1]);
		printf("\n");	
				printf("%2.2Bx", receive_array[2]);
		printf("\n");	
				printf("%2.2Bx", receive_array[3]);
		printf("\n");	
				printf("%2.2Bx", receive_array[4]);
		printf("\n");	*/
	}
	
/***********************************************************
	Command ACMD41
	
************************************************************/
	if(error_flag == no_errors)
	{
		ncs = 0; 
		timeout = 0;
		printf("Command 55 sent...\n");
		do
			{
				error_flag = send_command(55,0);
				if (error_flag == no_errors)
					{error_flag = receive_response(1, receive_array);}
				if ((receive_array[0] == 0x01) || (receive_array[0] == 0x00))
					{error_flag = send_command(41,0x40000000);}
				if (error_flag == no_errors)
					{error_flag = receive_response(1, receive_array);}
				timeout++;
				if (timeout == 0)
					{error_flag = TIMEOUT_ERROR;}
			}
		while(((receive_array[0] &0x01 ) ==0x01)&&(error_flag == no_errors) && (timeout == 0));
		
		if (timeout == 0)
			{error_flag = TIMEOUT_ERROR;}
			
		printf("Response - 0x");
		printf("%2.2Bx", receive_array[0]);
		printf("\n");	
		printf("\n");
			
			
			
	
			
			
		ncs = 1;
		
	}
	
/***********************************************************
Command 58
	
************************************************************/
	if(error_flag == no_errors)
		{
			
		ncs = 0; 
		error_flag = send_command(58,0);
		printf("Command 58 sent...");
		printf("\n");	
		
		error_flag = receive_response(5, receive_array);
		ncs = 1;	
		
		if (receive_array[0] == 0x01)
		{error_status == no_errors;}
		else
		{error_status = RESPONSE_ERROR58;}
		
		
		if (receive_array[1] & 0xC0)
		{
			error_flag = no_errors;
			printf("High Capacity Card Detected!");
			printf("\n");	
			printf("\n");	
		}
		else	
		{
			if (receive_array[1] & 0x80)
			{error_flag = STDCARD;}
			else
			{error_flag = RESPONSE_ERROR58;}
		}
	}


	if(error_flag != no_errors)
	{
		printf("An error has occurred...");
		printf("%Bx", error_status);
		printf("\n");
	}		

return error_status;

}


uint8_t read_block(uint16_t number_of_bytes, uint8_t *array)
{
	uint8_t error_flag, error_status, receive_value;
	uint8_t receive_array[10];
	uint8_t timeout = 0;	
	
	ncs = 0;
	send_command(17, 0);
	do
	{
		timeout++;
		error_flag=SPI_Transfer(0xFF,&receive_value);

	}
	while (((receive_value &0x80) == 0x80)&&(timeout != 0) && (error_flag == no_errors));
/*****************************************************/		
	if(timeout == 0)
		{error_flag = TIMEOUT_ERROR;}
	else
		if ( error_flag != no_errors)
			{error_status = READ_BLOCK_ERROR;}
	else 
	{
		if (receive_value == 0x00)
		{
	timeout = 0;
	do
	{ 
		timeout++;
		error_flag=SPI_Transfer(0xFF,&receive_value);
	}
	while ((timeout != 0) && (receive_value == 0xFF) && (error_flag == no_errors));	
	
/*****************************************************/		
	
	if(timeout == 0)
		{error_flag = TIMEOUT_ERROR;}
	else if (error_flag != no_errors)
		{error_flag = SDCARD_ERROR;}
	else if (receive_value == 0xFE)
	{
		for (timeout = 0; timeout < number_of_bytes; timeout++)
		{
			error_flag = SPI_Transfer(0xFF, &receive_value);
			*(array + timeout) = receive_value;
		}
		error_flag = SPI_Transfer(0xFF, &receive_value);
		error_flag = SPI_Transfer(0xFF, &receive_value);
	}
	else
	{error_status = SDCARD_ERROR;}
	}
			else
	{error_status = SDCARD_ERROR;}
}
	ncs = 1;
	error_flag=SPI_Transfer(0xFF,&receive_value);
	return error_status;
	
}