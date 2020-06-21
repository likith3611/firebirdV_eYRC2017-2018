#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
void buzzer_pin_config (void)
{
 DDRC = DDRC | 0x08;  //Setting PORTC 3 as output
 PORTC = PORTC & 0xF7;  //Setting PORTC 3 logic low to turnoff buzzer
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








void uart_init(void) 
{  UCSRB = 0x00; //disable while setting baud rate
UCSRA = 0x00;
UCSRC = 0x86;
UBRRL = 0x2F; //set baud rate lo
UBRRH = 0x00; //set baud rate hi
UCSRB = 0x98; //0xD8;;
   } 
void init()
{cli();
 port_init();
uart_init();
sei();
}
SIGNAL(SIG_UART_RECV) 
{
 unsigned char r;
 r=UDR;
 if(r==0x38)
 {
	 buzzer_on();
 } 
}
/*SIGNAL(SIG_USART_TRANS)
{

UDR=0x38;
} */
int main(void)
{
 init();
    while(1)
    {
  _delay_ms(1000); 
  buzzer_off();
    }
}