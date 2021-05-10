/*
 * esoud001_part1_lab9.c
 *void PWM_on() {
	 TCCR3A = (1 << COM3A0);
	 // COM3A0: Toggle PB6 on compare match between counter and OCR3A
	 TCCR3B = (1 << WGM32
 * Created: 5/6/2019 11:06:15 PM
 * Author : admin
 */ 

#include <avr/io.h>

void set_PWM(double frequency);
void PWM_off();
void PWM_on();

void set_PWM(double frequency) {
	
	
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT0 = 0; // resets counter
		//current_frequency = frequency;
	}
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


void PWM_on() {
	TCCR3A = (1 << COM0A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}


void Tick();
enum States{Start, ON, OFF, INIT} state;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0xFF;
	state = Start;
	PWM_on();
	
	while(1)
	{
		Tick();
	}
}

void Tick()
{
	switch(state) // transitions
	{
		case Start:
		{
			state = INIT;
			break;
		}
		case INIT:
		{
			if((~PINA & 0x07) != 0x00)
			{
				state = ON;
				break;
			}
			else
			{
				state = INIT;
				break;
			}
		}
		case ON:
		{
			if((~PINA & 0x07) != 0x00)
			{
				state = ON; break;
			}
			else if((~PINA & 0x07) == 0x03)
			{
				state = OFF; break;
			}
			else if((~PINA & 0x07) == 0x05)
			{
				state = OFF; break;
			}
			else if((~PINA & 0x07) == 0x06)
			{
				state = OFF; break;
			}
			else if((~PINA & 0x07) == 0x07)
			{
				state = OFF; break;
			}
			else
			{
				state = OFF; break;
			}
		}
		case OFF:
		{
			state = INIT;
			break;
		}
		default:
		break;
	}
	switch(state)
	{
		case Start:
		break;
		case INIT:
		{
			set_PWM(0);
			break;
		}
		case ON:
		{
			if((~PINA & 0x07) == 0x01)
			{
				set_PWM(261.63);
				break;
			}
			else if((~PINA & 0x07) == 0x02)
			{
				set_PWM(293.66);
				break;
			}
			else if((~PINA & 0x07) == 0x04)
			{
				set_PWM(329.63);
				break;
			}
			else 
			{
				set_PWM(0);
				break;
			}
		}
		case OFF:
		{
			set_PWM(0);
			break;
		}
		default:
		break;
		
	}
}






/*
typedef enum States
{
	C4, 
	D4, 
	E4, 
	nochords
}state;*/
/*
void nextstate(state curr)
{
	if(PINA == 0x01)
		curr = C4;
	else if (PINA == 0x02)
		curr = D4;
	else if (PINA == 0x04)
		curr = E4;
	else
		curr = nochords;
}
*/
/*
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state currentstate = nochords;
	PWM_on();
	
	switch (currentstate)
	{
		case nochords:
			set_PWM(0);
			//nextstate(currentstate);
			if(PINA == 0x01)
			currentstate = C4;
			else if (PINA == 0x02)
			currentstate = D4;
			else if (PINA == 0x04)
			currentstate = E4;
			else
			currentstate = nochords;
			
			break;
		case C4:
			set_PWM(261.63);
			//nextstate(currentstate);
			if(PINA == 0x01)
			currentstate = C4;
			else if (PINA == 0x02)
			currentstate = D4;
			else if (PINA == 0x04)
			currentstate = E4;
			else
			currentstate = nochords;
			break;
		case D4:
			set_PWM(293.66);
			//nextstate(currentstate);
			if(PINA == 0x01)
			currentstate = C4;
			else if (PINA == 0x02)
			currentstate = D4;
			else if (PINA == 0x04)
			currentstate = E4;
			else
			currentstate = nochords;
			break;
		case E4:
			set_PWM(329.63);
			//nextstate(currentstate);
			if(PINA == 0x01)
			currentstate = C4;
			else if (PINA == 0x02)
			currentstate = D4;
			else if (PINA == 0x04)
			currentstate = E4;
			else
			currentstate = nochords;
			break;
		default:
			currentstate = nochords;
			break;
	}
	
}*/
