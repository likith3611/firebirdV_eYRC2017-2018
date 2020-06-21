/*
 * GccApplication7.c
 *
 * Created: 12-01-2018 22:04:34
 *  Author: liki
 */ 


/*
 * GccApplication6.c
 *
 /*
 * GccApplication3.c
 *
 * Created: 03-01-2018 22:29:54
 *  Author: bhask
 */
#define F_CPU  7372900
#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int  i=0,j=0;
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


ISR(TIMER2_OVF_vect)
{
	
 TCNT2=111;
 

 
 
 
}
void pwm_init()
{	cli(); 			//Clears the global interrupts
	port_init();
 
 TCCR2 |= (1<<WGM20)|(1<<COM21)|(1<<WGM21)|(1<<CS20)|(1<<CS21)|(1<<CS22);
 TIMSK |= (1 << TOIE2);
  
 cli();
 DDRD |= (1<<PD7);
 TCNT2=111;
 
 
 sei();
 
 
}
/*int servo_rotation(int rotation)
{
	double time;
	int data1; 
	time = rotation*(1/180);
	time =(time/35.55)*256;
	data1= 111+time + 5.20112517;
	
	return(data1);
	
	
	
}*/
int main(void)
{

 pwm_init();
 
 _delay_ms(1000);
 while (1)
 {
	_delay_ms(4000);
	 OCR2=120;//120
	 _delay_ms(60);
	 OCR2=122;//122
	 _delay_ms(300);
	 
 }


}