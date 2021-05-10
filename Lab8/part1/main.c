/*
 * esoud001_lab8.c
 *
 * Created: 5/6/2019 1:31:40 AM
 * Author : admin
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0xFF;
	DDRB = 0xFF; PORTB = 0xFF;
	
	ADC_init();
	

	
    /* Replace with your application code */
    while (1) 
    {
		unsigned short my_short = ADC;
		PORTC = 0xFF & my_short;
		PORTB = my_short >> 8;
    }
}

