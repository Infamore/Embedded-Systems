/*
 * esoud001_lab2_part4.c
 *
 * Created: 4/4/2019 11:30:10 PM
 * Author : admin
 */ 

#include <avr/io.h>

//part 1 of challenge problem
int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0x00;
	unsigned char weight, weight1, tmpA, tmpB, tmpC, tmpD;
		
    while (1) 
    {
		tmpA = DDRA;
		tmpB = DDRB;
		tmpC = DDRC;
		tmpD = DDRD;
		
		weight = tmpA + tmpB + tmpC;
		//if weight exceeds 140, set PD0 to 1
		if (weight > 0x8c)
			tmpD = tmpD | 0x01;
		//if the weight of A - C is 80, set PD1 to 1
		if ((tmpA - tmpC) > 0x50)
			tmpD = tmpD | 0x02;
    }
}

