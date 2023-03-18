/*
 * FinalProject1.c
 *
 * Created: 6/4/2019 4:10:18 PM
 * Author : admin
 */ 

#include <avr/io.h>
#include "ADC.h"
#include "functions.h"
#include "PWM.h"
#include "io.h"
#include "io.c"
#include "timer.h"
#include <stdio.h>


unsigned short temp = 0;
unsigned short freq = 0;
unsigned char* str;

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

enum SM1_States { SM1_wait, SM1_press, SM1_release };

// Monitors button connected to PA1.
// When button is pressed, shared variable "pause" is toggled.

enum SM2_States {read };
//read from temperature sensor
int SMTick2 (int state)
{
	switch(state)
	{
		
		case read:
			state = read;
		break;
		default:
		break;
	}
	
	switch(state)
	{
		case read:
		temp = ADC;
		break;
		default:
		break;
	}
}

enum SM3_States {on, off};
//fan controller
int SMTick3 (int state)
{
	unsigned short actualtemp = temp / 2;
	
	switch(state)
	{
		case off:
		if(temp >= 200) //we want to make sure that if the entire system is off, so is the fan
		state = on;
		break;
		
		case on:
		if (temp < 200) //pause not mandatory to turn off fan since it will turn off if limit achieved
		state = off;
		break;
		
		default:
		break;
	}
	
	switch(state)
	{
		case off:
		set_PWM(0);
	
		break;
		
		case on:
		freq = PWM_fan(temp);
		set_PWM(freq);
		
		break;
		
		default:
		break;
		
	}
}

enum SM4_States{on1, off1};

int SMTick4(int state)
{
	switch(state)
	{
		case off1:
		
		state = on1;
		break;
		case on1:
	
		state = off1;
		break;
	}
	
	switch(state)
	{
		case off1:
		LCD_ClearScreen();
		
		break;
		
		case on1:
		{
		char result[2];
		sprintf(result, "%d", temp);
		LCD_DisplayString(1, result);
		if(temp <= 900 && temp > 800 ) {
			
			str = "High";
		}
		else if(temp <=800 && temp > 700) {
			
			str = "High";
		}
		else if(temp <= 700 && temp > 600) {
			
			str = "Medium";
		}
		else if(temp <= 600 && temp > 500) {
			
			str = "Medium";
		}
		else if(temp <= 500 && temp > 400) {
			
			str = "Medium";
		}
		else if(temp >= 400 && temp > 300) {
			str = "Low";
		}
		else if(temp <= 300 && temp > 200) {
			str = "Low";
		}
		else {
			str = "off";
		}
		LCD_DisplayString(17, str);
	
		
		break;
		}
	}
	
}

int main(void)
{
	
	DDRB = 0xFF; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	ADC_init();
	PWM_on();
	LCD_init();
	/*unsigned short tmp;
	while (1){
		tmp = ADC;
		char result[2];
		sprintf(result, "%d", tmp);
		LCD_DisplayString(1, result);
		
	}*/

	unsigned long int SMTick2_period = 200;
	unsigned long int SMTick3_period = 200;
	unsigned long int SMTick4_period = 1000;

	//Declare an array of tasks
	
	static task task2, task3, task4;
	task *tasks[] = { &task2, &task3, &task4 };
	
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);


	
	// Task 2
	task2.state = -1;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &SMTick2;//Function pointer for the tick.

	// Task 3
	task3.state = -1;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period; // Task current elasped time.
	task3.TickFct = &SMTick3; // Function pointer for the tick.

	// Task 4
	task4.state = -1;//Task initial state.
	task4.period = SMTick4_period;//Task Period.
	task4.elapsedTime = SMTick4_period; // Task current elasped time.
	task4.TickFct = &SMTick4; // Function pointer for the tick.

	// Set the timer and turn it on
	
	TimerSet(200);
	TimerOn();
	
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 200;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}

