/*
 * firebird_finalbomb.c
 *
 * Created: 20-02-2018 17:12:56
 *  Author: liki
 */ 

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include  <util/delay.h>
#include "path_finder_spark5.c"
#include "lcd.h"

unsigned char ADC_conversion(unsigned char );
unsigned char ADC_valueL,ADC_valueR,ADC_valueC;
unsigned char ADC_Value;
unsigned char flag10 = 0;

int l=0;
volatile int loc1,loc2=0,flag=0,flag2=0;flag3=0;flag4=0;
volatile int i=0,j=0,s100=0,spark_r[10],spark_b[10],fire_bird_r[10];
extern int c[49];
volatile int tent[5];
volatile loc3;
volatile dog=0;
int w,r30[3];
int y78=0,q89=0;
int present;
void uart_transmit(unsigned int data)
{
	lcd_print(1,2,data,4);
	lcd_print(2,2,data+'0',4);
	
	
	while(!(UCSR0A & (1<<UDRE0)));
	{
			UDR0 = data +'0' ;
			_delay_ms(1000);
		
	}

}
SIGNAL(SIG_USART0_RECV)
{
	int DATA = UDR0-'0';
	if(DATA>=100 && DATA<150)
	{
		spark_b[j]=DATA-100;
		j++;
	}
 if(DATA>=0 && DATA <50)
{
	spark_r[i]=DATA;
	i++;
	
	
}
 else if(DATA>=50 && DATA<100)
{
	fire_bird_r[s100]=DATA-50;
	s100++;
} 	

else if (DATA>=150 && DATA <200 )
	{
           loc1=DATA;
	}

	
else 
{
	flag=1;
} 

}
//ADC pin configuration
void adc_pin_config (void)
{
 DDRF = 0x00; 
 PORTF = 0x00;
 DDRK = 0x00;
 PORTK = 0x00;
}

//Function to configure ports to enable robot's motion
void motion_pin_config (void) 
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

//Function to Initialize PORTS
void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();	
}

// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//Function For ADC Conversion
unsigned char ADC_conversion(unsigned char Ch) 
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;  			
	ADMUX= 0x20| Ch;	   		
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

//Function To Print Sesor Values At Desired Row And Coloumn Location on LCD

	
	
	void print_sensor(char row,char coloumn,unsigned char channel)
	{
		if(channel==3)
		{ADC_valueL=ADC_conversion(channel);
		
		}
		if(channel==2)
		{ADC_valueC=ADC_conversion(channel);
			
		}
		if(channel==1)
		{ADC_valueR=ADC_conversion(channel);
			
		}
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

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 		// removing upper nibbel for the protection
 PortARestore = PORTA; 		// reading the PORTA original status
 PortARestore &= 0xF0; 		// making lower direction nibbel to 0
 PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
 PORTA = PortARestore; 		// executing the command
}

void usart_init(void)
{cli();
	UCSR0B = 0x00;
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x5F;
	UBRR0H = 0x00;
	UCSR0B = 0x98;
	sei();
}

//motion config






//movemnt for all
void movement_for_all(){	
	int start=c[0];
	int pos=start;
	int x;
	int lenght=0;
	int r,u,dw;
	int d=y78;
	
	
	for(x=0;x<49;x++)
	{if(c[x]!=99)
		{lenght++;
                }
else
{
break;
}
	}
	 
	  //for the number of turns
	for (x=1;x<=lenght;x++)
	{
lcd_print(1,2,pos,4);
		l=pos-1;
		r=pos+1;
		u=pos-7;
		dw=pos+7;
		
		/*if(a[i]==pos)
		{
			continue;
		}*/
uart_transmit(pos+200);
		if(c[x]==r)
		{	
			if(d==1)
			{
				Forward();
				_delay_ms(750);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						
						_delay_ms(100);
						
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
						
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
				
					}
				}
				
			}
			else if(d==3)
			{
				Backward();
				_delay_ms(750);
				scan();
			
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Leftback();
						_delay_ms(100);
					
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Rightback();
						_delay_ms(100);
					
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Backward();
						_delay_ms(100);
						
					}
				}
				
			}
			else if(d==0)
			{
				
			
			
			Right();
			_delay_ms(750);
			scan();
			while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
			{
				scan();
				if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
				{
					Right();
					_delay_ms(100);
				
				}
				if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
				{
					Left();
					_delay_ms(100);
				
				}
				if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
				{
					Forward();
					_delay_ms(100);
				
				}
				
			}
			
			}			
			else 
                            {Left();
                              _delay_ms(750);
			scan();
			while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
			{
				scan();
				if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
				{
					Right();
					_delay_ms(100);
				
				}
				if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
				{
					Left();
					_delay_ms(100);
					
				}
				if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
				{
					Forward();
					_delay_ms(100);
				
				}
				
			}Stop();
				_delay_ms(1000);
				pos = pos+1;
				scan();
			}				
		d=1;


                             



			
		}

		else if(c[x]==l)
		{
			if(d==3)
			{
				Forward();
				_delay_ms(750);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				
			}
			else if(d==1)
			{
				Left();
				_delay_ms(2000);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
						if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
						{
							Right();
							_delay_ms(100);
						}
						if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
						{
							Left();
							_delay_ms(100);
						}
						if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
						{
							Forward();
							_delay_ms(100);
						}
				}
				
			}
			else if(d==0)
			{
				
			
			Left();
			_delay_ms(750);
			scan();
			while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
			{
				scan();
				if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
				{
					Right();
					_delay_ms(100);
				}
				if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
				{
					Left();
					_delay_ms(100);
				}
				if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
				{
					Forward();
					_delay_ms(100);
				}
			}
			}			
else
{

			Right();
			_delay_ms(750);
			scan();
			while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
			{
				scan();
				if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
				{
					Right();
					_delay_ms(100);
				}
				if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
				{
					Left();
					_delay_ms(100);
				}
				if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
				{
					Forward();
					_delay_ms(100);
				}
			}
}

			
			Stop();
				_delay_ms(1000);
				pos = pos-1;
				scan();
						d=3;	
		}

		else if(c[x]==dw)
		{
			if(d==1)
			{
				
				Left();
				_delay_ms(950);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos+7;
				scan();
			}
			else if(d==3)
			{
				
				Right();
				_delay_ms(950);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos+7;
				scan();
				
			}
else if(d==0)
{
Right();
				_delay_ms(2000);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos+7;
				scan();
}
			else 
			{
				
				Forward();
				_delay_ms(750);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos+7;
				scan();				
			}
			d=2;
		}

		else if (c[x]==u)
		{
			if(d==1)
			{
				
				Right();
				_delay_ms(950);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos-7;
				scan();
			}
			else if(d==3)
			{
				
				Left();
				_delay_ms(950);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos+7;
				scan();
			}
else if(d==2)
{
Left();
				_delay_ms(2000);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos-7;
				scan();
}
			else
			{
				
				Forward();
				_delay_ms(750);
				scan();
				while(!(ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR>35))
				{
					scan();
					if((ADC_valueC<35 && ADC_valueL< 35 && ADC_valueR>35)||(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR>35))
					{
						Right();
						_delay_ms(100);
					}
					if((ADC_valueC>35 && ADC_valueL> 35 && ADC_valueR<35)||(ADC_valueC<35 && ADC_valueL> 35 && ADC_valueR<35))
					{
						Left();
						_delay_ms(100);
					}
					if(ADC_valueC>35 && ADC_valueL< 35 && ADC_valueR<35)
					{
						Forward();
						_delay_ms(100);
					}
				}
				Stop();
				_delay_ms(1000);
				pos = pos-7;
				scan();
			}
			d=0;
		}
		

	}
y78=d;
	present=pos;
	for(i=0;i<49;i++)
	{
		c[i]=99;
		
	}
	lcd_print(1,10,present,2);

}


void adc_pin_Config(void)
{
	
	DDRF=0x00;
	PORTF=0x00;
	DDRK=0X00;
	PORTK=0X00;
}	
void scan(void)
{
	print_sensor(1,1,1);
	print_sensor(1,5,2);
	print_sensor(1,9,3);
	
}

void init_devices(void)
{
	cli();
	port_init();
	adc_init();
	timer5_init();
	usart_init();
	sei();
	
}






int main(void)
{
	lcd_port_config();
	lcd_set_4bit();
	lcd_init();
init_devices();
	
	for(loc2=0;loc2<10;loc2++)
	{
		spark_r[loc2]=99;
		spark_b[loc2]=99;
		if (loc2<3)
		{
			r30[loc2]=99;
		}			
		
	}
	
//path_finder(9,34,r30);
//movement_for_all();
_delay_ms(1000);
loc2=0;
	while(1)
	{
	
	lcd_print(1,10,loc3,2);
			
			for(w=0;w<4;w++)
			{
				lcd_print(2,11,spark_b[w],4);
				lcd_print(2,1,spark_r[w],4);
				
				_delay_ms(1000);
			}
		
			
			lcd_print(1,5,flag,2);
			_delay_ms(1000);
		if(flag==1)
		{
			
			lcd_print(1,5,flag,2);
				
				for(w=0;w<4;w++)
				{
					lcd_print(1,5,flag,2);
					lcd_print(1,6,spark_r[w],4);
					lcd_print(1,13,j,2);
					
					_delay_ms(1000);
				}
			computational_stuff();
		
			lcd_wr_command(0x01);
				if(flag==2)
				{
					for(i=0;(i<6 && spark_r[i]!=99);i++)
					{
					
					uart_transmit(spark_r[i]+100);
				
					flag=0;
				}
				}			
		}
		 if(flag2==1)
		{
			flag=4;
			computational_stuff();
			if(flag2==2)
			{lcd_wr_command(0x01);
				lcd_print(1,1,flag2,2);
				
				for(i=0;(i<6 && spark_b[i]!=99);i++)
				{
					lcd_print(2,2,flag2,2);
					
					uart_transmit(spark_b[i]+100);
				
					flag=0;
					flag2=0;
					
				}
			}
			
		}
		
	}
}
void computational_stuff(void )
{int quad[10],small,temp,quad2[10];

	for(temp=0;temp<6;temp++)
	{
		
		quad[temp]=99;
		quad2[temp]=99;
	}
	if(flag==4)
	{
		
		for(i=0;i<6;i++)
		spark_r[i]=spark_b[i];
		
		loc2=loc3;
	}
	else
	{
		loc2=loc1;
	}
	for(i=0;spark_r[i]!=99;i++)
	{
		
		quad[i]= node_receiving(spark_r[i]);
		
		
	}
	
	for(i=0;(i<6 && (quad[i]!=99));i++)
	{
		quad2[i]=99;
		quad2[i]=quad[i];
	}
	for(i=0;(i<6 && (quad2[i]!=99));i++)
	{
		for(j=i+1;(j<6 && (quad2[j]!=99));j++)
		{
			
			if(quad2[i]>quad2[j])
			{
				temp=quad2[i];
				quad2[i]=quad2[j];
				quad2[j]=temp;
			}
		}
	}
	for(i=0;i<6 && (quad2[i]!=99);i++)
	{
		for(j=i+1;(j<6 && (quad[j]!=99));j++)
		{
			if(quad2[i]==quad[j])
			{small=spark_r[i];
				spark_r[i]=spark_r[j];
				spark_r[j]=small;
				break;
			}
		}
	}

	if(flag==4)
	{
		flag=0;
		flag2=2;
		for(i=0;i<6;i++)
		{
			spark_b[i]=spark_r[i];
		}			
	}
	else
	{
	flag=2;
	}	
}


int  node_receiving(int w0)
{
	int u1,u2,u3;
	
	
	
	
		u1=w0/6;//to filter from box to coordiantes
		u2=w0%6;
		u3=u1*7+u2;//filter from coordinates to node no
		
		
		
		
		return (cost_cal(u3));
	


}


int cost_cal(int t8)
{
	int p10[4],c=0;
	int g10[4],i;
	int x10[4];
	
	p10[0]=t8;
	
	g10[0]=path_finder(p10[0],loc2,r30);
	p10[1]=t8+7;
	
	g10[1]=path_finder(p10[1],loc2,r30);
	p10[2]=t8+8;

	g10[2]=path_finder(p10[2],loc2,r30);
	p10[3]=t8+1;
	
	g10[3]=path_finder(p10[3],loc2,r30);
	x10[1]=g10[1];
	x10[2]=g10[2];
	x10[3]=g10[3];
	x10[0]=g10[0];
	while((x10[1]!=0)&&(x10[2]!=0)&&(x10[3]!=0 )&& (x10[0]!=0))
	{
		x10[1]=x10[1]-1;
		x10[2]=x10[2]-1;
		x10[3]=x10[3]-1;
		x10[0]--;
		c++;
		
	}
	for(i=0;i<4;i++)
	{
		if(g10[i]==c)
		{
			break;
		}
	}
	
	c=	path_finder(p10[i],loc2,r30);
	return c;
}

	