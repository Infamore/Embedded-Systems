/*
 * esoud001_lab2_part3.c
 *
 * Created: 4/4/2019 6:06:10 PM
 * Author : admin
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x03; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char cntavail = 0x00;
	/* Replace with your application code */
	while (1)
	{
		tmpA = DDRA;
		cntavail = (~tmpA & 0x0F);
		
		if (cntavail == 0x00)
			cntavail = cntavail | 0x80;
			
		PORTC = cntavail;
		
			
	}
	
}

