/*	
 *	Esa Soudbakhsh (esoud001@ucr.edu)
 *	Lab Section: 027
 *	Assignment: Lab # 3 Exercise # 2
 *	Exercise Description: 
 *	Sets each bit according to the hexadecimal equivalents from 1-15
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
	/* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpa = 0x00;
	unsigned char tmpc = 0x00;
	
	
	//tmpc = tmpc | PORTB;
	tmpa = PINA;
	while (1)
	{
		
		if (tmpa >= 0x01) //less than or equal to 2
		{	
			
			tmpc = SetBit(tmpc, 5, 1);
			tmpc = SetBit(tmpc, 6, 1);
		}
		if (tmpa >= 0x03) //less than or equal to 4 
		{
			
			tmpc = SetBit(tmpc, 4, 1);
			tmpc = SetBit(tmpc, 6, 1);
		}
		if (tmpa >= 0x05) {//less than or equal to 6
		
		tmpc = SetBit(tmpc, 3, 1);
		}
		if (tmpa >= 0x07) {//less than or equal to 9
		
		tmpc = SetBit(tmpc,2,1);
		}
		if (tmpa >= 0x0a) {
		
		tmpc = SetBit(tmpc,1,1); //less than or equal to 12
		}
		if (tmpa >= 0x0d) { 
		
		tmpc = SetBit(tmpc,0,1); //less than or equal to 15
		}
		
		PORTB = tmpc;	
		tmpc = 0x00;
	}
}

