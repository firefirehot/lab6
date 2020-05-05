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
