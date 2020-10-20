 /******************************************************************************
 *
 * Module: Timers
 *
 * File Name: Timers.h
 *
 * Description: Header file for the AVR Timers driver
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


#ifndef TIMERS_H_
#define TIMERS_H_

/****************************************** Includes *********************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/************************************************************************************************/

/******************************************* Macros *********************************************/

#define PWM_TIMER_MODE		(0U)
#define NON_PWM_TIMER_MODE	(1U)

/************************************************************************************************/


/********************************** User Define Data Types **************************************/

typedef enum{

	NORMAL_MODE,
	CTC_MODE = 8,
	PHASE_COREECT_PWM = 64,
	FAST_PWM = 72

}Timer0_WaveformModeType;


typedef enum{

	NORMAL,
	TOGGLE,
	CLEAR_ON_COMPARE_NON_INVERTING,
	SET_ON_COMPARE_INVERTING

}Timer0_CompareMatchModeType;

typedef enum{

	NO_CLOCK,
	FCPU,
	FCPU_OVER_8,
	FCPU_OVER_64,
	FCPU_OVER_256,
	FCPU_OVER_1024,
	EXTERNAL_FALLING_CLOCK,
	EXTERNAL_RISING_CLOCK

}Timer0_ClockType;

typedef struct{

	uint8 						TimerMode;			/* You can find this value @TIMER_MODE
	 	 	 	 	 	 	   	   	   	   	   	   	   Used to decide Timer Mode (PWM / Non-PWM) */

	Timer0_WaveformModeType		WaveformMode;		/* You can find this value @Timer0_WaveformModeType
	 	 	 	 	 	 	 	 	 	 	 	   	   Used to decide Timer Mode (Fast/Phase Correct PWM , Normal/CTC Timer) */

	Timer0_CompareMatchModeType CompareMatchMode;	/* You can find this value @Timer0_CompareMatchModeType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide Timer Compare Match Mode */

	Timer0_ClockType			Clock;				/* You can find this value @Timer0_ClockType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide Timer Clock */

	uint8						InitialValue;		/* Value that the counter will start count from it */


	uint8						CompareValue;		/* End value of counting in compare match mode */


}Timer_ConfigType;

/************************************************************************************************/

/************************************* Functions Prototype **************************************/


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
void Timer0_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description: Used to Set Duty Cycle of Signal
 * @Param:		dutyCycle
 * @Return:		None
*/
void Timer0_setCallBack(void (*f_ptr)(void));

/*
 * Description: Function to set the Call Back function address
 * 				Used in Interrupt Situation
 * @Param:		Pointer to void Function
 * @Retrun:		None
 */
void PWM_setDutyCycle(uint8 dutyCycle);

/************************************************************************************************/



#endif /* TIMERS_H_ */
