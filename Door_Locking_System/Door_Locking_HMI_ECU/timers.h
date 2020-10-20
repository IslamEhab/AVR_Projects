 /******************************************************************************
 *
 * Module: Timers
 *
 * File Name: timers.h
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

#ifndef TIMERS_H_
#define TIMERS_H_

/****************************************** Includes *********************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/************************************************************************************************/

/******************************************* Macros *********************************************/

#define TIMER_ID_0		(0U)
#define TIMER_ID_1		(1U)
#define TIMER_ID_2		(2U)

/************************************************************************************************/


/********************************** User Define Data Types **************************************/

/********************************************************************************

 [Enumuration Name]:		Timer_ModeType

 [Enumuration Description]:	Has Timer Modes which will be used in Config struct

*********************************************************************************/
typedef enum{

	NORMAL_MODE,
	CTC_MODE,
	CTC_MODE_CHANNEL_A,
	CTC_MODE_CHANNEL_B

}Timer_ModeType;


/********************************************************************************

 [Enumuration Name]:		Timer_CompareMatchModeType

 [Enumuration Description]: Has Timer operation when compare match occure

*********************************************************************************/
typedef enum{

	NORMAL,
	TOGGLE,
	CLEAR_ON_COMPARE_NON_INVERTING,
	SET_ON_COMPARE_INVERTING

}Timer_CompareMatchModeType;


/********************************************************************************

 [Enumuration Name]:		Timer_OutputComparePinsType

 [Enumuration Description]:	Used to initiate O/P Pins for Output compare pins
 	 	 	 	 	 	 	if needed

*********************************************************************************/

typedef enum{

	NONE,
	OC0_PB3  = PB3,
	OC1B_PD4 = PD4,
	OC1A_PD5 = PD5,
	OC2_PD7  = PD7

}Timer_OutputComparePinsType;


/********************************************************************************

 [Enumuration Name]:		Timer_ClockType

 [Enumuration Description]:	Has Timer Prescalers

*********************************************************************************/
typedef enum{

	NO_CLOCK,
	FCPU,
	FCPU_OVER_8,
	FCPU_OVER_64,
	FCPU_OVER_256,
	FCPU_OVER_1024,
	EXTERNAL_FALLING_CLOCK,
	EXTERNAL_RISING_CLOCK

}Timer_PrescalerType;



/********************************************************************************

 [Structure Name]:			Timer_ConfigType

 [Structure Description]:	Structure which will get Timer configuration
 	 	 	 	 	 	 	to initiailize the timer properly

*********************************************************************************/
typedef struct{

	uint8						TimerID;			/* You can find this value @TIMER_ID
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide which timer will be used */

	Timer_ModeType				TimerMode;			/* You can find this value @Timer_ModeType
	 	 	 	 	 	 	 	 	 	 	 	   	   Used to decide Timer Mode (Normal/CTC Timer) */

	Timer_CompareMatchModeType  CompareMatchMode;	/* You can find this value @Timer_CompareMatchModeType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide Timer Compare Match Mode */

	Timer_OutputComparePinsType OutputPin;			/* You can finde this value @Timer_OutputComparePinsType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to initiate o/p for Output compare pins if needed */

	Timer_PrescalerType			Prescaler;			/* You can find this value @Timer_PrescalerType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide Timer Clock */

	uint16						InitialValue;		/* Value that the counter will start count from it
	 	 	 	 	 	 	 	 	 	 	 	 	   uint16 to be compatible with Timer1 16 bit */

	uint16						CompareValue;		/* End value of counting in compare match mode
	 	 	 	 	 	 	 	 	 	 	 	 	   uint16 to be compatible with Timer1 16 bit */


}Timer_ConfigType;

/************************************************************************************************/

/************************************* Functions Prototype **************************************/



/********************************************************************************
 [Function Name]:	Timer_init

 [Description]:		Function to Initiate Timer Driver
 					- Interrupt Mode
 					- Decide Timer Mode (Normal, CTC)
 					- Decide Compare Match Mode
 					- Decide Timer Clock
 					- Put an Initiate Value to start counting from it
 					- Put a compare match value if using this mode

 [Args]:			Config_Ptr

 [in]				Config_Ptr: Pointer to Timer Configuration Structure

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void Timer_init(const Timer_ConfigType * config_Ptr);



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
void Timer0_setCallBack(void (*f_ptr)(void));



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
void Timer1_setCallBack(void (*f_ptr)(void));



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
void Timer2_setCallBack(void (*f_ptr)(void));



/********************************************************************************
 [Function Name]:	Timer_stop

 [Description]:		Function used to stop timer by disabling clock

 [Args]:			timerID

 [in]				timerID: used to decide which timer to stop

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void Timer_stop(uint8 timerID);


/************************************************************************************************/



#endif /* TIMERS_H_ */
