/*
 * main.c
 *
 *  Created on: Sep 22, 2020
 *      Author: Islam Ehab
 */

/******************************************* Includes *********************************************/

#include "EXTI.h"
#include "ADC.h"
#include "Timers.h"
#include "LCD.h"
#include "Motors.h"
/************************************************************************************************/


/********************************* Global Variables Definition **********************************/
/* Global Flag to indicate if INT1 Pressed or not */
uint8 g_flag = 0;

/************************************************************************************************/


/************************************* Functions Prototype **************************************/
void INT1_Application(void);

/************************************************************************************************/


int main(void){

	/* Variable to Calculate Duty Cycle */
	uint8 DutyCycle = 0;

	/* Enable Global Interrupt */
	SREG |= (1<<7);

	/* Variable of type ADC Configuration Structure  */
	ADC_ConfigType 	 ADC_Config;

	/* Variable of type Timer Configuration Structure  */
	Timer_ConfigType Timer_Config;

	ADC_Config.AutoTrigger 			= DISABLE_AUTO_TRIGGER_MODE;
	ADC_Config.Voltage 				= AREF;
	ADC_Config.OperatingMode		= INTERRPUT_OPERATING_MODE;
	ADC_Config.Prescaler			= EIGHT;


	Timer_Config.TimerMode 			= PWM_TIMER_MODE;
	Timer_Config.CompareMatchMode	= CLEAR_ON_COMPARE_NON_INVERTING;
	Timer_Config.WaveformMode 		= FAST_PWM;
	Timer_Config.Clock				= FCPU_OVER_8;
	Timer_Config.InitialValue 		= 0;
	Timer_Config.CompareValue		= 0;	/* Start with 10%*/

	/* Send Address of ISR Application function to ISR using call back technique */
	INT1_setCallBack(INT1_Application);

	/* Interrupt 1 (PD3) Initialization */
	INT1_init();

	/* LCD Initialization */
	LCD_Init();

	/* ADC Initialization (Interrupt Mode) */
	ADC_init(&ADC_Config);

	/* Timer0 Initialization */
	Timer0_init(&Timer_Config);

	/* Motors Initialization */
	Motor_init();

	/* Go to next Row of LCD */
	LCD_goToRowColumn(0,0);

	LCD_displayString("ADC Value = ");

	/* Shift to Pixel 10 to display celsius unit */
	LCD_goToRowColumn(0,11);


	while(1){

		/* Initiate Reading operation (Without getting the digital value) */
		ADC_interruptReadChannel(CHANNEL0);

		/* Converting from 10 bit ADC Reading to only 8 bits to give it to PWM */
		DutyCycle = ((uint8)( (g_adcResult & 0x3FF) / 4));

		/* Still investigating about subtracting 255 from actual duty cycle value */
		PWM_setDutyCycle((255-DutyCycle));

		/* Checking of Global Flag to indicate if INT1 Pressed or not */
		if(g_flag == 1){

			/* If pressed move motor anti clock wise */
			Motor_clockWise();
		}

		else
		{
			/* Not pressed, move clock wise */
			Motor_antiClockWise();
		}

		/*
		 * If the number is less than 1000
		 * there is 4 displayed due to last value
		 * of g_adcResult
		 * So, here it will replaced with space
		*/
		if(g_adcResult < 1000){

			LCD_goToRowColumn(0,15);
			LCD_displayCharacter(' ');
		}

		/*
		 * View Potentiometer value using
		 * integer to string function
		 */
		LCD_goToRowColumn(0,12);

		/* Preview ADC Value using integer to string API */
		LCD_intgerToString((uint32)g_adcResult);
	}
}

/*
 * Description: Application function of INT1
 * @Param:		None
 * @Return:		None
*/
void INT1_Application(void){
	g_flag ^= 1;
}


