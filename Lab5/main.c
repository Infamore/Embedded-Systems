/*
 * esoud001_lab5_part1.c
 *
 * Created: 4/19/2019 8:41:25 PM
 * Author : admin
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

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}


typedef enum leds {
	under2,
	under4,
	under6,
	under9,
	under12,
	under15,
}ledlights;

void setstate(ledlights lights, unsigned char button1, unsigned char button2, unsigned char button3, unsigned char button4) {
	if (button1 || button2)
	lights = under2;
	else if ((button1 && button2) || (button3))
	lights = under4;
	else if ((button3 && button1) || (button2 && button3))
	lights = under6;
	else if ((button1 && button2 && button3) || button4 || (button4 && button1))
	lights = under9;
	else if ((button4 && button2) || (button1 && button2 && button4) || (button3 && button4))
	lights = under12;
	else if ((button1 && button3 && button4) || (button2 && button3 && button4) || (button1 && button2 && button3 && button4))
	lights = under15;
}
/*
int main(void)
{
	DDRA = 0x00;
	DDRB = 0xFF;
	PORTA = 0xFF;
	PORTB = 0x00;
	
	ledlights lights;
	
	unsigned char button1 = ~PINA & 0x01;
	unsigned char button2 = ~PINA & 0x02;
	unsigned char button3 = ~PINA & 0x04;
	unsigned char button4 = ~PINA & 0x08;
	
	while (1) {
		switch(lights) {
			case under2:
			PORTB = SetBit(PORTB, 5, 1);
			PORTB = SetBit(PORTB, 6, 1);
			
			setstate(lights, button1, button2, button3, button4);
			
			break;
			
			case under4:
			PORTB = SetBit(PORTB, 4, 1);
			PORTB = SetBit(PORTB, 6, 1);
			
			setstate(lights, button1, button2, button3, button4);
			
			break;
			
			case under6:
			PORTB = SetBit(PORTB, 3, 1);
			
			setstate(lights, button1, button2, button3, button4);
			
			break;
			
			case under9:
			PORTB = SetBit(PORTB,2,1);
			
			setstate(lights, button1, button2, button3, button4);
			
			break;
			
			case under12:
			PORTB = SetBit(PORTB,1,1);
			setstate(lights, button1, button2, button3, button4);
			
			break;
			
			case under15:
			PORTB = SetBit(PORTB,0,1);
			setstate(lights, button1, button2, button3, button4);
			
			break;
			
			default:
			//	PORTB = 0x00;
			break;
		}
	}
}
*/
