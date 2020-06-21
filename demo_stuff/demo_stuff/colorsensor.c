/*
 * color_sensor_code_for_spark5.c
 *
 * Created: 16-01-2018 18:07:15
 *  Author: liki
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> //included to support power function

volatile unsigned long int pulse = 0;
volatile unsigned long int red;       // variable to store the pulse count when read_red function is called
volatile unsigned long int blue; 
volatile unsigned long int green; 
 volatile int j;
ISR(INT2_vect)
{
	pulse++; //increment on receiving pulse from the color sensor
}

void interrupt_enable(void)
{
	cli();
	DDRB = DDRB & 0b11111011;//this is for setting the interrupt pin to receive inputs the 
	PORTB = PORTB | 0b00000100;//enabling pull up resistor
	
	GICR = 1<<INT2;
	MCUCSR &=  0b10111111;
	sei();
}
void interrupt_disable(void)
{
	GICR &=0b11011111; 

}

void enable_red_filter()
	{
		PORTB &= 0b11100111;
	//PORTB |= 0b10000000;//setting the frequency to 20%
	//PORTB &= 0b10111111;
}
void enable_green_filter()
{

	PORTB |= 0b00010000;
	PORTB |= 0b00001000;
	//PORTB |= 0b10000000;//setting the frequency to 20%
//	PORTB &= 0b00111111;
}

void enable_blue_filter()
{
	PORTB |= 0b00010000;
	PORTB &= 0b11110111;
	//PORTB |= 0b10000000;//setting the frequency to 20%
	//PORTB &= 0b00111111;
}

void blue_read()
{
	enable_blue_filter();
	pulse= 0;

	
	_delay_ms(200);
	blue=pulse;
	clear_filter();
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
//	lcd_string("Blue Pulses"); // Display "Red Pulses" on LCD
	//lcd_print(2,1,blue,5);  //Print the count on second row
	_delay_ms(1000);	// Display for 1000ms or 1 second
	lcd_wr_command(0x01); //Clear the LCD
	
}

void red_read()
{
	enable_red_filter();
	pulse= 0;
	
	_delay_ms(200);
	red=pulse;
	clear_filter();
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
//	lcd_string("Red Pulses"); // Display "Red Pulses" on LCD
	//lcd_print(2,1,red,5);  //Print the count on second row
	_delay_ms(1000);	// Display for 1000ms or 1 second
	lcd_wr_command(0x01); //Clear the LCD

}


void green_read()
{
	enable_green_filter();
	pulse= 0;
	j=1;
	_delay_ms(200);
	green=pulse;
	clear_filter();
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
	//lcd_string("Green Pulses"); // Display "Red Pulses" on LCD
//	lcd_print(2,1,green,5);  //Print the count on second row
	_delay_ms(1000);	// Display for 1000ms or 1 second
	lcd_wr_command(0x01); //Clear the LCD
		
	_delay_ms(250);
}
void color_sensor_scaling(void)
{
	DDRB |= 0b00011011;
	//setting the digital pins for output
	PORTB |= 0b00000001;//setting the frequency to 20%
	PORTB &= 0b11111101;	
	
	
}
void clear_filter(void)
{
	PORTB |= 0b00001000;
	PORTB &= 0b11101111;
	
}

