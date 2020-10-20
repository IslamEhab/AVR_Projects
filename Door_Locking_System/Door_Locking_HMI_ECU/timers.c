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
 * Date: 27/9/2020
 *
 * Version:	2.0.0
 *
 * Note: Still Needs some updates Like
 * 		- Testing with all timers
 *******************************************************************************/



#include "timers.h"
#define F_CPU 8000000
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function for timer 0 in the application */
static volatile void (*g_callBackPtrTimer0)(void) = NULL_PTR;

/* Global variables to hold the address of the call back function for timer 1 in the application */
static volatile void (*g_callBackPtrTimer1)(void) = NULL_PTR;

/* Global variables to hold the address of the call back function for timer 2 in the application */
static volatile void (*g_callBackPtrTimer2)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_OVF_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTimer0 != NULL_PTR){

		 /* Call the Call Back function for Timer0 Overflow mode */
		(*g_callBackPtrTimer0)();

	}
}

ISR(TIMER0_COMP_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTimer0 != NULL_PTR){

		 /* Call the Call Back function for Timer0 CTC mode */
		(*g_callBackPtrTimer0)();
	}
}

ISR(TIMER1_OVF_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTimer1 != NULL_PTR){

		 /* Call the Call Back function for Timer1 Overflow mode */
		(*g_callBackPtrTimer1)();
	}
}

ISR(TIMER1_COMPA_vect){

	//PORTD ^= (1<<7);
	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTimer1 != NULL_PTR){

		 /* Call the Call Back function for Timer1 CTC Channel A mode */
		(*g_callBackPtrTimer1)();
	}
}

ISR(TIMER1_COMPB_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTimer1 != NULL_PTR){

		 /* Call the Call Back function for Timer1 CTC Channel B mode */
		(*g_callBackPtrTimer1)();
	}
}

ISR(TIMER2_OVF_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTimer2 != NULL_PTR){

		 /* Call the Call Back function for Timer2 Overflow mode */
		(*g_callBackPtrTimer2)();
	}
}

ISR(TIMER2_COMP_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTimer2 != NULL_PTR){

		 /* Call the Call Back function for Timer2 CTC mode */
		(*g_callBackPtrTimer2)();
	}
}




/********************************************************************************
 [Function Name]:	Timer_init

 [Description]:		Function to Initiate Timer Driver
 					- Interrupt Mode
 					- Decide Timer Mode (Normal, CTC)
 					- Decide Compare Match Mode
 					- Decide Timer Clock
 					- Put an Initiate Value to start counting from it
 					- Put a compare match value if using this mode

 [Args]:			config_Ptr

 [in]				config_Ptr: Pointer to Timer Configuration Structure

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void Timer_init(const Timer_ConfigType * config_Ptr){

	switch(config_Ptr -> TimerID){

	case TIMER_ID_0:

		/* Clear Registers before do anything */
		TCCR0 &= 0x00;
		TCNT0 &= 0x00;
		OCR0  &= 0x00;

		/*
		 * Set Initial Value to count from it
		 * (8 bit Timer so & with 0xFF to ensure
		 * start value does not exceed 255)
		*/
		TCNT0 = ((config_Ptr -> InitialValue) & 0xFF);

		/*
		 * Set Compare Match Value
		 * (8 bit Timer so & with 0xFF to ensure
		 * Compare match value does not exceed 255)
		*/
		OCR0  = ((config_Ptr -> CompareValue) & 0xFF);

		/* Choose Non PWM Mode */
		TCCR0 = (1 << FOC0);

		/* Check if O/P needed for Output compare match */
		/* Check for OC0 */
		if((config_Ptr -> OutputPin) == OC0_PB3){

			DDRB |= (1 << OC0_PB3);
		}

		/* Check which timer mode is chosen */
		if( (config_Ptr -> TimerMode ) == NORMAL_MODE) {

		/*	 Enable Normal Mode Timer Interrupt */
			TIMSK |= (1 << TOIE0);
		}
		else if ( (config_Ptr -> TimerMode) == CTC_MODE){

		/*	 Enable CTC Mode Timer Interrupt */
			TIMSK |= (1 << OCIE0);
		}

		/* Choose Timer Mode (Normal or CTC Mode) */
		TCCR0 = ( (TCCR0 & ~(0x48) ) | ( ( (config_Ptr -> TimerMode) << WGM00) ) );


		/* Choose which operation should be done when compare match occures */
		TCCR0 = ( (TCCR0 & 0xCF) | ( (config_Ptr -> CompareMatchMode) << COM00) );


		/* Select Timer Operating Clock */
		TCCR0 = ( (TCCR0 & 0xF8) | (config_Ptr -> Prescaler) );

		break;

	case TIMER_ID_1:

		/* Clear Registers before do anything */
		TCCR1A &= 0x00;
		TCCR1B &= 0x00;

		/* 16 bit Registers */
		TCNT1  &= 0x0000;
		OCR1A  &= 0x0000;
		OCR1B  &= 0x0000;

		/*
		 * Set Initial Value to count from it
		 * (16 bit Timer so no need to check variable size)
		*/
		TCNT1 = ( (config_Ptr -> InitialValue) );


		/* Choose Non PWM Mode */
		TCCR1A = (1 << FOC1A) | (1 << FOC1B) ;

		/* Check if O/P needed for Output compare match */

		/* Check for OC1A */
		if((config_Ptr -> OutputPin) == OC1A_PD5){

			DDRD |= (1 << OC1A_PD5);
		}

		/* Check for OC1B */
		else if ((config_Ptr -> OutputPin) == OC1B_PD4){

			DDRD |= (1 << OC1B_PD4);
		}

		/* Check which timer mode is chosen */
		if( (config_Ptr -> TimerMode ) == NORMAL_MODE) {

			/* Enable Normal Mode Timer Interrupt */
			TIMSK |= (1 << TOIE1);
		}
		else if ( (config_Ptr -> TimerMode) == CTC_MODE_CHANNEL_A){

			/*
			 * Set Compare Match Value for Channel A
			 * (16 bit Timer so no need to check variable size)
			*/
			OCR1A  = ((config_Ptr -> CompareValue));

			/* Enable CTC Mode Channel A Timer Interrupt */
			TIMSK |= (1 << OCIE1A);

			/* Choose which operation should be done when compare match occures
			 * If Channel A Chosen */
			TCCR1A = ( (TCCR1A & 0x3F) | ( (config_Ptr -> CompareMatchMode) << COM1A0) );

		}

		else if ( (config_Ptr -> TimerMode) == CTC_MODE_CHANNEL_B){

			/*
			 * Set Compare Match Value for Channel B
			 * (16 bit Timer so no need to check variable size)
			*/
			OCR1B  = ((config_Ptr -> CompareValue));

			/* Enable CTC Mode Channel B Timer Interrupt */
			TIMSK |= (1 << OCIE1B);

			/* Choose which operation should be done when compare match occures
			 * If Channel B Chosen */
			TCCR1A = ( (TCCR1A & 0xCF) | ( (config_Ptr -> CompareMatchMode) << COM1B0) );

		}


		/* Choose Timer Mode (Normal or CTC Mode) */
		TCCR1B = ( ( TCCR1B & 0xE7 ) |  ( ( (config_Ptr -> TimerMode) >> 1) << WGM12)  );

		/* Select Timer Operating Clock */
		TCCR1B = ( ( TCCR1B & 0xF8 ) | (config_Ptr -> Prescaler) );


		break;

	case TIMER_ID_2:

		/* Clear Registers before do anything */
		TCCR2 &= 0x00;
		TCNT2 &= 0x00;
		OCR2  &= 0x00;

		/*
		 * Set Initial Value to count from it
		 * (8 bit Timer so & with 0xFF to ensure
		 * start value does not exceed 255)
		*/
		TCNT2 = ((config_Ptr -> InitialValue) & 0xFF);

		/*
		 * Set Compare Match Value
		 * (8 bit Timer so & with 0xFF to ensure
		 * Compare match value does not exceed 255)
		*/
		OCR2  = ((config_Ptr -> CompareValue) & 0xFF);

		/* Choose Non PWM Mode */
		TCCR2 = (1 << FOC2);

		/* Check which timer mode is chosen */
		if( (config_Ptr -> TimerMode ) == NORMAL_MODE) {

		/*	 Enable Normal Mode Timer Interrupt */
			TIMSK |= (1 << TOIE2);
		}
		else if ( (config_Ptr -> TimerMode) == CTC_MODE){

		/*	 Enable CTC Mode Timer Interrupt */
			TIMSK |= (1 << OCIE2);
		}

		/* Choose Timer Mode (Normal or CTC Mode) */
		TCCR2 |= ( (TCCR2 & ~(0x48) ) | ( ( (config_Ptr -> TimerMode) << WGM20) ) );

		/* Choose which operation should be done when compare match occures */
		TCCR2 = ( (TCCR2 & 0xCF) | (config_Ptr -> CompareMatchMode) << COM20);

		/* Select Timer Operating Clock */
		TCCR2 = ( (TCCR2 & 0xF8) |(config_Ptr -> Prescaler) );

		break;

	default:

		break;
	}


}



/********************************************************************************
 [Function Name]:	Timer0_setCallBack

 [Description]:		Function Callback to serve Timer0 ISR Code

 [Args]:			f_ptr

 [in]				f_ptr: Pointer to void function
					which will save address of ISR Handler Function in main

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void Timer0_setCallBack(void (*f_ptr)(void)){
	g_callBackPtrTimer0 = f_ptr;
}



/********************************************************************************
 [Function Name]:	Timer1_setCallBack

 [Description]:		Function Callback to serve Timer0 ISR Code

 [Args]:			f_ptr

 [in]				f_ptr: Pointer to void function
					which will save address of ISR Handler Function in main

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void Timer1_setCallBack(void (*f_ptr)(void)){
	g_callBackPtrTimer1 = f_ptr;
}



/********************************************************************************
 [Function Name]:	Timer2_setCallBack

 [Description]:		Function Callback to serve Timer2 ISR Code

 [Args]:			f_ptr

 [in]				f_ptr: Pointer to void function
					which will save address of ISR Handler Function in main

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void Timer2_setCallBack(void (*f_ptr)(void)){
	g_callBackPtrTimer2 = f_ptr;
}



/********************************************************************************
 [Function Name]:	Timer_stop

 [Description]:		Function used to stop timer by disabling clock

 [Args]:			timerNumber

 [in]				timerNumber: used to decide which timer to stop

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void Timer_stop(uint8 timerID){

	switch(timerID){

	case TIMER_ID_0:

		/* Stop Clock on timer 0 */
		TCCR0 &= ~(0x07);
	break;

	case TIMER_ID_1:

		/* Stop Clock on timer 1 */
		TCCR1B &= ~(0x07);
	break;

	case TIMER_ID_2:

		/* Stop Clock on timer 0 */
		TCCR2 &= ~(0x07);
	break;
	}

}


