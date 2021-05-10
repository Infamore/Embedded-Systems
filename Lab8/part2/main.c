/*
 * esoud001_part3_lab8.c
 *
 * Created: 5/6/2019 8:51:15 PM
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
	DDRB = 0xFF; PORTB = 0x00;
	ADC_init();
	
	while(1)
	{
		tmp = ADC;
		if(tmp >= (MAX / 2))
		{
			PORTB = 0x01;
		}
		else
		{
			PORTB = 0x00;
		}
	}
}
