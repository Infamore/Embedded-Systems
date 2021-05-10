/*	
 *	Esa Soudbakhsh
 *	Lab Section: 027
 *	Assignment: Lab # 3 Exercise # 3 
 *	Exercise Description:
 *	An addition to the previous exercise where bits from the higher nibble are assigned due to extra scenarios
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
	DDRA = 0x00; PORTA = 0x83;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpa = 0x00;
	unsigned char tmpc = 0x00;
	
	tmpa = tmpa | PORTA;
	tmpc = tmpc | PORTC;
	
	while (1)
	{
		if ((tmpa & 0x0F) >= 0x33) //less than or equal to 2
		{
			
			tmpc = SetBit(tmpc, 5, 1);
			tmpc = SetBit(tmpc, 6, 1);
		}
		if ((tmpa & 0x0F) >= 0x03) //less than or equal to 4
		{
			tmpc = SetBit(tmpc, 4, 1);
			tmpc = SetBit(tmpc, 6, 1);
		}
		if ((tmpa & 0x0F) >= 0x05) //less than or equal to 6
		tmpc = SetBit(tmpc, 3, 1);
		if ((tmpa & 0x0F) >= 0x07) //less than or equal to 9
		tmpc = SetBit(tmpc,2,1);
		if ((tmpa & 0x0F) >= 0x0a)
		tmpc = SetBit(tmpc,1,1); //less than or equal to 12
		if ((tmpa & 0x0F) >= 0x0d)
		tmpc = SetBit(tmpc,0,1); //less than or equal to 15
		if (GetBit(tmpa,4) == 1 && GetBit(tmpa,5) == 1 && GetBit(tmpa,6) == 0)
		tmpc = SetBit(tmpc,7,1);
		
		PORTC = PORTC | tmpc;
		asm("break");
	}
}
