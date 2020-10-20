 /******************************************************************************
 *
 * Module: EXTI
 *
 * File Name: EXTI.c
 *
 * Description: Source file for the AVR EXTI Driver
 *
 * Author: Islam Ehab
 *
 * Date: 22/9/2020
 *
 * Version:	1.0.0
 *
 * Note: Still Needs some updates
 *
 *******************************************************************************/


#include "EXTI.h"

/*
 * Global pointer to void function to hold address of call back function
 * static used to make this pointer file scope only
 */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;


ISR(INT1_vect){

		/*
		 * Check if the Pointer not a Null one
		 * To avoid segmentation fault
		*/
		if(g_callBackPtr != NULL_PTR){

			/* Call the Call Back function */
			g_callBackPtr();
		}
}

/*
 * Description: Function to initiate EXTI 1 (PD3)
 * 				- PD3 I/P
 *				- External Pull Down Circuit
 * 				- Interrupt with rising edge
 * @Param:		None
 * @Return:		None
 */
void INT1_init(void){

	/* Disable interrupts by clearing I-bit */
	SREG  &= ~(1<<7);

	/* Set PD3 as I/P */
	DDRD &= ~(1<<PD3);

	/* Detect Interrupt with rising edge */
	MCUCR |= ( (1<<ISC11) | (1<<ISC10) );

	/* Enable INT1 */
	GICR  |= (1<<INT1);

	/* Enable Interrupt by Setting I-Bit */
	SREG |= (1<<7);
}

/*
 * Description: Function to set the Call Back function address.
 * @Param:		Pointer to void function
 * @Return:		None
*/
void INT1_setCallBack(void (*f_ptr)(void)){
	g_callBackPtr = f_ptr;
}

