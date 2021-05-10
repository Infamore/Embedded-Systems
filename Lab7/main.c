/*
 * esoud001_lab7_part1.c
 *
 * Created: 4/30/2019 5:39:48 PM
 * Author : admin
 */ 

/*
 * esod001_lab7_part1.c
 *
 * Created: 4/30/2019 4:51:37 PM
 * Author : admin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char Command);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,const unsigned char *string);
void delay_ms(int miliSec);



void LCD_ClearScreen(void) {
	LCD_WriteCommand(0x01);
}

void LCD_init(void) {

	//wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);
}

void LCD_WriteCommand (unsigned char Command) {
	CLR_BIT(CONTROL_BUS,RS);
	DATA_BUS = Command;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
	SET_BIT(CONTROL_BUS,RS);
	DATA_BUS = Data;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
	LCD_ClearScreen();
	unsigned char c = column;
	while(*string) {
		LCD_Cursor(c++);
		LCD_WriteData(*string++);
	}
}

void LCD_Cursor(unsigned char column) {
	if ( column < 17 ) { // 16x1 LCD: column < 9
		// 16x2 LCD: column < 17
		LCD_WriteCommand(0x80 + column - 1);
		} else {
		LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
		// 16x2 LCD: column - 9
	}
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
	int i,j;
	for(i=0;i<miliSec;i++)
	for(j=0;j<775;j++)
	{
		asm("nop");
	}
}



unsigned char i = 0x00;
unsigned char c = '0';

// TimerISR() sets this to 1. C programmer should clear to 0.
volatile unsigned char TimerFlag = 0;

//Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1 ms ticks

void TimerOn()
{
	// AVR timer/counter controller register TCCR1
	// bit3 = 0; CTC mode (clear timer on compare)
	// bit2bit1bit0 = 011: prescaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 / 64 = 125,000 ticks/s
	// THUS, TCNT! register will count at 125,000 ticks/s
	TCCR1B = 0x0B;
	
	//AVR output compare register OCR1A.
	// Timer interrupt will be generated when TCNT! == OCR1A
	// We want a 1 ms tick. 0.001S * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// ! ms has passed. Thus, we compare to 125.
	OCR1A = 125;
	// AVR timer interrupt mask register
	// bit1: OCIE1A -- enables compare match interrupt
	TIMSK1 = 0x02;
	//Initialize avr counter
	TCNT1=0;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;
	//Enable global interrupts: 0x80: 1000000
	SREG |= 0x80;
}
void TimerOff()
{
	// bit3bit1bit0=000: timer off
	TCCR1B = 0x00;
}
void TimerISR()
{
	TimerFlag = 1;
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT1 == OCR1
	// (every 1 ms per TimerOn settings)
	// Count down to 0 rather than up to TOP (results in a more efficient comparison)
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		// Call the ISR that the user uses
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
void Tick();
enum States {Start, INIT, INC, DEC, WAIT, RESET}state;


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0xFF; //LCD data lines
	DDRD = 0xFF; PORTD = 0xFF; //LCD control lines
	
	//Initializes the LCD display
	LCD_init();
	
	TimerSet(500);
	TimerOn();
	
	while(1)
	{
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

void Tick()
{
	switch(state) // transitions
	{
		case Start:
		{
			LCD_Cursor(1);
			LCD_WriteData('0');
			state = INIT;
			break;
		}
		
		case INIT:
		{
			if((~PINA & 0x03) == 0x01)
			{
				state = INC; break;
			}
			else if((~PINA & 0x03) == 0x02)
			{
				state = DEC; break;
			}
			else if((~PINA & 0x03) == 0x03)
			{
				state = RESET; break;
			}
			else
			{
				state = INIT; break;
			}
		}
		
		case INC:
		{
			state = WAIT; break;
		}
		
		case DEC:
		{
			state = WAIT; break;
		}
		
		case WAIT:
		{
			if((~PINA & 0x03) == 0x01)
			{
				state = INC; break;
			}
			else if((~PINA & 0x03) == 0x02)
			{
				state = DEC; break;
			}
			else if((~PINA & 0x03) == 0x03)
			{
				state = RESET; break;
			}
			else if((~PINA & 0x03) == 0x00)
			{
				state = INIT; break;
			}
			else
			{
				state = WAIT; break;
			}
		}
		
		case RESET:
		{
			if((~PINA & 0x03) == 0x00)
			{
				state = INIT; break;
			}
			else
			{
				state = RESET; break;
			}
		}
		
		default:
		break;
	}
	
	switch(state) //state actions
	{
		case Start:
		break;
		
		case INIT:
		break;
		
		case INC:
		{
			if(i >= 9)
			{
				i = 9;
			}
			else
			{
				++i;
			}
			LCD_Cursor(1);
			LCD_WriteData(i + '0');
			break;
		}
		
		case DEC:
		{
			if(i <= 0)
			{
				i = 0;
			}
			else
			{
				--i;
			}
			LCD_Cursor(1);
			LCD_WriteData(i + '0');
			break;
		}
		
		case WAIT:
		break;
		
		case RESET:
		{
			i = 0;
			LCD_Cursor(1);
			LCD_WriteData(i + '0');
			break;
		}
	}
}
