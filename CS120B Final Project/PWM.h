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


unsigned short PWM_fan(unsigned short tmp)
{
if(tmp <= 900 && tmp > 800 ) {
	return 900;

}
else if(tmp <=800 && tmp > 700) {
	return 600;

}
else if(tmp <= 700 && tmp > 600) {
	return 500;

}
else if(tmp <= 600 && tmp > 500) {
	return 400;

}
else if(tmp <= 500 && tmp > 400) {
	return 300;

}
else if(tmp >= 400 && tmp > 300) {
	return 200;

}
else if(tmp <= 300 && tmp > 200) {
	return 100;
}

else {
	return 0;
	
}
	
}
/*
unsigned char* retmode(unsigned short tmp)
{
	char* stri;
	if(tmp <= 900 && tmp > 800 ) {
		
		return stri = "High";
	}
	else if(tmp <=800 && tmp > 700) {
		
		return stri = "High";
	}
	else if(tmp <= 700 && tmp > 600) {
		
		return stri = "Medium";
	}
	else if(tmp <= 600 && tmp > 500) {
		
		return stri = "Medium";
	}
	else if(tmp <= 500 && tmp > 400) {
		
		return stri = "Medium";
	}
	else if(tmp >= 400 && tmp > 300) {
		stri = "Low";
		return stri;
	}
	else if(tmp <= 300 && tmp > 200) {
		stri = "Low";
		return stri;
	
	}
	else {	
		stri = "off";
		return stri;
	}
	
}*/