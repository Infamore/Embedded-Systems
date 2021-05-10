/*
 * esoud001_part4_lab8.c
 *
 * Created: 5/6/2019 9:15:01 PM
 * Author : admin
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger
	// whenever the previous conversion completes.
}


unsigned short MAX = 0x0C7;
unsigned short tmp = 0x00;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	ADC_init();
	
	while(1)
	{
		tmp = ADC;
		if (tmp >= (MAX / 8))
		{
			PORTC = 0x01;
		}
		if (tmp >= 2*(MAX / 8))
		{
			PORTC = (0x01 << 1) | PORTC;
		}
		if (tmp >= 3*(MAX / 8))
		{
			PORTC = (0x01 << 2) | PORTC;
		}
		if (tmp >= 4*(MAX / 8))
		{
			PORTC = (0x01 << 3) | PORTC;
		}
		if (tmp >= 5*(MAX / 8))
		{
			PORTC = (0x01 << 4) | PORTC;
		}
		if (tmp >= 6*(MAX / 8))
		{
			PORTC = (0x01 << 5) | PORTC;
		}
		if (tmp >= 7*(MAX / 8))
		{
			PORTC = (0x01 << 6) | PORTC;
		}
		if (tmp >= 8*(MAX / 8))
		{
			PORTC = (0x01 << 7) | PORTC;
		}
	}
}
