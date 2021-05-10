/*	Esa Soudbakhsh
 *	
 *	Lab Section: 027
 *	Assignment: Lab # 4 Exercise # 3 
 *	Exercise Description: 
 *	enters a key pattern and if correct the door becomes unlocked. If a button is pressed then the door is locked.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; //PORTA = 0x00;
    DDRB = 0xFF; //PORTB = 0x00;

    typedef enum {
        firstkey, //wait for PA3 to be pressed
        secondkey, //wait for PA2 to be pressed
        dooropens, //yay
        }doorstate;

    doorstate state = firstkey;

    while (1) 
    {
        switch(state) 
        {
            case firstkey:
                if (PORTA == 0x04)  {
                    state = secondkey; 
                    PORTA = 0x00; //releases button
                }
                else 
                    state = firstkey;  
                break;
            case secondkey:
                if (PORTA == 0x02)  {
                    state = dooropens;
					PORTA = 0x00;
				}
                else 
                    state = firstkey;  
                break; 
            case dooropens:
                PORTB = 0x01;
                if (PORTA == 0x80) {
                    state = firstkey;
                    PORTB = 0x00;
                }
                break;
            default:
                state = firstkey;
        }
    }
}
