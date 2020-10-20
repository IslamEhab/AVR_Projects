 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC.h
 *
 * Description: Source file for the AVR ADC driver
 *
 * Author: Islam Ehab
 *
 * Date: 22/9/2020
 *
 * Version:	1.0.0
 *
 *******************************************************************************/


#ifndef ADC_H_
#define ADC_H_

/****************************************** Includes *********************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/************************************************************************************************/

/****************************************************************************************************************
 * 											Temperature Calculation												*
 * 																												*
 * 								( ADC Result * MAX Temperature * Vref of ADC )									*
 * Temp = --------------------------------------------------------------------------------------------			*
 * 			(Vmax of sensor (Max O/P Voltage of Sensor) * ADC Resolution - 1 (2 ^ 10 in avr - 1) )				*
 *																												*
 *																												*
 * 										Any Sensor Reading Calculation											*
 *																												*
 * 							(ADC Result * MAX Physical Value sensor can sense * Vref of ADC )					*
 * Digital Value = ------------------------------------------------------------------------------------------	*
 *					(Vmax of sensor (Max O/P Voltage of Sensor) * ADC Resolution - 1 (2 ^ 10 in avr - 1) ) 		*
 ****************************************************************************************************************/

/******************************************* Macros *********************************************/


#define ADC_INTERRUPT_MODE

/*
 * If you will use Interrupt Mode
 * Copy this line in ADC.h
 * #define ADC_INTERRUPT_MODE
 * Otherwise, Polling mode works by default
 */
#ifndef ADC_INTERRUPT_MODE
#define ADC_POLLING_MODE
#endif

#define INTERRPUT_OPERATING_MODE		(1U)
#define POLLING_OPERATING_MODE			(0U)

#define ENABLE_AUTO_TRIGGER_MODE		(1U)
#define DISABLE_AUTO_TRIGGER_MODE		(0U)

/************************************************************************************************/

/*************************************** Global Variables  **************************************/

/*
 * Global Variable used to hold ADC Result after conversion
 * extern used because this variable will be used in main
 * to calculate temperature value from it
 */
extern volatile uint16 g_adcResult;

/************************************************************************************************/

/********************************** User Define Data Types **************************************/

typedef enum{

	AREF,
	AVCC,
	INTERNAL_2_56V = 3

}ADC_VoltageRefType;

typedef enum{

	TWO,
	FOUR = 2,
	EIGHT,
	SIXTEEN,
	THIRTYTWO,
	SIXTYFOUR,
	ONEHUNDRADE_TWENTY_EIGHT

}ADC_PrescalerType;

typedef enum{

	FREE_RUNNING,
	ANALOG_COMPARATOR,
	EXTI0,
	TIMER0_COMPARE,
	TIMER0_OVERFLOW,
	TIMER1_COMPARE,
	TIMER1_OVERFLOW,
	TIMER1_CAPTURE

}ADC_AutoTriggerSourceType;


typedef enum{

	CHANNEL0,
	CHANNEL1,
	CHANNEL2,
	CHANNEL3,
	CHANNEL4,
	CHANNEL5,
	CHANNEL6,
	CHANNEL7

}ADC_Channels;

/*
 * Structure to hold ADC Configuration
 */
typedef struct{

	ADC_VoltageRefType			Voltage;			/* You can find values of this @ADC_VoltageRefType
													    Used to decide ADC Operational Voltage */

	uint8						AutoTrigger;		/* You can find values of this @TRIGGER_MODE
	 	 	 	 	 	 	 	 	 	 	 	 	    Used to Enable ADC Auto Trigger option */

	uint8						OperatingMode;		/* You can find values of this @OPERATING_MODE
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide ADC Operating mode (Polling or Interrupt) */

	ADC_PrescalerType			Prescaler;			/* You can find Values of this @ADC_PrescalerType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide ADC Prescaler */

	ADC_AutoTriggerSourceType	AutoTriggerSource;	/* You can find Values of this @ADC_AutoTriggerSource
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide ADC Auto Trigger Source */

}ADC_ConfigType;

/************************************************************************************************/



/************************************* Functions Prototype **************************************/


/*
 * Description: Function to Initiate ADC Driver
 * 				- Decide Reference Voltage
 * 				- Auto Trigger Mode (Disabled / Enabled)
 * 				- Operational Mode  (Polling / Interrupt)
 * 				- Prescaler Value
 * 				- Auto Trigger Source (if Enabled)
 * 				- ADC Enable
 * 				- No Left Adjustment mode
 * @Param:		Config_Ptr: Pointer to ADC Configuration Structure
 * @Return:		None
 */
void ADC_init(const ADC_ConfigType * Config_Ptr);


#ifdef ADC_INTERRUPT_MODE
/*
 * Description: Function to initiate reading ADC Channel
 * @Param:		ch_Num: Channel Number (0 -> 7)
 * 				You can find this variable @ADC_Channels
 * @Return:		None (O/P of conversion is on ADC Interrupt)
 */
void ADC_interruptReadChannel(uint8 ch_Num);

/*
 * Description: Function to set the Call Back function address
 * 				Used in Interrupt Situation
 * @Param:		Pointer to void Function
 * @Retrun:		None
 */
void ADC_setCallBack(void (*f_ptr)(void));


#endif

#ifdef ADC_POLLING_MODE
/*
 * Description: Function to start reading ADC Channel
 * @Param:		ch_Num: Channel Number (0 -> 7)
 * 				You can find this variable @ADC_Channels
 * @Return:		Result: O/P of ADC conversion (uint16)
 */
uint16 ADC_readChannel(uint8 ch_Num);

#endif

/************************************************************************************************/

#endif /* ADC_H_ */
