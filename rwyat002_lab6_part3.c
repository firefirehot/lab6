/*
 * LINK:  https://drive.google.com/open?id=1qWfVsUgssZbhIgoVBVncqHbE3xFzLQWl
 *PART1: 0:25
PART2: 0:55
PART3: 1:52
 * */


/*
Author: Robert Wyatt
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B 	= 0x0B;	// bit3 = 1: CTC mode (clear timer on compare)
					// bit2bit1bit0=011: prescaler /64
					// 00001011: 0x0B
					// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
					// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A 	= 125;	// Timer interrupt will be generated when TCNT1==OCR1A
					// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
					// So when TCNT1 register equals 125,
					// 1 ms has passed. Thus, we compare to 125.
					// AVR timer interrupt mask register

	TIMSK1 	= 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1 = 0;

	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;

	//Enable global interrupts
	SREG |= 0x80;	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B 	= 0x00; // bit3bit2bit1bit0=0000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; 			// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { 	// results in a more efficient compare
		TimerISR(); 				// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}





	enum States{init,base,up,down,reset}state;
	
//	volatile unsigned char TimerFlag = 0;
//        void TimerISR() {TimerFlag = 1;}
	unsigned char uccounter = 0;
	unsigned char uctimer = 0;


	void Tick(){
	switch(state){
		case init:
		state = base;
		uccounter = 7;
		break;

		case base:
		if((~PINA & 0x02) && (~PINA & 0x01)){
			state = reset;
		}
		else if((~PINA & 0x01) && !(~PINA & 0x02)){
			state = up;
			if(uccounter < 9)
				uccounter++;
		}
		else if((~PINA & 0x02) && !(~PINA & 0x01)){
			state = down;
			if(uccounter > 0)
				uccounter--;
		}
		else
			state = base;
		break;

		case up:
		if((~PINA & 0x02)){
		state = reset;}
		else if((~PINA & 0x01)){
		state = up;}
		else{
		state = base;}
		break;

		case down:
		if((~PINA & 0x01)){
                state = reset;}
                else if((~PINA & 0x02)){
                state = down;}
                else{
                state = base;}

		break;
		
		case reset:
		state = base;
		break;	
	
	}
	switch(state){

	case base:
		uctimer = 0;
		PORTB = uccounter;
	break;
	case up:
		uctimer++;
		if(uctimer > 10){
		uctimer = 0;
		if(uccounter < 9)
			uccounter++;
		}
		PORTB = uccounter;
	break;
	case down:
		uctimer++;
		if(uctimer > 10){
		uctimer = 0;
		if(uccounter > 0)
			uccounter--;
		}

		PORTB = uccounter;
	break;
	case reset:
	uccounter = 0;
	PORTB = uccounter;
	break;

	}
	}

int main(void) {
    /* Insert DDR and PORT initializations */

    /* Insert your solution below */
	DDRA = 0x00;PORTA = 0xFF;
       DDRB = 0xFF;PORTB = 0x00;	
	

	TimerSet(100);
	TimerOn();

	state = init;
	uccounter = 0x00;
	uctimer = 0;

    while (1) {
	Tick();
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
