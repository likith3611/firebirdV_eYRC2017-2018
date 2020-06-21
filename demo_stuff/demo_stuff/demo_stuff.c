/*
 * demo_stuff.c
 *
 * Created: 19-01-2018 20:00:08
 *  Author: liki
 */ 


#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.c"
#include "ultracode.c"
#include "colorsensor.c"
#include "xbee_comm.c"
#include "servo_code.c"


 int i=0;
#define		THRESHOLD		25        // set the pots such that all three sensor
// calibrated to show its min value on LCD.
// i.e on LCD Sensor values are betwn 0 To 25
// on white line
#define		VELOCITY_MAX	50
#define		VELOCITY_MIN	35
#define 	VELOCITY_LOW	0

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
int x,y,i;

//Function to configure LCD port
void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7;    //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80;  // all the LCD pins are set to logic 0 except PORTC 7
}

//ADC pin configuration
void adc_pin_config (void)
{
	DDRA = 0x00;   //set PORTF direction as input
	PORTA = 0x00;  //set PORTF pins floating
}

//Function to configure ports to enable robot's motion
void motion_pin_config (void)
{
	DDRB = DDRB | 0x0F;    //set direction of the PORTB3 to PORTB0 pins as output
	PORTB = PORTB & 0xF0;  //set initial value of the PORTB3 to PORTB0 pins to logic 0
	DDRD = DDRD | 0x30;    //Setting PD5 and PD4 pins as output for PWM generation
	PORTD = PORTD | 0x30;  //PD5 and PD4 pins are for velocity control using PWM
}

//Function to Initialize PORTS
void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
}

//TIMER1 initialize - prescale:64
// WGM: 5) PWM 8bit fast, TOP=0x00FF
// desired value: 450Hz
// actual value: 450.000Hz (0.0%)
void timer1_init(void)
{
	TCCR1B = 0x00; //stop
	TCNT1H = 0xFF; //setup
	TCNT1L = 0x01;
	OCR1AH = 0x00;
	OCR1AL = 0xFF;
	OCR1BH = 0x00;
	OCR1BL = 0xFF;
	ICR1H  = 0x00;
	ICR1L  = 0xFF;
	TCCR1A = 0xA1;
	TCCR1B = 0x0D; //start Timer
}


//Function to Initialize ADC
void adc_init()
{
	ADCSRA = 0x00;
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//This Function accepts the Channel Number and returns the corresponding Analog Value
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;	//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10;      //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	return a;
}

// This Function prints the Analog Value Of Corresponding Channel No. at required Row
// and Coloumn Location.
void print_sensor(char row, char coloumn,unsigned char channel)
{
	ADC_Value = ADC_Conversion(channel);
	 // uart_transmit(ADC_Value);
	lcd_print(row, coloumn, ADC_Value, 3);
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

void forward (void) //both wheels forward
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

void init_devices (void)
{
	cli();          //Clears the global interrupts
	port_init();
	ultra_port_init();
	
	xbee_init(); //u know what 
ultra_init_devices();
	timer1_init();
	adc_init();
	servo_init();
	sei();          //Enables the global interrupts
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
     		  servo_drop();
			  velocity(50,45);
			  forward();
			  _delay_ms(1000);
	i=1;
	
}
int main(void)
{
	init_devices();
	

	lcd_set_4bit();
	lcd_init();

	while (1)
	{
	ultrasonic_trigger();
		_delay_ms(150);
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(4);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(5);
	 lcd_wr_command(0x01);
	// uart_transmit(2);    
	 Right_ultrasonic_Sensor = ADC_Conversion(7) * 2;
		//lcd_print(2,9,Right_ultrasonic_Sensor,3);		
		print_sensor(1,1,3);		//Prints value of White Line Sensor Left
		print_sensor(1,5,4);		//Prints value of White Line Sensor Center
	print_sensor(1,9,5);		//Prints value of White Line Sensor Right
		if((Left_white_line<13)&&(Center_white_line>11)&&(Right_white_line<13))
		{	
			velocity(50,45);
			forward();
			
			if(Right_ultrasonic_Sensor>180 && i==0)
			call_color();
		}
		else if((Left_white_line>11)&&(Right_white_line<12))
		{
			velocity(30,0);
			soft_left_2();
		if(Right_ultrasonic_Sensor>180 && i==0)
		call_color();
			
		}
		else if((Left_white_line<12)&&(Right_white_line>11))
		{
			velocity(0,30);
			soft_right_2();
		if(Right_ultrasonic_Sensor>180 && i==0)
		call_color();
		}
		else if((Left_white_line>15)&&(Right_white_line>15))
		{ i=0;
			hard_stop();
		}
		
		
	}
}