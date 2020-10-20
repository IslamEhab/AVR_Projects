 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
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


#include "uart.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function for UART Rx Interrupt in the application */
static volatile void (*g_callBackPtrUartRx)(void) = NULL_PTR;

/* Global variables to hold the address of the call back function for UART Tx Interrupt in the application */
static volatile void (*g_callBackPtrUartTx)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(USART_RXC_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrUartRx != NULL_PTR){

		 /* Call the Call Back function for UART Rx Interrupt */
		(*g_callBackPtrUartRx)();
	}
}

ISR(USART_TXC_vect){

	/*
	 * Check if the Pointer not a Null one
	 * To avoid segmentation fault
	*/
	if(g_callBackPtrUartRx != NULL_PTR){

		 /* Call the Call Back function for UART Tx Interrupt */
		(*g_callBackPtrUartTx)();
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
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
void UART_init(const UART_ConfigType* config_Ptr)
{
	/* Initialize All registers with zeros */
	UCSRA &= 0x00;
	UCSRB &= 0x00;
	UCSRC &= 0x00;
	UBRRH &= 0x00;
	UBRRL &= 0x00;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1 << U2X);

	/* Enable UART Tx, Rx  */
	UCSRB = (1 << RXEN) | (1 << TXEN);

	/* Chose Operating Mode (Tx Interrupt, Rx Interrupt, both, or None) */
	UCSRB = ( (UCSRB & 0x3F) | ( (config_Ptr -> InterruptMode) << TXCIE) );

	/*
	 * Enable UCSRC Register
	 * Choose Asyncronous Mode
	*/
	UCSRC = (1 << URSEL);

	/* Choose Parity Mode (Disabled, Even, or odd parity) */
	UCSRC = ( (UCSRC & 0xCF) | ( (config_Ptr -> ParityMode) << UPM0) );

	/* Choose Stop bits number (1 bit or 2 bits) */
	UCSRC = ( (UCSRC & 0xF7) | ( (config_Ptr -> StopBit) << USBS) );

	/* Choose 8 bit mode */
	UCSRC = ( (UCSRC & 0xF9) | (EIGHT_BIT_CHAR_SIZE << UCSZ0) );

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ( ( ( (F_CPU / ( (config_Ptr -> BaudRate)* 8UL) ) ) - 1) >> 8 );
	UBRRL = ( ( (F_CPU / ( (config_Ptr -> BaudRate)* 8UL) ) ) - 1);
}



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
void UART_setCallBackTx(void (*f_ptr)(void)){
	g_callBackPtrUartTx = f_ptr;
}



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
void UART_setCallBackRx(void (*f_ptr)(void)){
	g_callBackPtrUartRx = f_ptr;
}



/********************************************************************************
 [Function Name]:	UART_sendByte

 [Description]:		Function used to send byte through UART communication protocol

 [Args]:			data

 [in]				data: constant unsigned char which has the byte to be sent

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void UART_sendByte(const uint8 data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}



/********************************************************************************
 [Function Name]:	UART_recieveByte

 [Description]:		Function used to receive a byte through UART Communication Protocol

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Byte recived by UART
**********************************************************************************/
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
    return UDR;
}



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
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/
}



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
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}
