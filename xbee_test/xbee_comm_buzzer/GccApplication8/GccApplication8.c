/*
 * GccApplication8.c
 *
 * Created: 31-01-2018 19:09:56
 *  Author: liki
 */ 


#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include"buzzer_firebird.c"
void uart_transmit(unsigned int data)
{
	
	
	
	while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '8';
}
void usart_init(void)
{
	UCSR0B = 0x00;
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x5F;
	UBRR0H = 0x00;
	UCSR0B = 0x98;
	
}
int main(void)
{usart_init();
    while(1)
	{
      uart_transmit(122);
	}    
}