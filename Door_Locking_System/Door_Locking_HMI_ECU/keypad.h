 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 * Description: Header file for the Keypad driver
 *
 * Author: Islam Ehab
 *
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/************************************************************************************
 * 								Necessary Includes									*
 ************************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


/************************************************************************************
 * 								Preprocessor Defines								*
 ************************************************************************************/

/* Number of rows and columns in keypad HW */
#define N_COL	4
#define N_ROW	4

/* KEYPAD Port Configuration to MC */
#define KEYPAD_PORT_OUT		PORTA
#define KEYPAD_PORT_IN		PINA
#define KEYPAD_PORT_DIR		DDRA

/************************************************************************************
 * 								Functions Prototype									*
 ************************************************************************************/

/*
 * Description: Function to know which button pressed on keypad
 * @Param:		None
 * @Return:		Button Pressed (uint8)
 */
uint8 KeyPad_getPressedKey(void);


#endif /* KEYPAD_H_ */
