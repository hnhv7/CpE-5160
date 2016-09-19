#include <stdio.h>
#include <reg52.h>

sbit LED_pin=p1^5;
bit LED_State_G;
void LED_Flash_Init(void);
void LED_Flash_Change_State(void);
void Delay_Loop_MS(const unsigned int);

int main(void)
{
	UART_Init();


}
