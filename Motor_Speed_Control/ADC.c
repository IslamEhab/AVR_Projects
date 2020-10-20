 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC.c
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

#include "ADC.h"

#ifdef ADC_INTERRUPT_MODE
/*
 * Global Variable used to hold ADC Result after conversion
 * extern used in ADC.h because this variable will be used in main
 * to calculate temperature value from it
 */
volatile uint16 g_adcResult;


/*
 * Global pointer to void function to hold address of call back function
 * static used to make this pointer file scope only
 */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
#endif

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
void ADC_init(const ADC_ConfigType * Config_Ptr){

	/* Clearing Register at first */
	ADMUX  &= 0x00;
	ADCSRA &= 0x00;
	SFIOR  &= 0x1F;

	/* Configure Reference Voltage of ADC */
	ADMUX  |= ( (Config_Ptr -> Voltage) << 6);

	/* Configure Auto Trigger Mode */
	ADCSRA |= ( (Config_Ptr -> AutoTrigger) << 5);

	/* Configure Operation Mode (Polling, Interrupt) */
	ADCSRA |= ( (Config_Ptr -> OperatingMode) << 3);

	/* Configure Prescaler Value */
	ADCSRA |= ( (Config_Ptr -> Prescaler) );

	/* Configure AutoTrigger Source */
	SFIOR  |= ( (Config_Ptr -> AutoTriggerSource) << 5 );

	/* ADC Enable */
	ADCSRA |= (1<<7);
}


#ifdef ADC_INTERRUPT_MODE

/*
 * Description: ISR OF ADC
 * @Param:		ISR Name
 * @Return:		None
 */
ISR(ADC_vect){

	g_adcResult = ADC;
/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault

	if(g_callBackPtr != NULL_PTR){

		 Call the Call Back function
		*(g_callBackPtr)();
	}*/
}

/*
 * Description: Function to initiate reading ADC Channel
 * @Param:		ch_Num: Channel Number (0 -> 7)
 * @Return:		None (O/P of conversion is on ADC Interrupt)
 */
void ADC_interruptReadChannel(uint8 ch_Num){

	/* Make Sure that Ch_Num is not more than 7 */
	ch_Num &= 0x07;

	/* Clearing the channel Number Bits */
	ADMUX &= ~(0x1F);

	/* Put channel for reading  */
	ADMUX |= ( (ch_Num) );

	/* Start Conversion */
	ADCSRA |= (1<<ADSC);

}

/*
 * Description: Function to set the Call Back function address
 * 				Used in Interrupt Situation
 * @Param:		Pointer to void Function
 * @Retrun:		None
 */
void ADC_setCallBack(void (*f_ptr)(void)){

	g_callBackPtr = f_ptr;
}


#endif

#ifdef ADC_POLLING_MODE
/*
 * Description: Function to start reading ADC Channel
 * @Param:		ch_Num: Channel Number (0 -> 7)
 * @Return:		Result: O/P of ADC conversion (uint16)
 */
uint16 ADC_readChannel(uint8 ch_Num){
	/* Make Sure that Ch_Num is not more than 7 */
	ch_Num &= 0x07;

	/* Clearing the channel Number Bits */
	ADMUX &= ~(0x1F);

	/* Put channel for reading  */
	ADMUX |= ( (ch_Num) );

	/* Start Conversion */
	ADCSRA |= (1<<ADSC);

	/* Polling for conversion */
	while(BIT_IS_CLEAR(ADCSRA, ADIF));

	/* Clear flag by putting 1 on it */
	ADCSRA |= (1<<ADIF);

	return ADC;

}

#endif
