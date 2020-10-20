/******************************************************************************
 *
 * Module: I2C
 *
 * File Name: i2c.c
 *
 * Description: Source file for the I2C AVR driver
 *
 * Author: Islam Ehab
 *
 * Date: 27/9/2020
 *
 * Version: 1.0.0
 *
 *******************************************************************************/


#include "i2c.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/* Global Variable to hold address of the call back funtion of TWI Interrupt */
static volatile void (*g_callBackPtrTWI)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TWI_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrTWI != NULL_PTR){

		/* Call the Call Back function for TWI Interrupt */
		(*g_callBackPtrTWI)();		/* Another way of calling g_callBackPtrTWI(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/********************************************************************************
 [Function Name]:	TWI_init

 [Description]:		Used to initialize I2C Module
 	 	 	 	 	- 1 pre-scaler to ease bit rate calculations
 	 	 	 	 	- Initiate with device address (If not a microcontroller or master)
 	 	 	 	 	  if master, its address will be 0x01
 	 	 	 	 	- Enable I2C Module

 [Args]:			config_Ptr

 [in]				config_Ptr: Pointer to Timer Configuration Structure

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void TWI_init(const TWI_ConfigType* config_Ptr){

	uint8 bitRateCalculation;
	/* Clear all registers before doing  anything */
	TWBR &= 0x00;
	TWCR &= 0x00;
	TWDR &= 0x00;
	TWAR &= 0x00;

	/* Bit rate calculations */
	bitRateCalculation = ( ( (F_CPU) - ( (config_Ptr -> BitRate) * 16) ) / (2 * (config_Ptr -> BitRate) ) );

	/* Check if number is Bigger than zero or not */
	if(bitRateCalculation >= 0){

		/*Bit Rate calculation proper with this clock*/
		TWBR = bitRateCalculation;
	}

	/*
	 * Check if BitRate calculation value < 0
	 * Also check if processor clock is bigger than 1 MHz
	 * which is the proper one for calculations here
	*/
	else if ( (F_CPU > 1000000) && (bitRateCalculation < 0) )
	{
		/* Reduce BitRate to Normal Mode  */
		bitRateCalculation = ( ( (F_CPU) - ( (NORMAL_MODE_100KB) * 16) ) / (2 * (NORMAL_MODE_100KB) ) );
		TWBR = bitRateCalculation;
	}

	/* Zero Prescaler */
	TWSR = 0x00;

    /*
     * Two Wire Bus address my address
     * if any master device want to call me: 0x1
     * (used in case this MC is a slave device)
     *  General Call Recognition: Off
    */
    TWAR = ( (config_Ptr -> Address) & 0xFE);

    /* Choose Operating mode (Interrupt or polling) */
    TWCR = ( (config_Ptr -> InterruptMode) );

    /* Enable TWI Module */
    TWCR = (1<<TWEN);

}



/********************************************************************************
 [Function Name]:	TWI_start

 [Description]:		Used to send start bit on I2C Frame

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void TWI_start(void){

    /*
	 * - Clear the TWINT flag before sending the start bit TWINT=1
	 * - send the start bit by TWSTA=1
	 * - Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}



/********************************************************************************
 [Function Name]:	TWI_stop

 [Description]:		Used to Send Stop bit on I2C Frame

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void TWI_stop(void){

    /*
	 * - Clear the TWINT flag before sending the stop bit TWINT=1
	 * - send the stop bit by TWSTO=1
	 * - Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


/********************************************************************************
 [Function Name]:	TWI_write

 [Description]:		Used to write data/address on I2C Frame

 [Args]:			data

 [in]				data: Data/address to be written

 [out]				None

 [in/out]]			None

 [Returns]:]		None
**********************************************************************************/
void TWI_write(uint8 data){

	/* Put data On TWI data Register */
    TWDR = data;

    /*
	 * - Clear the TWINT flag before sending the data TWINT=1
	 * - Enable TWI Module TWEN=1
	*/
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}



/********************************************************************************
 [Function Name]:	TWI_readWithACK

 [Description]:		Perform a read operation with sending ACK

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Data which will be read
**********************************************************************************/
uint8 TWI_readWithACK(void){

	/*
	 * - Clear the TWINT flag before reading the data TWINT=1
	 * - Enable sending ACK after reading or receiving data TWEA=1
	 * - Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));

    /* Read Data */
    return TWDR;
}



/********************************************************************************
 [Function Name]:	TWI_readWithNACK

 [Description]:		Perform a read operation without sending ACK

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Data which will be read
**********************************************************************************/
uint8 TWI_readWithNACK(void){

	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));

    /* Read Data */
    return TWDR;
}



/********************************************************************************
 [Function Name]:	TWI_getStatus

 [Description]:		return Frame Status

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Status of the bus
**********************************************************************************/
uint8 TWI_getStatus(void){

    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;

    return status;
}



/********************************************************************************
 [Function Name]:	TWI_setCallBack

 [Description]:		Function Callback to serve TWI ISR Code

 [Args]:			f_ptr

 [in]				f_ptr: Pointer to void function
					which will save address of ISR Handler Function in main

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void TWI_setCallBack(void (*f_ptr)(void)){
	g_callBackPtrTWI = f_ptr;
}
