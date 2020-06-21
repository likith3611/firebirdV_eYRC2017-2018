/*/*
 * color_sensor_code_for_spark5.c
 *
 * Created: 16-01-2018 18:07:15
 *  Author: liki
 */
 #define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> //included to support power function
#include "lcd.c"
volatile unsigned long int pulse = 0;
volatile unsigned long int red;       // variable to store the pulse count when read_red function is called
volatile unsigned long int blue; 
volatile unsigned long int green; 
unsigned long int d=0;
ISR(INT1_vect)
{
	pulse++; 
}

void interrupt_enable(void)
{
	cli();
	
	 DDRD  = DDRD & 0b11110111;//Set the direction of the PORTD 3 pin as input  PORT
	PORTD = PORTD | 0b00001000;
	GICR = 1<<INT1;
	MCUCR |= 0b00001000;
	MCUCR &= 0b11111011;
	sei();
}

void enable_red_filter()
{
	PORTB &= 0b11100111;	
	//PORTB |= 0b10000000;//setting the frequency to 20%
	//PORTB &= 0b10111111;
}
void enable_green_filter()
{

	PORTB |= 0b00011000;

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
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
	lcd_string("Blue Pulses"); // Display "Red Pulses" on LCD
	lcd_print(2,1,blue,5);  //Print the count on second row
	_delay_ms(1000);	// Display for 1000ms or 1 second
	lcd_wr_command(0x01); //Clear the LCD
}

void red_read()
{
	enable_red_filter();
	pulse= 0;
	_delay_ms(200);
	red=pulse;
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
	lcd_string("Red Pulses"); // Display "Red Pulses" on LCD
	lcd_print(2,1,red,5);  //Print the count on second row
	_delay_ms(1000);	// Display for 1000ms or 1 second
	lcd_wr_command(0x01); //Clear the LCD
}


void green_read()
{
	enable_green_filter();
	pulse= 0;
	_delay_ms(200);
	green=pulse;
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
	lcd_string("Green Pulses"); // Display "Red Pulses" on LCD
	lcd_print(2,1,green,5);  //Print the count on second row
	_delay_ms(1000);	// Display for 1000ms or 1 second
	lcd_wr_command(0x01); //Clear the LCD
}
void color_sensor_scaling(void)
{
	DDRB |= 0b00011011;
	//setting the digital pins for output
	PORTB |= 0b00000001;//setting the frequency to 20%
	PORTB &= 0b11111101;	
	
	
}
void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7;    //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80;  // all the LCD pins are set to logic 0 except PORTC 7
}

void init_devices()
{
	 interrupt_enable();
	 color_sensor_scaling();
	lcd_port_config();
	
}
int main(void)
{ 
	init_devices();
	lcd_set_4bit();
	lcd_init();
    while(1)
    {//lcd_print(2,7,d,4);
//	blue_read();
//_delay_ms(250);
		//green_read();
		//_delay_ms(600);
		red_read();
//	_delay_ms(600);
		//blue_read();
		_delay_ms(600);
		
        //TODO:: Please write your application code 
    }
}