/*
 * Timer1_CTC.c
 *
 *  Created on: Aug 23, 2020
 *      Author: Islam Ehab
 */
#include "Timer1_CTC.h"

extern unsigned char g_tick;

/* Description: Timer1 CTC Configuration (Channel A)
 * 				Used to count for 1 Second delay in Stop Watch
 * 				- F_CPU = 1MHz
 * 				  F_Timer1 = F_CPU / 1024 (Prescaler = 1024) = 976.5625 Hz
 * 				  T_Timer1 = 1 / 976.5625 = 1.024 mSec nearly 1 mSec
 * 				  it will count for 1 Sec so OCR1A = 1000;
 *				- CTC Mode with Clear OC1A on Compare match
 *				- Enable Timer1_CTC Interrupt Channel A
 *				- Enable Global Interrupt Bit
 *@Param:		None
 *@Retun:		None
 * */
void Timer1_CTC_Init(void)
{
	/* Starts count from 0 */
	TCNT1 = 0;

	/* Enable Interrupt for CTC Mode Channel A*/
	TIMSK = ( (1<<OCIE1A) );

	/* Clear OC1A on Compare match
	 * FOC1A = 1 Because CTC Mode
	 * */
	TCCR1A = ( (1<<COM1A1) | (1<<FOC1A) );

	/* CTC Mode with top value at OCR1A
	 * Prescaler = 1024 -> CS12 = 1, CS10 = 1
	 * */
	TCCR1B = ( (1<<WGM12) | (1<<CS12) | (1<<CS10));

	/* Compare Value for indicating that 1 sec has passed */
	/* OCR1A should = 1000, but after real time testing
	 * i found that 750 is a perfect value due to latency of Simulator
	 * And the +0.024ms of the T_Timer1
	*/
	OCR1A = 750;

	/* Enable Global Interrupt Bit */
	SREG |= (1<<7);
}

/* Description: Function used to make delay for Stop Watch
 * 				using Timer1 CTC Mode Channel A
 * @Param:		None
 * @Retrun:		None
 *  */
void delay(void){

	//TCCR1B = ( (1<<CS12) | (1<<CS10) );
	while(g_tick != 1){
	}
	g_tick = 0;
}

/* Description: Function Used to Stop the timer by
 * 				Turning off the clocks
 * @Param:		None
 * @Return:		None
 * */
void Timer1_Stop(void){


	/* No Clock Source to Stop the timer */
	TCCR1B &= ~(1<<CS12);
	TCCR1B &= ~(1<<CS10);

	/* Disable Interrupt for CTC Mode Channel A*/
	TIMSK  &= ~(1<<OCIE1A);

	/* Disable FOC1A = 1 Because CTC Mode */
	TCCR1A &= ~(1<<FOC1A) ;
}

/* Description: Function Used to Resume the timer by
 * 				Turning On the clocks
 * @Param:		None
 * @Return:		None
 * */
void Timer1_Resume(void)
{

	/* Enable Interrupt for CTC Mode Channel A */
	TIMSK  |= (1<<OCIE1A);

	/* FOC1A = 1 Because CTC Mode */
	TCCR1A |= (1<<FOC1A) ;

	/* Prescaler = 1024 -> CS12 = 1, CS10 = 1 */
	TCCR1B = ( (1<<CS12) | (1<<CS10) );


}
