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
	enum States{L1,L2,L3,freeze}state;
	
//	volatile unsigned char TimerFlag = 0;
//        void TimerISR() {TimerFlag = 1;}
	unsigned char left = 0;;
	unsigned char uctimer = 0;


	void Tick(){
	switch(state){
		case L1:
		if((~PINA & 0x01)){
			uctimer = 0;
			state = freeze;
		}
		else if ((uctimer > 2)){
			uctimer = 0;
			state = L2;
		}
		else
			state = L1;
	
		
		break;

		case L2:
		if((~PINA & 0x01)){
                        uctimer = 0;
                        state = freeze;
                }
                else if ((uctimer > 2)){
			uctimer = 0;
                        if((left & 0x01))
				state = L3;
			else
				state = L1;
                }
                else
                        state = L2;

		break;

		case L3:
		if((~PINA & 0x01)){
                        uctimer = 0;
                        state = freeze;
                }
                else if ((uctimer > 2)){
			uctimer = 0;
                        state = L2;
                }
                else
                        state = L3;

		break;

		case freeze:
		if((~PINA & 0x01))
			state = L1;
		else
			state = freeze;
		break;
	
	
	}
	switch(state){
		case L1:
			left = 0x01;
			PORTB = 0x01;
			uctimer++;
		break;
		case L2:
			PORTB = 0x02;
			uctimer++;
		break;
		case L3:
			left = 0x00;
			PORTB = 0x04;
			uctimer++;
		break;
		case freeze:

		break;
	}
	}

int main(void) {
    /* Insert DDR and PORT initializations */

    /* Insert your solution below */
	DDRA = 0x00;PORTA = 0xFF;
       DDRB = 0xFF;PORTB = 0x00;	
	

	TimerSet(150);
	TimerOn();

	state = L1;
	left = 0x01;
	uctimer = 0;

    while (1) {
	Tick();
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
