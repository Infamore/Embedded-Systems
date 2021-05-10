/*	Esa Soudbakhsh
 *	
 *	Lab Section: 027 
 *	Assignment: Lab # 4 Exercise # 2 
 *	Exercise Description: 
 *	adds and subtracts bits from PORTC with limits at 0x09 and 0x00. If reset is high then all bits are 0x00.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

typedef enum {
	donothing,
	adder,
	subtractor,
	reset,
}numstate;

int main(void)
{
	DDRA = 0xFC; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x07;
	
	
	
    numstate state = donothing;
	 
	
	while (1) 
    {
		switch (state) 
		{
			
		case donothing: 
		if(PINA == 0x01) state = adder; 
		
		else if(PINA == 0x02) state = subtractor; 
		
		else if (PINA == 0x03) state = reset;
		
		break;
		//
		case adder: 
		
		if(PINA == 0x02) state = subtractor; 
		
		else if (PINA == 0x03) state = reset; 
		
		PORTC = PORTC + 0x01;
		
		if (PORTC >= 0x09) PORTC = 0x09;
		
		PORTA = 0x00;
		
		state = donothing;
		
		break;
		//
		case subtractor: 
		
		if(PINA == 0x01) state = adder; 
		
		else if (PINA == 0x03) state = reset; 
		
		PORTC = PORTC - 0x01;
		
		if (PORTC <= 0x00 || PORTC == 0xFF) PORTC = 0x00;
		
		PORTA = 0x00;
		
		state = donothing;
		
		break;
		//
		case reset: 
		
		if(PINA == 0x01) state = adder; 
		
		else if (PINA == 0x02) state = subtractor; 
		
		PORTC = 0x00;
		
		PORTA = 0x00;
		
		state = donothing;
		
		break;
		
		default: state = donothing;
		
		}
		
	
	
	}
}

