/*	
 *	Esa Soudbakhsh
 *	Lab Section: 027
 *	Assignment: Lab # 3 Exercise # 4 
 *	Exercise Description: 
 *	Assigns the higher nibble to port b and then assigns the lower nibble to port c
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0x31;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
    /* Replace with your application code */
    unsigned char tmpa = PORTA;
	unsigned char tmpb = PORTB;
	unsigned char tmpc = PORTC;
	
	while (1) 
    {
		tmpb = (tmpa & 0xF0) >> 4;
		tmpc = (tmpa & 0x0F) << 4;
		
		PORTB = PORTB | tmpb;
		PORTC = PORTC | tmpc;
		asm("break");
	}
}

