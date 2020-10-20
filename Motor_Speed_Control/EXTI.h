 /******************************************************************************
 *
 * Module: EXTI
 *
 * File Name: EXTI.h
 *
 * Description: Header file for the AVR EXTI Driver
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

#ifndef EXTI_H_
#define EXTI_H_

/****************************************** Includes *********************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/************************************************************************************************/

/************************************* Functions Prototype **************************************/

/*
 * Description: Function to initiate EXTI 1 (PD3)
 * 				- PD3 I/P
 *				- External Pull Down Circuit
 * 				- Interrupt with rising edge
 * @Param:		None
 * @Return:		None
*/
void INT1_init(void);

/*
 * Description: Function to set the Call Back function address.
 * @Param:		Pointer to void function
 * @Return:		None
*/
void INT1_setCallBack(void (*f_ptr)(void));

/************************************************************************************************/


#endif /* EXTI_H_ */
