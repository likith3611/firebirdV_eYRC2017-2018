/*
 * firebirdno_idea.c
 *
 * Created: 24-02-2018 16:11:13
 *  Author: liki
 */
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include  <util/delay.h>

#include "lcd.h"
unsigned char ADC_conversion(unsigned char );
unsigned char ADC_valueL,ADC_valueR,ADC_valueC;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
unsigned int value;
 void motion_set (unsigned char Direction)
 {
	 int PortARestore = 0x00;
	 Direction &= 0x0F;
	 PortARestore = PORTA;
	 PortARestore &= 0xF0;
	 PortARestore |= Direction;
	 PORTA =  PortARestore;
	 
 }
 void scan(void)
 {
	 print_sensor(1,1,1);
	 print_sensor(1,5,2);
	 print_sensor(1,9,3);
	  Left_white_line = ADC_conversion(3);	//Getting data of Left WL Sensor
	  Center_white_line = ADC_conversion(4);	//Getting data of Center WL Sensor
	  Right_white_line = ADC_conversion(5);
 }
 void velocity (unsigned char left_motor, unsigned char right_motor)
 {
	 OCR5AL = (unsigned char)left_motor;
	 OCR5BL = (unsigned char)right_motor;
 }
 void motion_pinconfig (void)
 {
	 DDRA = DDRA | 0x0F;
	 PORTA = PORTA & 0xF0;
	 DDRL = DDRL | 0x18;
	 PORTL = PORTL | 0x18;
 }
 void normalmovement()
 {
	 while(!(Right_white_line>35 && Left_white_line> 35 && Left_white_line>35))
	 {
		 scan();
		 if((Center_white_line<35 && Left_white_line< 35 && Right_white_line>35)||(Center_white_line>35 && Left_white_line< 35 && Right_white_line>35))
		 {
			 Right();
			 
			 _delay_ms(100);
			 
		 }
		 if((Center_white_line>35 && Left_white_line> 35 && Right_white_line<35)||(Center_white_line<35 && Left_white_line> 35 && Right_white_line<35))
		 {
			 Left();
			 _delay_ms(100);
			 
		 }
		 if(Center_white_line>35 && Left_white_line< 35 && Right_white_line<35)
		 {
			 Forward();
			 _delay_ms(100);
			 
		 }
	 }
 }


 void timer_init(void)
 {
	 TCCR5B = 0x00;
	 TCNT1H = 0xFF;
	 TCNT5L = 0x01;
	 OCR5AH = 0x00;
	 OCR5AL = 0xFF;
	 OCR5BH = 0x00;
	 OCR5BL = 0xFF;
	 OCR5CH = 0x00;
	 OCR5CL = 0xFF;
	 TCCR5A = 0xA9;
	 TCCR5B = 0x0B;
 }

 void Leftback (void)
 {
	 velocity(0,90);
	 motion_set(0x08);
 }
 void Rightback (void)
 {
	 velocity(90,0);
	 motion_set(0x01);
 }

 void Forward (void)
 {
	 velocity(100,100);
	 motion_set (0x06);

	 
	 
	 
 }

 void Backward (void)
 {	velocity(90,90);
	 motion_set(0x09);
	 
 }
 void Right (void)
 {	velocity(110,0);
	 motion_set(0x0A);
	 
	 
 }
 void Left1 (void)
 {	velocity(0,90);
	 motion_set(0x05);

 }
 void Right1 (void)
 {	velocity(0,90);
	 motion_set(0x05);

 }

 void Left (void)
 {	velocity(0,110);
	 motion_set(0x05);

 }

 void Stop (void)
 {
	 motion_set(0x00);
 }
 //movemnt for all
 void forward_all()
 {
	 velocity(90,90);
	 Forward();
	 _delay_ms(550);
	 scan();
	 normalmovement();
	 Stop();
	 _delay_ms(700);
	 scan();
 }
 void reverse_all()
 {
	 velocity(120,120);
	 Right();
	 _delay_ms(950);
	 scan();
	 normalmovement();
	 Stop();
	 _delay_ms(750);
	 
	 scan();
 }
 void left_all()
 {//velocity(50,50);
	 //forward();
	 //	_delay_ms(150);

	 Left();
	 _delay_ms(750);
	 scan();
	 
	 normalmovement();
	 Stop();
	 _delay_ms(750);
	 
	 scan();
 }
 right_all()
 {//velocity(50,50);
	 //forward();
	 //_delay_ms(200);
	 
	 
	 Right();
	 _delay_ms(750);
	 
	 scan();
	 normalmovement();
	 Stop();
	 _delay_ms(750);
	 
	 scan();
 }


 void port_init()
 {
	 lcd_port_config();
	 adc_pin_Config();
	 motion_pinconfig();
	 
	 
 }
 void adc_pin_Config(void)
 {
	 
	 DDRF=0x00;
	 PORTF=0x00;
	 DDRK=0X00;
	 PORTK=0X00;
 }
 void adc_init()
 {
	 ADCSRA=0x00;
	 ADCSRB=0x00;
	 ADMUX=0x20;
	 ACSR=0x80;
	 ADCSRA=0x86;
 }
 unsigned char ADC_conversion(unsigned char ch )
 {
	 unsigned char a;
	 if(ch>7)
	 {
		 ADCSRB=0x08;
	 }
	 ch=ch & 0x07;
	 ADMUX=0x20 |ch;
	 ADCSRA =ADCSRA |0x40;
	 while((ADCSRA&0x10)==0);
	 a=ADCH;
	 ADCSRA=ADCSRA|0x10;
	 ADCSRB=0x00;
	 return a;
	 
	 
 }
 void print_sensor(char row,char coloumn,unsigned char channel)
 {
	 if(channel==3)
	 {ADC_valueL=ADC_conversion(channel);
		 lcd_print(row,coloumn,ADC_valueL, 3);
	 }
	 if(channel==2)
	 {ADC_valueC=ADC_conversion(channel);
		 lcd_print(row,coloumn,ADC_valueC, 3);
	 }
	 if(channel==1)
	 {ADC_valueR=ADC_conversion(channel);
		 lcd_print(row,coloumn,ADC_valueR, 3);
	 }

 }


int main(void)
{port_init();
    while(1)
    {normalmovement();
		Stop();
	   }
}