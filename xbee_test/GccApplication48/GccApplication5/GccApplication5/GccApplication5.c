/*
 * GccApplication5.c
 *
 * Created: 07-01-2018 17:55:38
 *  Author: liki
 */ 



#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

void port_init (void)
{
	buzzer_pin_config();
}

void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}



void init_uart()

{UCRSB = 0x00;
	UCSRA=0x00;
	  UCSRC |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
UBRRL = 0x2F; //set baud rate lo
UBRRH = 0x00; //set baud rate hi
    UCRSB |= (1<<TXEN)|(1<<RXEN);                // enable receiver and transmitter
    cli();
}
void uart_transmit (unsigned char data)
{
	while (!( UCSRA & (1<<UDRE)));                // wait while register is free
	UDR = '8';                                   // load data in the register
}
void uart_recieve (void)
{int r;
	while(!(UCSRA) & (1<<RXC));                   // wait while data is being received
	   r=UDR;
	if(r==0x38)
	buzzer_on();
	                                // return 8-bit data
}
int main(void)
{init_uart();
	buzzer_pin_config();
    while(1)
    {
        uart_transmit();
		//uart_recieve();
}
}