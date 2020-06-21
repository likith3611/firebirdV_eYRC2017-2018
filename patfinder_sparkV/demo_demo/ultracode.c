



unsigned char Right_ultrasonic_Sensor;


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

