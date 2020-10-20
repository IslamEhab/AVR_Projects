 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Islam Ehab
 *
 * Date: 27/9/2020
 *
 * Version: 1.0.0
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_



/****************************************** Includes *********************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/************************************************************************************************/


/******************************************* Macros *********************************************/

#define STOP_BIT_1_BIT			(0U)
#define STOP_BIT_2_BITS			(1U)

#define EIGHT_BIT_CHAR_SIZE 	(3U)

/************************************************************************************************/


/********************************** User Define Data Types **************************************/


/********************************************************************************

 [Enumuration Name]:		UART_InterruptModeType

 [Enumuration Description]: Used to decide Operating mode
 	 	 	 	 	 	    (Polling, Interrupt when Tx, Interrupt when Rx,
							or both INT)

*********************************************************************************/
typedef enum{

	POLLING,
	TX_INT_ENABLE,
	RX_INT_ENABLE,
	TX_RX_INT_ENABLE

}UART_InterruptModeType;


/********************************************************************************

 [Enumuration Name]:		UART_ParityModeType

 [Enumuration Description]: Used to decide Parity bit mode
 	 	 	 	 	 	   (Even parity, odd parity, or disabled)

*********************************************************************************/
typedef enum{

	NO_PARIT_BITS,
	EVEN_PARITY = 2,
	ODD_PARITY

}UART_ParityModeType;

/********************************************************************************

 [Structure Name]: 		  UART_ConfigType

 [Structure Description]: Structure which will get UART configuration
 	 	 	 	 	 	  to initiailize the UART properly

*********************************************************************************/
typedef struct{

	UART_InterruptModeType		InterruptMode;		/* You can find this value @UART_InterruptModeType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide Operating mode (INT with TX, RX, Both or Polling Mode) 	*/

	uint8 						StopBit;			/* You can find this value @STOP_BIT
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide How many bits for Stop Bit indicatior on the Frame 		*/

	UART_ParityModeType			ParityMode;			/* You can find this value @UART_ParityModeType
	 	 	 	 	 	 	 	 	 	 	 	 	   Used to decide Parity bit mode (Even, odd parity , or disabled) 			*/

	uint32						BaudRate;			/* Baud rate used with UART Operations
	 	 	 	 	 	 	 	 	 	 	 	 	   it vary from 9600 till 115200 or even 921600 with laptop using USB-TTL	*/

}UART_ConfigType;

/***********************************************************************************************/


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/********************************************************************************
 [Function Name]:	UART_init

 [Description]:		Used to initialize UART Module
 	 	 	 	 	- Choose BaudRate prefered
 	 	 	 	 	- Decide Stop bits number
 	 	 	 	 	- Decide operating mode (Tx Interrupt, Rx Interrupt, both, or polling mode)
 	 	 	 	 	- Decide Parity  mode	(Even, Odd, or no parity)
 	 	 	 	 	- Enable Tx, Rx both
					- 8 bit Data transfer mode
					- Asynchronous Mode (UART not USART)

 [Args]:			config_Ptr

 [in]				config_Ptr: Pointer to Timer Configuration Structure

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void UART_init(const UART_ConfigType* config_Ptr);



/********************************************************************************
 [Function Name]:	UART_setCallBackRx

 [Description]:		Function Callback to serve UART Rx ISR Code

 [Args]:			f_ptr

 [in]				f_ptr: Pointer to void function
					which will save address of ISR Handler Function in main

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void UART_setCallBackRx(void (*f_ptr)(void));



/********************************************************************************
 [Function Name]:	UART_setCallBackTx

 [Description]:		Function Callback to serve UART Tx ISR Code

 [Args]:			f_ptr

 [in]				f_ptr: Pointer to void function
					which will save address of ISR Handler Function in main

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void UART_setCallBackTx(void (*f_ptr)(void));



/********************************************************************************
 [Function Name]:	UART_sendByte

 [Description]:		Function used to send byte through UART communication protocol

 [Args]:			data

 [in]				data: constant unsigned char which has the byte to be sent

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void UART_sendByte(const uint8 data);



/********************************************************************************
 [Function Name]:	UART_recieveByte

 [Description]:		Function used to receive a byte through UART Communication Protocol

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Byte recived by UART
**********************************************************************************/
uint8 UART_recieveByte(void);



/********************************************************************************
 [Function Name]:	UART_sendString

 [Description]:		Function used to send byte through UART communication protocol

 [Args]:			Str

 [in]				Str: Constant pointer to unsigned char which has the string needed
 	 	 	 	 	 	 to be sent. It will be sending string till before Null terminator

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void UART_sendString(const uint8 *Str);



/********************************************************************************
 [Function Name]:	UART_receiveString

 [Description]:		Function used to receive a string through UART

 [Args]:			Str

 [in]				Str: Pointer to a unsigned char which is and Empty string
 	 	 	 	 	Which will be filled through this function

 	 	 	 	 	String will be filled till '#' sign which is for me an indicator
 	 	 	 	 	that sender ended his string. Sender should put '#' at the end
 	 	 	 	 	of every string sent by this function.

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void UART_receiveString(uint8 *Str); // Receive until #


#endif /* UART_H_ */
