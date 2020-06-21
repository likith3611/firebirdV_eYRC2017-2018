/*
 * GccApplication7.c
 *
 * Created: 12-01-2018 22:04:34
 *  Author: liki
 */ 


/*
 * GccApplication6.c
 *

 * GccApplication3.c
 *
 * Created: 03-01-2018 22:29:54
 *  Author: bhask
 */
#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>





ISR(TIMER2_OVF_vect)
{
	
 TCNT2=111;
 

 
 
 
}
void servo_init()
{cli();	//Clears the global interrupts
 
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
 void servo_drop(void)
{


 
 
 

	 OCR2=120;//120
	 _delay_ms(60);
	 OCR2=122;//122
	 _delay_ms(500);


	 


}