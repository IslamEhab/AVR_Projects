/******************************************************************************
 *
 * Module: I2C
 *
 * File Name: i2c.c
 *
 * Description: Header file for the I2C AVR driver
 *
 * Author: Islam Ehab
 *
 * Date: 27/9/2020
 *
 * Version: 1.0.0
 *
 *************************************************************************************************/
#ifndef I2C_H_
#define I2C_H_


/****************************************** Includes *********************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/************************************************************************************************/


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* Preprocessor macros for Interrupt Mode */
#define INTERRUPT_MODE_ENABLE		(1U)
#define INTERRUPT_MODE_DISABLE		(0U)

/* Preprocessor macros for MC Address if it is in slave mode */
#define ADDRESS_OF_MC_IN_SLAVE_MODE	((0x02)U)

/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

/************************************************************************************************/

/********************************** User Define Data Types **************************************/


/********************************************************************************

 [Enumuration Name]:		TWI_BitRateType

 [Enumuration Description]: Enumerator that have all I2C Bit rate modes
 	 	 	 	 	 	 	to avoid confiusing while manipulating with alot of zeors

*********************************************************************************/
typedef enum{

	NORMAL_MODE_100KB 		= 100000,
	FAST_MODE_400KB	  		= 400000,
	FAST_MODE_PLUS_1MB		= 1000000,
	HIGH_SPEDD_MODE_3_4MB	= 3400000,

}TWI_BitRateType;

/********************************************************************************

 [Structure Name]:			TWI_ConfigType

 [Structure Description]:	Structure which will get I2C configuration
 	 	 	 	 	 	  	to initiailize the I2C properly

*********************************************************************************/

typedef struct{

	uint8 			Address;			/* You can find this value @ADDRESS_OF_MC_IN_SLAVE_MODE
										   if you are using this MC in slave mode and there is
										   NO OTHER DEVICES HAVE THE SAME ADDRESS
										   used to decide Address of device if it's a slave */

	TWI_BitRateType BitRate;			/* You can find this value @TWI_BitRateType
										   Value of bit rate required for I2C from programmer */

	uint8			InterruptMode;		/* You can find this value @INTERRUPT_MODE
	 	 	 	 	 	 	   	   	   	   Used to decied which operating mode is activated (Polling or Interrupt) */
}TWI_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
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
void TWI_init(const TWI_ConfigType* config_Ptr);



/********************************************************************************
 [Function Name]:	TWI_start

 [Description]:		Used to send start bit on I2C Frame

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void TWI_start(void);


/********************************************************************************
 [Function Name]:	TWI_stop

 [Description]:		Used to Send Stop bit on I2C Frame

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void TWI_stop(void);


/********************************************************************************
 [Function Name]:	TWI_write

 [Description]:		Used to write data/address on I2C Frame

 [Args]:			data

 [in]				data: Data/address to be written

 [out]				None

 [in/out]]			None

 [Returns]:]		None
**********************************************************************************/
void TWI_write(uint8 data);


/********************************************************************************
 [Function Name]:	TWI_readWithACK

 [Description]:		Perform a read operation with sending ACK

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Data which will be read
**********************************************************************************/
uint8 TWI_readWithACK(void);


/********************************************************************************
 [Function Name]:	TWI_readWithNACK

 [Description]:		Perform a read operation without sending ACK

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Data which will be read
**********************************************************************************/
uint8 TWI_readWithNACK(void);


/********************************************************************************
 [Function Name]:	TWI_getStatus

 [Description]:		return Frame Status

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Status of the bus
**********************************************************************************/
uint8 TWI_getStatus(void);



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
void TWI_setCallBack(void (*f_ptr)(void));

#endif /* I2C_H_ */
