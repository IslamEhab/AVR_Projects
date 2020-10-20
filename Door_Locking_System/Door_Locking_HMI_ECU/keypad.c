 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: Islam Ehab
 *
 *******************************************************************************/
/************************************************************************************
 * 								Necessary Includes									*
 ************************************************************************************/
#include "keypad.h"

/************************************************************************************
 * 							Functions Prototype (Private)							*
 ************************************************************************************/

/* Preprocessor Check for Keypad Hardware */
#if	(N_COL == 3)
/*
 * Function to handle keypad 4x3 buttons
 * and its representation on actual HW
 * */

static uint8 KeyPad_4x3_keyMapping(uint8 button);
#elif(N_COL == 4)
/*
 * Function to handle keypad 4x4 buttons
 * and its representation on actual HW
 * */

static uint8 KeyPad_4x4_keyMapping(uint8 button);
#endif


/************************************************************************************
 * 								Functions Definitions 								*
 ************************************************************************************/
/*
 * Description: Function to know which button pressed on keypad
 * @Param:		None
 * @Return:		Pressed Button (uint8)
 */
uint8 KeyPad_getPressedKey(void){
	uint8 row, col;

	/* Super loop to give the user time to press the key */
	while(1){
		/* Columns Loop */
		for(col = 0; col < N_COL; col++){

			/*
			 * Each time only 1 column will be active (Output)
			 * Other columns will be inactive (Input).
			 * Also, clear output register(PORT) to enable
			 * internal Pull up resistor for rows
			 */
			KEYPAD_PORT_DIR = (0x10 << col);

			KEYPAD_PORT_OUT = (~(0x10 << col));

			/* Rows Loop */
			for(row = 0; row < N_ROW; row++){

				/*
				 * We're using internal pull up, so
				 * when button pressed, its corresponding
				 * value on PINA Register should be 0
				 */
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN, row)){
					#if (N_COL == 3)
						return KeyPad_4x3_keyMapping( ((row*N_COL) + col + 1) );
					#elif(N_COL == 4)
						return KeyPad_4x4_keyMapping( ((row*N_COL) + col + 1) );
					#endif
				}
			}
		}
	}

}

/************************************************************************************
 * 							Functions Definitions (Private)							*
 ************************************************************************************/
#if (N_COL == 3)

/*
 * Description: Helper Function to know which button pressed on 4x3 keypad
 * @Param:		Pressed Button on HW Matrix
 * @Return:		Actual value of Pressed Button (uint8)
 */
static uint8 KeyPad_4x3_keyMapping(uint8 button){

	/* From 1 till 9 HW Matrix should be
	 * the same with actual Pressed button
	 */
	switch(button)
	{
	case 10:
		/* Return ASCII Code of (*) */
		return '*';
		break;
	case 11:
		return 0;
		break;

	case 12:
		/* Return ASCII Code of (*) */
		return '#';
		break;

	default:
		return button;
		break;
	}
}


#elif (N_COL ==4)
/*
 * Description: Helper Function to know which button pressed on 4x4 keypad
 * @Param:		Pressed Button on HW Matrix
 * @Return:		Actual value of Pressed Button (uint8)
 */
static uint8 KeyPad_4x4_keyMapping(uint8 button){

	switch(button){
	case 1: return 7;
			break;
	case 2: return 8;
			break;
	case 3: return 9;
			break;
	case 4: return '%'; // ASCII Code of %
			break;
	case 5: return 4;
			break;
	case 6: return 5;
			break;
	case 7: return 6;
			break;
	case 8: return '*'; /* ASCII Code of '*' */
			break;
	case 9: return 1;
			break;
	case 10: return 2;
			break;
	case 11: return 3;
			break;
	case 12: return '-'; /* ASCII Code of '-' */
			break;
	case 13: return 13;  /* ASCII of Enter */
			break;
	case 14: return 0;
			break;
	case 15: return '='; /* ASCII Code of '=' */
			break;
	case 16: return '+'; /* ASCII Code of '+' */
			break;
	default: return button;
	}
}
#endif
