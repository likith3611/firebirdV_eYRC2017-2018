
/*
 * TASK1B.c
 *
 * Created: 04/11/2017 22:58:28
 *  Author: Liki
 */ 
#define F_CPU 14745600


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
unsigned char ADC_conversion(unsigned char );
unsigned char ADC_valueL,ADC_valueR,ADC_valueC;
unsigned int value;
#include "buzzer.h"
void turn(void)
{
	int Length;
	int m,a[Length],i;
	if (m%2!=0)
	{
		if (a[i+1]<a[i])
		{
			Right();
		}
		else
		
		{
			Left();
		}
	}


	else
	{

		if(a[i+1]<a[i])
		{
			
		}
		

		else
		{
			Right();
			_delay_ms(2000);
		}
		// The amount of delay has to be twice the one given there
	}
}

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

void   ReachDestinationAvoidingNode  (unsigned char Xd,unsigned char Yd,unsigned char Xn,unsigned char Yn)
{
	
	
	 
	 unsigned int value;
	 init_devices();
	 lcd_set_4bit();
	 lcd_init();	
	
if(Yd=='A')
{
	Yd=0;
}
else if(Yd=='B')
{
	Yd=1;
}
else if(Yd=='C')
{
	Yd=2;
}
else if(Yd=='D')
{
	
	Yd=3;
}
else if(Yd=='E')
{
	Yd=4;
}
else if(Yd=='F')
{
	Yd=5;
}
else if(Yd=='G')
{
	Yd=6;
}
printf("%d\n",Yd);


if(Yn=='A')
{
	Yn=0;
}
else if(Yn=='B')
{
	Yn=1;
}
else if(Yn=='C')
{
	Yn=2;
}
else if(Yn=='D')
{
	
	Yn=3;
}
else if(Yn=='E')
{
	Yn=4;
}
else if(Yn=='F')
{
	Yn=5;
}
else if(Yn=='G')
{
	Yn=6;
}
printf("%d\n",Yn);
 
		int Px=4,Py=1;
		int A[50],B[50];
		int m =0;int n=0;
		int Qx=0;int Qy = 0;
		int t =0; int i;
		int total;int Path[50];
		for (i=0;i<50;i++)
		{
			Path[i]=99;
		}

		for(i=0;i<50;i++)
		{

			A[i]=99;
			B[i]=99;
		}
		A[m]=Px;B[n]=Py;
		//printf("%d   %d\n",A[m],B[n]);
		m++;n++;
		while((Xd!=Px)||(Yd!=Py))
		{
			while(Xd!=Px)
			{

				if(Xd<Px)
				{
					Qx=Px-1;
					//printf("%d\n",Qx);

					hen:
					if((Qx!=Xn)||(Py!=Yn))
					{
						A[m]=Qx;B[n]=Py;
						//    printf("%d   %d\n",A[m],B[n]);
						Px=Qx;
						m++;n++;t++;
					}
					else
					{
						if(Py==1)
						{
							Py = Py+1;
							Qx++;
							Px=Qx;
							goto hen;
						}
						else if(Py==8)
						{
							Py = Py-1;
							Qx++;
							Px=Qx;
							goto hen;
						}
						else
						{
							Py = Py+1;
							Qx++;
							Px=Qx;
							goto hen;
						}
					}
				}
				else if(Xd>Px)
				{
					Qx = Px+1;
					//printf("%d\n",Qx);
					hen1:
					if((Qx!=Xn)||(Py!=Yn))
					{
						A[m] = Qx;B[n]=Py;
						//       printf("%d   %d\n",A[m],B[n]);
						Px=Qx;
						m++;n++;t++;
					}
					else
					{
						if(Py==1)
						{
							Py = Py+1;
							Qx--;
							Px=Qx;
							goto hen1;
						}
						else if(Py==8)
						{
							Py = Py-1;
							Qx--;
							Px=Qx;
							goto hen1;
						}
						else
						{
							Py = Py+1;
							Qx--;
							Px=Qx;
							goto hen1;
						}

					}
				}
			}

			while(Yd!=Py)
			{
				if(Yd<Py)
				{
					Qy=Py-1;
					//printf("%d\n",Qy);
					hen2:
					if((Qy!=Yn)||(Px!=Xn))
					{
						A[m]=Px;B[n]=Qy;
						//    printf("%d   %d\n",A[m],B[n]);
						Py=Qy;
						m++;n++;t++;
					}
					else
					{
						if(Qy==1)
						{
							Px = Px+1;
							Qy++;
							Py=Qy;
							goto hen2;
						}
						else if(Qy==8)
						{
							Px = Px-1;
							Qy++;
							Py=Qy;
							goto hen2;
						}
						else
						{
							Px = Px+1;
							Qy++;
							Py=Qy;
							goto hen2;
						}
					}
				}
				else if(Yd>Py)
				{
					Qy = Py+1;
					//printf("%d\n",Qy);
					hen3:
					if((Qy!=Yn)||(Px!=Xn))
					{
						A[m] = Px;B[n]=Qy;
						//printf("%d   %d\n",A[m],B[n]);
						Py=Qy;
						m++;n++;t++;
					}
					else
					{
						if(Qy==1)
						{
							Px = Px+1;
							Qy--;
							Py=Qy;
							goto hen3;
						}
						else if(Qy==8)
						{
							Px = Px-1;
							Qy--;
							Py=Qy;
							goto hen3;
						}
						else
						{
							Px = Px+1;
							Qy--;
							Py=Qy;
							goto hen3;
						}

					}
				}
			}
		}
		 
		for(i=0;i<=t;i++)
		{
			total = ((A[i]-1)*7)+B[i];
			Path[i]=total;
			printf("%d\n",total);
		}
		
		int Length =t+1;
		printf("\n %d \n ",Length);
		
	int start;
	start=45;
	int pos;
	pos=start;
	int b,c,d,e;
	int f,a[Length];
	f=Length;
	for(i=0;i<f;i++)
	{
		
		a[i]
	}
	m=0;  //for the number of turns
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



//Do not make changes in main function

int main(void)
{
       ReachDestinationAvoidingNode(5,'D',6,'D');
	   buzzer_on();
	   _delay_ms(500);
	   buzzer_off();
	   ReachDestinationAvoidingNode(2,'F',2,'D');
	   buzzer_on();
	   _delay_ms(500);
	   buzzer_off();
	   ReachDestinationAvoidingNode(2,'A',2,'C');
	   buzzer_on();
	   
}
