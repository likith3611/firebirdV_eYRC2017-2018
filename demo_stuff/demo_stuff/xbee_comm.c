/*
 * GccApplication6.c
 *
 * Created: 07-01-2018 22:13:58
 *  Author: liki
 */ 


/*
 * GccApplication5.c
 *
 * Created: 07-01-2018 17:55:38
 *  Author: bhask
 */ 




#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

void buzzer_init (void)
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
SIGNAL(SIG_USART_RECV)
{	buzzer_off();
	
	

	unsigned int  r = UDR-'0'; //UDr-'0' conversion to decimal

}


void init_uart()
{
	
UCSRB=0x00;
	UCSRA = 0x00;
	UCSRC = 0x86;
	UBRRL = 0x2F; //set baud rate lo
	UBRRH = 0x00; //set baud rate hi
	UCSRB = 0x98;
	
	
	
}
void uart_transmit(unsigned int data)
{
	while (!(UCSRA & (1<<UDRE)));
	UDR= data+'0';  //UDR+'0' conversion to char from decimal


}
void xbee_init()
{cli();
	buzzer_init();	
	init_uart();
	sei();
}

