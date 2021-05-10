/*
 * esoud001_part4_lab10.c
 *
 * Created: 5/14/2019 2:40:29 AM
 * Author : admin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


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

typedef enum single_LED {wait, on, off} singleled;
typedef enum three_LED {wait1, one, two, three} threeled;
typedef enum pwm {init, increment, decrement, on} pwmbuzzer;
/*
singleled singletick(singleled currentled, unsigned char* tmp) {
	
	switch(currentled)
	{
		case wait:
		currentled = on;
		tmp = 0x00;
		break;
		
		case on:
		currentled = off;
		tmp = 0x08;
		break;
		
		case off:
		currentled = on;
		tmp = 0x00;
		break;
		
		default:
		currentled = wait;
		tmp = 0x00;
		break;
		
	}
	
	return currentled;
}

threeled threetick(threeled threestate, unsigned char* threetmp)
{
	switch(threestate)
	{
		case wait1:
		threestate = one;
		threetmp = 0x00;
		break;
		
		case one:
		threestate = two;
		threetmp = 0x01;
		break;
		
		case two:
		threestate = three;
		threetmp = 0x02;
		break;
		
		case three:
		threestate = one;
		threetmp = 0x04;
		break;
		
		default:
		threestate = wait;
		threetmp = 0x00;
		break;
		
	}
	
	return threestate;
	
	
}
*/

int main(void)
{
    DDRB = 0xFF; DDRA = 0x00;
	PORTB = 0x00; PORTA = 0x00;
	TimerSet(1000);
	TimerOn();
	PWM_on();
	int single_elapsed_time = 0;
	int three_elapsed_time = 0;
	singleled singlestate = wait;
	threeled threestate = wait1;
	pwmbuzzer pwmstate = init;
	int pwmcounter = 0;
	unsigned char singletmp = 0x00;
	unsigned char threetmp = 0x00;
	unsigned char pwmtmp = 0x00;
	
    while (1) 
    {
		if (single_elapsed_time >= 1000) {
			//singlestate = singletick(singlestate, singletmp);
			
			switch(singlestate)
			{
				case wait:
				singletmp = 0x00;
				singlestate = on;
				
				break;
				
				case on:
				singletmp = 0x08;
				singlestate = off;
				
				break;
				
				case off:
				singletmp = 0x00;
				singlestate = on;
				
				break;
				
				default:
				singletmp = 0x00;
				singlestate = wait;
				
				break;
				
			}
			
			single_elapsed_time = 0;
		}
		
		if (three_elapsed_time >= 300) {
			//threestate = threetick(threestate, threetmp);
			switch(threestate)
			{
				case wait1:
				threetmp = 0x00;
				threestate = one;
				
				break;
				
				case one:
				threetmp = 0x01;
				threestate = two;
				
				break;
				
				case two:
				threetmp = 0x02;
				threestate = three;
				
				break;
				
				case three:
				threetmp = 0x04;
				threestate = one;
				
				break;
				
				default:
				threetmp = 0x00;
				threestate = wait;
				
				break;
				
			}
			
			
			three_elapsed_time = 0;
		}
		
		switch(pwmstate)
		{
			case init:
				set_PWM(0);
				if (PINA == 0x01)
					pwmstate = on;
				if (PINA == 0x02)
					pwmstate = increment
				if (PINA == 0x04)
					pwmstate = decrement;
				break;
			
			case on:
				set_PWM(pwmcounter);
				if (PINA == 0x00)
					pwmstate = init;
				break;
			
			case increment:
				pwmcounter += 100;
				pwmstate = init;
				break;
				
			case decrement:
				pwmcounter -= 100;
				pwmstate = init;
			
			default:
				set_PWM(0);
				pwmstate = init;
		}
			
		while(!TimerFlag);
		TimerFlag = 0;
		single_elapsed_time += 1000;
		three_elapsed_time += 1000;
		PORTB = singletmp | threetmp;
    }
}
