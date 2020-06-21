/*
 * GccApplication4.c
 *
 * Created: 07-01-2018 11:25:19
 *  Author: liki
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "path_finder_spark5.c"
#include "spamovmentin.c"
#include "lcd.c"
volatile int loc1,loc2=0,flag=0,flag2=0;
int i=0,j=0,spark_r[10],spark_b[10];
extern int c[49];
volatile int tent[5];
volatile loc3;


void motion_pin_config (void)
{
	DDRB = DDRB | 0x0F;   //set direction of the PORTB3 to PORTB0 pins as output
	PORTB = PORTB & 0xF0; // set initial value of the PORTB3 to PORTB0 pins to logic 0
	DDRD = DDRD | 0x30;   //Setting PD4 and PD5 pins as output for PWM generation
	PORTD = PORTD | 0x30; //PD4 and PD5 pins are for velocity control using PWM
}

//Function to initialize ports
void port_init1()
{
	motion_pin_config();
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
	unsigned char PortBRestore = 0;

	Direction &= 0x0F; 			// removing upper nibbel as it is not needed
	PortBRestore = PORTB; 			// reading the PORTB's original status
	PortBRestore &= 0xF0; 			// setting lower direction nibbel to 0
	PortBRestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTB status
	PORTB = PortBRestore; 			// setting the command to the port
}

void forward (void)         //both wheels forward
{
	motion_set(0x06);
}

void back (void)            //both wheels backward
{
	motion_set(0x09);
}

void left (void)            //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void)           //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void soft_left (void)       //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void)      //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

void soft_left_2 (void)     //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void)    //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
}

void hard_stop (void)       //hard stop(stop suddenly)
{
	motion_set(0x00);
}

void soft_stop (void)       //soft stop(stops solowly)
{
	motion_set(0x0F);
}
//code for motion control ending



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


SIGNAL(SIG_USART_RECV)
{

int DATA=UDR-'0';
tent[loc2]= DATA;
loc2++;
lcd_print(1,10,DATA,4);
if(DATA>=100 && DATA<150)
{
	spark_r[i]=DATA;
	i++;
}


if(DATA>=250 && DATA <300)
{
	loc3=DATA-250;
}
else

{

	
		loc1=DATA-200;
		flag=1;
		
	
}


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
{lcd_print(1,2,data,4);
	lcd_print(2,2,data+'0',4);
	while (!(UCSRA & (1<<UDRE)));
	UDR=data+'0';

_delay_ms(1000);

}


void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7;
	PORTC = PORTC & 0x80;
}


void init_xbee()
{cli();

	init_uart();
	sei();
}


int main(void)
{init_xbee();
	lcd_port_config();
	lcd_set_4bit();
	lcd_init();

	
for(loc2=0;loc2<10;loc2++)
{
	spark_r[loc2]=99;
	spark_b[loc2]=99;
}
	_delay_ms(1000);
	uart_transmit(27);
	while(1)
	{		
		
			
		_delay_ms(1000);
	
		if(flag==1)
		{
			
			
			computational_stuff();
			if(flag==2)
			{lcd_wr_command(0x01);
				lcd_print(2,2,flag,2);
				for(i=0;(i<5 && spark_r[i]!=99);i++)
			{
				
				
				uart_transmit(spark_r[i]);
				buzzer_off();
				flag=0;
			
			}
			}
			
		}
		if(flag2==1)
		{
			computational_stuff();
			if(flag==2)
			{lcd_wr_command(0x01);
				lcd_print(2,2,flag,2);
				flag=4;
				for(i=0;(i<5 && spark_b[i]!=99);i++)
				{
					
					
					uart_transmit(spark_b[i]);
					buzzer_off();
					flag=0;
					
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
	{for(i=0;i<6;i++)
		spark_r[i]=spark_b[i];
		loc2=loc3;
	}
	for(i=0;spark_r[i]!=99;i++)
	{
		
		quad[i]= node_receiving(spark_r[i]);
		
		
	}
	
	for(i=0;(i<6 && (quad[i]!=99));i++)
	{quad2[i]=99;
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
		for(i=0;i<6;i++)
		spark_b[i]=spark_r[i];
	}
	
flag=2;
	}


int  node_receiving(int w0)
{
	int u1,u2,u3;
	
	
	if(w0>100 && w0<200)
	{
		w0-=100;
		u1=w0/6;//to filter from box to coordiantes
		u2=w0%6;
		u3=u1*7+u2;//filter from coordinates to node no
		
	
	
		
		return (cost_cal(u3));
	}


}



int cost_cal(int t8)
{
	int p10[4],c=0;
	int g10[4],i;
	int x10[4];
	
	p10[0]=t8;
	
	g10[0]=path_finder(p10[0],loc1);
	p10[1]=t8+7;
	
	g10[1]=path_finder(p10[1],loc1);
	p10[2]=t8+8;
	path_finder(p10[2],loc1);
	g10[2]=path_finder(p10[2],loc1);
	p10[3]=t8+1;
	
	g10[3]=path_finder(p10[3],loc1);
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
	
	c=	path_finder(p10[i],loc1);
	return c;
}







