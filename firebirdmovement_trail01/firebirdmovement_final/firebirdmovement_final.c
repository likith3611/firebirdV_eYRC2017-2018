/*
 * firebirdmovement_final.c
 *
 * Created: 14-02-2018 11:56:29
 *  Author: liki
 */ 




#define F_CPU 14745600


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
unsigned char ADC_conversion(unsigned char );
unsigned char ADC_valueL,ADC_valueR,ADC_valueC;
unsigned int value;
unsigned int row1,row2,row3,noden,a[49];

void lcd_port_config (void)
{
	DDRC=DDRC | 0xF7;
	PORTC=PORTC & 0x80;
	
}


void motion_pinconfig (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;
	PORTL = PORTL | 0x18;
}

void adc_pin_config(void)
{	
	
	DDRF=0x00;
	PORTF=0x00;
	DDRK=0X00;
	PORTK=0X00;
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

void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

void motion_set (unsigned char Direction)
{
	int PortARestore = 0x00;
	Direction &= 0x0F;
	PortARestore = PORTA;
	PortARestore &= 0xF0;
	PortARestore |= Direction;
	PORTA =  PortARestore;
	
}

void Forward (void)
{
	velocity(150,150);
	motion_set (0x06);
}

void Backward (void)
{	velocity(110,110);
	motion_set(0x09);
}


void Right (void)
{
	velocity(120,100);
	motion_set(0x0A);
}


void Left (void)
{	velocity(90,120);
	motion_set(0x05);
}

void Stop (void)
{	velocity(0,0);
	motion_set(0x00);
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

void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pinconfig();
	adc_init();
}

void print_sensor(char row,char coloumn,unsigned char channel)
{
	if(channel==3)
	{
		ADC_valueL=ADC_conversion(channel);
		lcd_print(row,coloumn,ADC_valueL, 3);
	}
	if(channel==2)
	{
		ADC_valueC=ADC_conversion(channel);
		lcd_print(row,coloumn,ADC_valueC, 3);
	}
	if(channel==1)
	{
		ADC_valueR=ADC_conversion(channel);
		lcd_print(row,coloumn,ADC_valueR, 3);
	}

}
void init_devices(void)
{
	cli();
	port_init();
	adc_init();
	timer_init();
	lcd_port_config();	
	sei();
}
void turn(void){
	int Length;
	int m,a[Length],i;
	if(m%2!=0){
		
		if(a[i+1]<a[i]){
			
			Right();
		}
		else
		Left();
	}
	
	else
	{
		if(a[i+1]<a[i])
		{
			
			Left();
		}
		else
		{
			Right();
			_delay_ms(2000);
		}
	}
}

void movement_fire(int Length)
{		
	int start;
	start=40;
	int pos;
	pos=start;
	int b,c,d,e;
	int f;
	f=Length;
	int i;
	int m=0;  //for the number of turns
	for (i=0;i<f;i++)
	{

		e=pos-1;
		b=pos+1;
		c=pos+7;
		d=pos-7;

		if(a[i+1]==b)
		{
			Right();
			_delay_ms(100);
			do 
			{
				print_sensor(1,1,1);
				print_sensor(1,5,2);
				print_sensor(1,9,3);
				
				Forward();
			} while (ADC_valueC>35&&ADC_valueL>35&&ADC_valueR>35);
			
			pos=pos+1;
			m++;
		turn();
		}

		else if(a[i+1]==e)
		{
			Left();
			_delay_ms(100);
			do 
			{
				
				print_sensor(1,1,1);
				print_sensor(1,5,2);
				print_sensor(1,9,3);
				
				Forward();
			} while (ADC_valueC>35&&ADC_valueL>35&&ADC_valueR>35);
			pos=pos-1;
			m++;
		turn();
		}

		else if(a[i+1]==d)
		{
			Forward();
			pos=pos-7;
			m++;
			turn();
		}

		else if (a[i+1]==b)
		{
			Backward();
			_delay_ms(100);
			do
			{
				
				print_sensor(1,1,1);
				print_sensor(1,5,2);
				print_sensor(1,9,3);
				
			Stop();
			} while (ADC_valueC>35&&ADC_valueL>35&&ADC_valueR>35);
			
			pos=pos+7;
			m++;
			turn();
		}

		Stop();

	}
			
		}	

	// these lines are to orient the bot towards the forward direction at every new position


//TODO:: Please write your application code. You can make your own functions & header files




