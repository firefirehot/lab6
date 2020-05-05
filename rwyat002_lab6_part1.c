
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
#include "timer.h"
#endif
	enum States{L1,L2,L3}state;
	
//	volatile unsigned char TimerFlag = 0;
//        void TimerISR() {TimerFlag = 1;}

	unsigned char uctimer = 0;


	void Tick(){
	switch(state){
		case L1:
		state = L2;
		break;

		case L2:
		state = L3;
		break;

		case L3:
		state = L1;
		break;
	
	
	}
	switch(state){
		case L1:
			PORTB = 0x01;
		break;
		case L2:
			PORTB = 0x02;
		break;
		case L3:
			PORTB = 0x04;
		break;
	}
	}

int main(void) {
    /* Insert DDR and PORT initializations */

    /* Insert your solution below */
	DDRA = 0x00;PORTA = 0xFF;
       DDRB = 0xFF;PORTB = 0x00;	
	

	TimerSet(1000);
	TimerOn();

	state = L1;

    while (1) {
	Tick();
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
