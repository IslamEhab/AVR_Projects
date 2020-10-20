 /******************************************************************************
 *
 * Module: Timers
 *
 * File Name: Timers.c
 *
 * Description: Source file for the AVR Timers driver
 *
 * Author: Islam Ehab
 *
 * Date: 22/9/2020
 *
 * Version:	1.0.0
 *
 * Note: Still Needs some updates Like
 * 		- Support of Timer1 & 2
 * 		- Testing with all timers
 * 		- How to Interact with all Timer Interrupts
 *
 *******************************************************************************/



#include "Timers.h"


/*
 * Description: Function to Initiate ADC Driver
 *				- Interrupt Mode
 *				- Decide PWM / Non PWM Mode
 *				- Decide Compare Match Mode
 *				- Decide Timer Clock
 *				- Put an Initiate Value to start counting from it
 *				- Put a compare match value if using this mode
 * @Param:		Config_Ptr: Pointer to Timer Configuration Structure
 * @Return:		None
 */
void Timer0_init(const Timer_ConfigType * Config_Ptr){

	TCCR0 &= 0x00;
	TCNT0 &= 0x00;
	OCR0  &= 0x00;

	/* Set Compare Match Value */
	OCR0  = (Config_Ptr -> CompareValue);


	if( (Config_Ptr -> TimerMode) == PWM_TIMER_MODE){
		/*	 set PB3/OC0 as output pin -if PWM Mode Enabled- pin where the PWM signal is generated from MC */
		DDRB |= (1 << PB3);

		/* Enable O/P On compare match */
		TCCR0 |= (1<<COM01);

		/* Choose PWM Mode */
		TCCR0 &= ~(1<<7);
	}

	else{
	/* Choose Non PWM Mode */
	TCCR0 = ( (Config_Ptr -> TimerMode) << 7);
	}

	/* Choose Waveform Mode */
	TCCR0 = ( (TCCR0 & ~(0x48) ) | ( (Config_Ptr -> WaveformMode) ) );

	/* In PWM Modes there are not Toggle Mode */
	if( ( (Config_Ptr -> TimerMode) == PWM_TIMER_MODE) && (Config_Ptr -> CompareMatchMode) == TOGGLE){
		TCCR0 |= CLEAR_ON_COMPARE_NON_INVERTING;
	}

	/* In Normal / CTC Mode there is a Toggle Mode */
	else{
		TCCR0 |= ( (Config_Ptr -> CompareMatchMode) << 3);
	}

	/* Select Timer Operating Clock */
	TCCR0 |= (Config_Ptr -> Clock );


	/* Check which timer mode is chosen */
	if( (Config_Ptr -> WaveformMode ) == NORMAL_MODE) {

	/*	 Enable Normal Mode Timer Interrupt */
		TIMSK |= (1<<0);
	}
	else if ( (Config_Ptr -> WaveformMode) == CTC_MODE){
	/*	 Enable CTC Mode Timer Interrupt */
		TIMSK |= (1<<1);

	}
}

/*
 * Description: Used to Set Duty Cycle of Signal
 * @Param:		dutyCycle
 * @Return:		None
*/
void PWM_setDutyCycle(uint8 dutyCycle){

	OCR0 = dutyCycle;
}

/*
 * Description: Function to set the Call Back function address
 * 				Used in Interrupt Situation
 * @Param:		Pointer to void Function
 * @Retrun:		None
 */
void Timer0_setCallBack(void (*f_ptr)(void)){

}
