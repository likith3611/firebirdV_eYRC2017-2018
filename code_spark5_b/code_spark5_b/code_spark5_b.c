/*
 * code_spark5_b.c
 *
 * Created: 22-02-2018 22:56:34
 *  Author: liki
 */ 




#include <avr/io.h>
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.c"
#include "ultracode.c"
#include "colorsensor.c"
#include "spamovmentin.c"

#include "path_finder_spark5.c"


int present;

extern int  d10;
int l=0,w100=0,w;
extern int c[49];
int y78=1;
	int t89[49];
extern int d10;
volatile int r20[6],r30[3];
volatile int flag=0,flag2=0;

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
volatile int q90=0,q80=0;
int f90=0;
int w13;

SIGNAL(SIG_USART_RECV)
{
	
	unsigned int r10;
	r10=UDR-'0';
	

	
	if(r10>=0 && r10<50 )
	{
		r20[q90]=r10-100;
		

		q90++;
		_delay_ms(100);
	}
	if (r10>=100 && r10 <150)
	{
		r30[q80]=r10;
		q80++;
		_delay_ms(100);
	}
	
	
	
}	
	
void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
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
{
	
	UCSRB=0x00;
	UCSRA = 0x00;
	UCSRC = 0x86;
	UBRRL = 0x2F; //set baud rate lo
	UBRRH = 0x00; //set baud rate hi
	UCSRB = 0x98;
	

	
}
void uart_transmit_char(unsigned char data)
{
	while (!(UCSRA & (1<<UDRE)));
	UDR=data;
}	
void uart_transmit(unsigned int data)
{lcd_wr_command(0x01);
	
	lcd_print(1,2,data,4);
	while (!(UCSRA & (1<<UDRE)));
	UDR=data+'0';
	_delay_ms(1000);

}
void init_xbee()
{cli();

	init_uart();
	sei();
}



void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7;
	PORTC = PORTC & 0x80;
}


void adc_pin_config (void)
{
	DDRA = 0x00;
	PORTA = 0x00;

}
void motion_pin_config (void)
{
	DDRB = DDRB | 0x0F;
	PORTB = PORTB & 0xF0;
	DDRD = DDRD | 0x30;
	PORTD = PORTD | 0x30;
}


void port_init()
{cli();
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
	
	sei();
}


void timer1_init(void)
{
	TCCR1B = 0x00;
	TCNT1H = 0xFF;
	TCNT1L = 0x01;
	OCR1AH = 0x00;
	OCR1AL = 0xFF;
	OCR1BH = 0x00;
	OCR1BL = 0xFF;
	ICR1H  = 0x00;
	ICR1L  = 0xFF;
	TCCR1A = 0xA1;
	TCCR1B = 0x0D;
}


void adc_init()
{
	ADCSRA = 0x00;
	ADMUX = 0x20;
	ACSR = 0x80;
	ADCSRA = 0x86;
}


unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;
	while((ADCSRA&0x10)==0);
	a=ADCH;
	ADCSRA = ADCSRA|0x10;
	return a;
}



void print_sensor(char row, char coloumn,unsigned char channel)
{
	ADC_Value = ADC_Conversion(channel);
	
	lcd_print(row, coloumn, ADC_Value, 3);
}


void motion_set (unsigned char Direction)
{
	unsigned char PortBRestore = 0;

	Direction &= 0x0F; 			// removing upper nibbel as it is not needed
	PortBRestore = PORTB; 			// reading the PORTB's original status
	PortBRestore &= 0xF0; 			// setting lower direction nibbel to 0
	PortBRestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTB status
	PORTB = PortBRestore; 			// setting the command to the port
}

void forward (void)
{
	motion_set(0x06);
}

void back (void) //both wheels backward
{
	motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
}

void hard_stop (void)    //hard stop (Stop suddenly)
{
	motion_set(0x00);
}

void soft_stop (void)    //hard stop (Stop slowly)
{
	motion_set(0x0F);
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR1AH = 0x00;
	OCR1AL = left_motor;
	OCR1BH = 0x00;
	OCR1BL = right_motor;
}
void init_acd_motion(void)
{
	cli();
	port_init();
	timer1_init();
	adc_init();
	ultra_init_devices();
	sei();
}
void scan(void)
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(4);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(5);
}
forward_all()
{
	velocity(90,90);
	forward();
	_delay_ms(550);
	scan();
	normalmovement();
	hard_stop();
	_delay_ms(700);
	scan();
}
reverse_all()
{
	velocity(120,120);
	right();
	_delay_ms(950);
	scan();
	normalmovement();
	hard_stop();
	_delay_ms(750);
	
	scan();
}
left_all()
{//velocity(50,50);
	//forward();
//	_delay_ms(150);
			velocity(0,70);
			soft_left();
			_delay_ms(2000);
			scan();
			
			normalmovement();
			hard_stop();
			_delay_ms(750);
			
			scan();
}
right_all()
{//velocity(50,50);
	//forward();
	//_delay_ms(200);
	velocity(70,0);
	
	soft_right();
	_delay_ms(2000);
	
	scan();
	normalmovement();
	hard_stop();
	_delay_ms(750);
	
	scan();
}
void normalmovement(void)
{
	
	while(!(Center_white_line >13 && Right_white_line >13 && Left_white_line >13))
	{
		ultrasonic_trigger();
		Right_ultrasonic_Sensor = ADC_Conversion(7) * 2;
		if((Left_white_line<13)&&(Center_white_line>13)&&(Right_white_line<13))
		{
			velocity(55,50);
			forward();
			
		
			
		}
		else if((Left_white_line>12)&&(Right_white_line<12))
		{
			velocity(30,0);
			soft_left_2();
			
			
			
			
		}
		else if((Left_white_line<12)&&(Right_white_line>12))
		{
			velocity(0,30);
			soft_right_2();
			
			
		}
		scan();
		
	}
	


}
void movement_for_all(void)
{
	int start=c[0];
	int position=start;
	int x;
	int lenght=0;
	int r,u,dw;
	int d=y78;    //1-right,0-up,2-down,3-left

	for(x=0;x<49;x++)
	{
		if(c[x] !=99 )
		{
			lenght++;
		}
		else
		{
			break;
		}
		

	}
	lcd_print(2,10,c[lenght-1],2);
	for(x=0;x<=lenght;x++)
	{
		
		r=position+1;
		l=position-1;
		u=position-7;
		dw=position+7;
		
		
		lcd_print(2,7,position,2);
		
		
		if(r==c[x])
		{
			if(d==3)
			{
				reverse_all();
			}
			else if(d==0)
			{right_all();
			}
			else if(d==2)
			{
		left_all();
			}
			else
			{
				forward_all();
			}
			d=1;
			position++;
			
		}
		else if(l==c[x])
		{
			
			if(d==1)
			{reverse_all();
			}
			else if(d==0)
			{left_all();
			}
			else if(d==2)
			{right_all();
			}
			else
			{
				forward_all();
				
			}
			
			d=3;
			position--;
			
		}
		else if(u==c[x])
		{
			if(d==2)
			{reverse_all();
			}
			else if(d==1)
			{left_all();
			}
			else if(d==3)
			{
				
				right_all();
			}
			else
			{
				forward_all();
			}
			d=0;
			position-=7;
		}
		else if(dw==c[x])
		{
			
			if(d==0)
			{reverse_all();
			}
			else if(d==1)
			{right_all();
			}
			else if(d==3)
			{
				left_all();
			}
			else
			{

				forward_all();
				
			}
			d=2;
			position+=7;
		}
		
		
	}
	y78=d;
	present=position;
	for(i=0;i<49;i++)
	{
		c[i]=99;
		
	}
	lcd_print(1,10,present,2);
}


int main(void)
{init_acd_motion();
	lcd_set_4bit();
	lcd_init();
	init_xbee();
	buzzer_pin_config();
	for(w=0;w<7;w++)
	{
		if(w<3)
	{	r30[w]=99;
	}
	r20[w]=99;		

	}
	for(w=0;w<49;w++)
	{
		c[w]=99;
	}
	
//path_finder(9,25,r30);
//obstacleavoiding();
		
		spamovement();
		
			movement_for_all();
			
		uart_transmit(33);
    	uart_transmit(22);
		uart_transmit(12);
		uart_transmit(191);
	uart_transmit(209);
	
			_delay_ms(10000);
				
	while(1)
	{
		lcd_wr_command(0x01);	
		
			lcd_print(1,14,q90,1);
			
			for(w=0;w<3;w++)
			{
				lcd_print(2,12,r20[w],4);
				_delay_ms(1000);
			}
			if(q90>=3)
			{
				
				flag=0;
				
				
				lcd_wr_command(0x01);
				
				for(w=0;w<6 && r20[w]!=99;w++)
				{lcd_print(1,14,r20[w],2);
					
					node_receiving(r20[w]);
				movement_for_all();
					plant_reader();		
					r20[w]=99;
				}
				flag=5;
			}
			
	
		if(flag==5)
		{
			path_finder(13,present,r30);
		movement_for_all();
			break;
		}
		}
		
		
		
		


	
	}

int node_receiving(int w0)
{
	int u1,u2,u3;
	
	
		u1=w0/6;//to filter from box to coordiantes
		u2=w0%6;
		u3=u1*7+u2;//filter from coordinates to node no
		
		if(w100==1)
		{
			return u3;
		}
		else
		{
			
			
			cost_cal(u3);
		}
	
	


}
int cost_cal(int t8)
{
	
	int p10[4],c=0;
	int g10[4],i;
	int x10[4];
	p10[0]=t8;
	g10[0]=path_finder(p10[0],present,r30);
	
	p10[1]=t8+7;
	
	
	g10[1]=path_finder(p10[1],present,r30);
	p10[2]=t8+8;
	g10[2]=path_finder(p10[2],present,r30);
	p10[3]=t8+1;
	
	g10[3]=path_finder(p10[3],present,r30);
	x10[1]=g10[1];
	x10[2]=g10[2];
	x10[3]=g10[3];
	x10[0]=g10[0];
	while(x10[1]!=0&&x10[2]!=0&&x10[3]!=0 &&x10[0]!=0)
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

	c=	path_finder(p10[i],present,r30);
}
void plant_reader()
{w100=1;
	int h100=	node_receiving(r20[w]);
f90=1;
	if( (h100+1)==present)
	{
		c[0]=present;
		c[1]=present+7;
		
	}
	else if((h100+8)==present)
	{
		c[0]=present;
		c[1]=present-1;
	}
	else if((h100+7)==present)
	{
		c[0]=present;
		c[1]=present-7;
	}
	else
	{
		c[0]=present;
		c[1]=present+1;
		
	}
	w100=0;
	movement_for_all();
	f90=0;
	
}

 void call_color(void)
 {     PORTB =0x00;
	 velocity(0,0);    //disabling the motion pins
	 interrupt_enable();
	 color_sensor_scaling();
	 red_read();
	 _delay_ms(60);
	 green_read();
	 _delay_ms(60);
	 blue_read(60);
	 _delay_ms(60);
	 if(red>green)
	 {
		 if(red>blue)
		 lcd_string("red color");
		 else
		 lcd_string("blue_color");
	 }
	 else
	 {if(green>blue)
		 lcd_string("green color");
		 else
		 lcd_string("blue color");
		 
	 }
	 
	 interrupt_disable();
	 DDRB = DDRB | 0x0F;    //for again setting back the motion pins
	 PORTB = PORTB & 0xF0;
	 
	 velocity(50,45);
	 forward();
	 _delay_ms(1000);
	 i=1;
	 
 }
