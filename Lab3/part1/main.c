/*	
 *	Esa Soudbakhsh (esoud001@ucr.edu)
 *	Lab Section: 027
 *	Assignment: Lab # 3 Exercise # 1
 *	Exercise Description: 
 *	Calculates the bits on ports A and B then assigns the total to port C using the GetBit function
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Replace with your application code */
    int count = 0;
	while (1) 
    {
		for (int i = 0; i < 8; i++)
			if (GetBit(PORTA,i))
				count++;
		for (int i = 0; i < 8; i++)
			if (GetBit(PORTB,i))
				count++;
		
		PORTC = count;
		asm("break");
	}
}

