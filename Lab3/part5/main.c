/*	
 *	Esa Soudbakhsh (esoud001@ucr.edu)
 *	Lab Section: 027
 *	Assignment: Lab # 3 Exercise # 5 
 *	Exercise Description: 
 *	manipulate a 9 bit value and change PORTB accordingly	
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFE; PORTB = 0x01;
   
	unsigned int tmp = (PORTD << 1) + PORTB;
	
	while (1) 
    {
		if(tmp >= 0x46)
			PORTB = SetBit(PORTB,1,1); //airbag enabled
		else if (tmp >= 0x05 && tmp < 0x46)
			PORTB = SetBit(PORTB,2,1); //airbag disabled
	}
	
	asm("break");
	
}

