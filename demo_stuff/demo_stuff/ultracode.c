/********************************************************************************
 Platform: SPARK V
 Experiment: 7B_ADC_URSWLBAT_Sensor_display_On_LCD
 Written by: Vinod Desai, NEX Robotics Pvt. Ltd.
 Edited By: Sachitanand Malewar, NEX Robotics Pvt. Ltd.
 Last Modification: 22nd September 2010
 AVR Studio Version 4.17, Build 666
 
 Concepts covered:  ADC, LCD. SENSOR interfacing

 In this experiment ADC captures data from the analog sensors such as Ultrasonic 
 Range Sensors, Whiteline sensors and Battery voltage and displayed it on the LCD.

 Ultrasonic sensors are used to detect proximity of any obstacles in the very short 
 range to long range from the robot. We are using EZ series of Ultrasonic Range Sensors 
 from Maxbotix. It can sense obsticals from from 6-inches to 254-inches with 1-inch 
 resolution. Output analog voltage has scaling factor of (Vcc/512) per inch. A supply 
 of 5V yields ~9.8mV/inch.

 White line sensors are used for detecting white line on the ground surface. White 
 lines are used to give robot sense of localization. White line sensor consists of 
 IR photo diode for line sensing and IRLED for illumination. White line cause increace 
 in reflected light resulting fall in voltage across the photodiode. 


 There are Three IR Proximity sensor, Three White Line Sensors on the SPARKV robot. 
 There are sockets for four Ultrasonic Range Sensors from Maxbotix. Due to limted 
 number of ADC channels either we can use IR Proximity sensors or Ultrasonic Range Sensors
 by jumper settings of J2,J3,J4 on the Robot. In this expirement three Ultrasonic Range
 Sensors are connected. You can also use less number of Ultrasonic Range Sensors. To do
 that you need to short some pads. For correct jumper setting and pad settings refer 
 to robot's hardware manual.

 
 IR Proximity Sensor interfacing is covered in "7A_ADC_IRWLBAT_Sensor_display_On_LCD"
 experiment.
  
 ADC Connection:
 			  ACD CH.	PORT	Sensor
			  0			PA0		Ultrasonic analog sensor Left
			  1			PA1		Ultrasonic analog sensor Center
			  2			PA2		Ultrasonic analog sensor Right
			  3			PA3		White line sensor Left
			  4			PA4		White line sensor Center
			  5			PA5		White line sensor Right
			  6			PA6		Battery Voltage
			  7         PA7     4th Ultrasonic Range Sensor Analog output is connected from 
			                    external connector (Refer Ultrasonic range sensor interfacing 
								in Manual)

 LCD Connections:
 			  LCD	  Microcontroller Pins
 			  RS  --> PC0
			  RW  --> PC1
			  EN  --> PC2
			  DB7 --> PC7
			  DB6 --> PC6
			  DB5 --> PC5
			  DB4 --> PC4


 Jumper settings:
 There are sockets for four Ultrasonic Range Sensors from Maxbotix. Due to 
 limted number of ADC channels either we can use IR Proximity sensors or 
 Ultrasonic Range Sensors by jumper settings of J2,J3,J4 on the Robot. For 
 correct jumper setting refer to robot's hardware manual.

 LCD Display interpretation:
 ***************************************************************************************************
 *ULTRASONIC SENSOR LEFT	ULTRASONIC SENSOR CENTER	ULTRASONIC SENSOR RIGHT                    *
 *LEFT WL SENSOR		    CENTER WL SENSOR	        RIGHT WL SENSOR		       BATTERY VOLTAGE *
 ***************************************************************************************************
 
 For more detail on hardware and theory refer the hardware manual. 

 Note: 
 
 1. Make sure that in the configuration options following settings are 
 	done for proper operation of the code

 	Microcontroller: atmega16
 	Frequency: 7372800
 	Optimization: -O0 (For more information read section: Selecting proper optimization
	              options below figure 4.22 in the hardware manual)

 2. Make sure that you copy the lcd.c file in your folder

*********************************************************************************/

/********************************************************************************

   Copyright (c) 2010, NEX Robotics Pvt. Ltd.                       -*- c -*-
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   * Source code can be used for academic purpose. 
	 For commercial use permission form the author needs to be taken.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 

  Software released under Creative Commence cc by-nc-sa licence.
  For legal information refer to: 
  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

********************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function



unsigned char Right_ultrasonic_Sensor;

//Function to configure LCD po

//ADC pin configuration


void ultrsonic_trigger_config(void)
{
 DDRD = DDRD | 0x40;   //all the LCD pin's direction set as output
 PORTD = PORTD & 0x00; // all the LCD pins are set to logic 0 except PORTC 7
}

//Function to Initialize PORTs
void ultra_port_init()
{

		
 ultrsonic_trigger_config();
}



// This Function prints the Analog Value Of Corresponding Channel No. at required Row
// and Coloumn Location. 

// Ultrasonic sensor are connected in chaining mode. This function rise a 
// trigger pulse of >20usec to command ringing.     
void ultrasonic_trigger(void)                 
{
 PORTD = PORTD | 0x40;  // make high the Trigger input for Ultrasonic sensor
 _delay_us(50);         // Wait for >20usec
 PORTD = PORTD & 0xBF;  // make low the Trigger input for Ultrasonic sensor
}

void ultra_init_devices (void)
{
 cli();           //Clears the global interrupts,lcd_print(1,1,Right_ultrasonic_Sensor,3);, Right_ultrasonic_Sensor = ADC_Conversion(7) * 2;        // In Inches
 
 
 ultra_port_init();
 adc_init();
 sei();           //Enables the global interrupts
}

