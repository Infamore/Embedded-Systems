/*	Esa Soudbakhsh
 *	Lab Section: 027
 *	Assignment: Lab # 4 Exercise # 1
 *	Exercise Description: 
 *	switches the led output each time 0x01 is high
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x03; PORTB = 0x01;
	
	typedef enum {
		initial,
		PB0on,
		PB1on,
		}ledstate;
	
	ledstate state = initial;
	
    while (1) 
    {
		switch(state) {//transitions
			case initial:
			PORTB = 0x01;
			if (PORTA == 0x01) { 
				state = PB1on;
				PORTA = 0x00;
			}
			
			break;
			case PB1on:
			PORTB = 0x02;
			if (PORTA == 0x01) { 
				state = PB0on;
				PORTA = 0x00;
			}
			
			break;
			case PB0on:
			PORTB = 0x01;
			if (PORTA == 0x01) {
				state = PB1on;
				PORTA = 0x00;
			}
			break;
			default:
			state = initial;
			PORTB = 0x01;
		}
		
	}
}

