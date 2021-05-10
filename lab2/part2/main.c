/*
 * esoud001_lab2_part2.c
 *
 * Created: 4/4/2019 5:52:20 PM
 * Author : admin
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; 
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char cntavail = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		tmpA = DDRA;
		cntavail = ~tmpA;
		PORTC = cntavail;
		tmpA = PORTA; //arbitrary line to allow break for previous line
    }
}

