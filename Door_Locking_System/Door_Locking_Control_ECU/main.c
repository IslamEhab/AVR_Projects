/* Control ECU */
/*
 * main.c
 *
 *  Created on: Sep 27, 2020
 *      Author: Islam Ehab
 */


/***********************************************************************
								Includes
 ***********************************************************************/
#include "micro_config.h"
#include "uart.h"
#include "timers.h"
#include "external_eeprom.h"
#include "Motors.h"

/***********************************************************************
 	 	 	 	 	 	 	 Preprocessor Macros
 ***********************************************************************/
#define M1_READY 							0x10
#define PASSWORD_NOT_SAVED					0x11
#define NO_PREVIOS_PASSWORDS 				0x20
#define PREVIOUS_PASSWORD_DETECTED			0x23
#define NEW_PASSWORD_TO_BE_SAVED			0x21
#define NEW_PASSWORD_SAVED_SUCCESSFULLY		0x22
#define OVER_WRITE_OLD_PASSWORD				0x30
#define STEP3_DOOR_MANIPULATION				0x40
#define CHECK_PASSWORD						0x41
#define CORRECT_PASSWORD					0x42
#define INCORRECT_PASSWORD					0x43
#define PASSWORD_WRONG_3_TIMES				0x44
#define DOOR_OPEN							0x45
#define DOOR_CLOSE							0x46


/************************************************************************
  	  	  	  	  	  	  	  Function Prototypes
 ***********************************************************************/
uint8 Read_eepromEmptyValue(void);
void Write_eepromValue(uint8* arr);
uint8 Compare_eepromMultipleValue(uint8* arr);
void EEPROM_writeNewPassword(void);
void EEPROM_overwritePassword(void);
void Check_passwordEEPROM(void);
void Timer1_INT(void);
void Theif_onTheSystem(void);
void Door_on(void);
void Door_off(void);

/************************************************************************
  	  	  	  	  	  	  	  Global Variables
 ***********************************************************************/

/* Counter to count 10 seconds */
volatile uint8 g_timer1_10SecFlag = 0;

/* Counter to count 1 Minute */
volatile uint8 g_timer1_1MinFlag = 0;

/* Flag to find the theif */
uint8 g_theifFlag = 0;


/************************************************************************
  	  	  	  	  	  	  	  Main Function
 ***********************************************************************/
int main(void)
{


	/* UART Structure for its configurations */
	UART_ConfigType uart_Struct;


	/* Timer Structure for timer 1 configurations */
	Timer_ConfigType timer1_Strut;

	uart_Struct.BaudRate			= 9600;
	uart_Struct.InterruptMode		= POLLING;
	uart_Struct.ParityMode			= NO_PARIT_BITS;
	uart_Struct.StopBit				= STOP_BIT_1_BIT;

	/* Timer1 Tick every 1 Second */
	timer1_Strut.TimerID 			= TIMER_ID_1;
	timer1_Strut.TimerMode			= CTC_MODE_CHANNEL_A;
	timer1_Strut.CompareMatchMode	= NORMAL;
	timer1_Strut.OutputPin			= NONE;
	timer1_Strut.Prescaler			= FCPU_OVER_1024;
	timer1_Strut.InitialValue		= 0;
	timer1_Strut.CompareValue		= 7900;

	/* Enable Global Interrupt */
	SREG |= (1<<7);

	/* Timer 1 Initialization */
	Timer_init(&timer1_Strut);

	/* CallBack Function for timer 1 CTC Channel A Interrupt Function */
	Timer1_setCallBack(Timer1_INT);

	/* Buzzer Configurations */
	DDRC |= (1 << PC2);
	PORTC &= ~(1<<PC2);

	/* Initialization of Motors */
	Motor_init();

	/* LED for INT Testing */
	DDRD |= (1 << PD7);

	/* UART initialization */
	UART_init(&uart_Struct);

	while(UART_recieveByte() != M1_READY){} // wait until MC2 is ready

	/* Init EEPROM */
	EEPROM_init();

	/*
	 * Initially, EEPROM Suppose to be empty (0xFF)
	 * so we need to put a password at first
	 */
	EEPROM_writeNewPassword();

	while(1)
	{

		/* Check if theif occured to start the program from first */
		if(g_theifFlag == 1){

			/* Wait till receivng the ACK From the HMI ECU */
			while(UART_recieveByte() != M1_READY);

			/* Check for received command if it is overwrite the old password or not */
			if(UART_recieveByte() == OVER_WRITE_OLD_PASSWORD){

				/*
				 * Go to check password given from HMI ECU
				 * with the old one in EEPROM
				 */
				do{
					/*
					 * Execute at least once
					 * Used to hault on it till enter the right password
					 */
					Check_passwordEEPROM();
				}while(g_theifFlag == 1);

				/* Write new password */
				EEPROM_overwritePassword();
			}

			/* Check for second command which is door manipulation */
			if(UART_recieveByte() == STEP3_DOOR_MANIPULATION){

				/*
				 * Go to check password given from HMI ECU
				 * with the old one in EEPROM
				 */
				do{
					/*
					 * Execute at least once
					 * Used to hault on it till enter the right password
					 */
					Check_passwordEEPROM();
				}while(g_theifFlag == 1);

				/* Check for receiving Door open command */
				if(UART_recieveByte() == DOOR_OPEN){

					/* Opening the door for 10 seconds */
					Door_on();
				}
				/* Closing the door for 10 seconds */
				Door_off();
			}
			else{
				/* Send error */
				UART_sendByte(ERROR);
			}

			/* Clear theif flag */
			g_theifFlag = 0;
		}

		else{
			/* Wait till receivng the ACK From the HMI ECU */
			while(UART_recieveByte() != M1_READY);

			/* Check for received command if it is overwrite the old password or not */
			if(UART_recieveByte() == OVER_WRITE_OLD_PASSWORD){

				/*
				 * Go to check password given from HMI ECU
				 * with the old one in EEPROM
				 */
				do{
					/*
					 * Execute at least once
					 * Used to hault on it till enter the right password
					 */
					Check_passwordEEPROM();
				}while(g_theifFlag == 1);

				/* Write new password */
				EEPROM_overwritePassword();
			}

			/* Check for second command which is door manipulation */
			if(UART_recieveByte() == STEP3_DOOR_MANIPULATION){

				/*
				 * Go to check password given from HMI ECU
				 * with the old one in EEPROM
				 */
				do{
					/*
					 * Execute at least once
					 * Used to hault on it till enter the right password
					 */
					Check_passwordEEPROM();
				}while(g_theifFlag == 1);

				/* Check for receiving Door open command */
				if(UART_recieveByte() == DOOR_OPEN){

					/* Opening the door for 10 seconds */
					Door_on();
				}
				/* Closing the door for 10 seconds */
				Door_off();
			}
			else{
				/* Send error */
				UART_sendByte(ERROR);
			}

			/* Clear theif flag */
			g_theifFlag = 0;
		}
	}
}

/********************************************************************************
 [Function Name]:	Door_on

 [Description]:		Used as a helper function to operate with door (open it)

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Door_on(void){

	uint8_t save_globalNow = 0;

	save_globalNow = g_timer1_10SecFlag;

	while(g_timer1_10SecFlag < (save_globalNow + 10)){
		Motor_clockWise();
	}

}

/********************************************************************************
 [Function Name]:	Door_off

 [Description]:		Used as a helper function to operate with door (close it)

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Door_off(void){

	/* Variable used to save global counter flag in ISR */
	uint8_t save_globalNow = 0;

	/* Getting the value of the Counter flag now */
	save_globalNow = g_timer1_10SecFlag;

	/*
	 * delay for 10 Seconds to close the door
	 * Here i check for the global counter flag value
	 * if less than the first value of it before a
	 * 1 second from this time + 10 seconds
	*/
	while(g_timer1_10SecFlag < (save_globalNow + 10)){

		/* Closing the door */
		Motor_antiClockWise();
	}
	/* Turn off the motors */
	Motor_off();
}


void Check_passwordEEPROM(void){

	/* Iteration variable */
	uint8 i = 0;

	/* Array to hold the password */
	uint8 PasswordArr[5] = {0};
	/*
	 * Variable to hold how many times fucntion called itself
	 * made static to hold its last value after recalling
	 * Not a good behavior to make the function call itself due to
	 * stack overflow problems, but here function call itself only twice
	 * so it wont be a big problem
	*/
	static uint8 Flag_of_Calling = 0;

	/* Wait untill HMI ECU get the password from the user */
	while(UART_recieveByte() != CHECK_PASSWORD);

	/* Recive the password byte by byte */
	for(i = 0; i < 5; i++){

		/*
		 * Recive the password byte by byte
		 * and save it on passwordArray
		 */
		PasswordArr[i] = UART_recieveByte();
	}

	/* Check if received password from HMI ECU is Correct or not */
	if (Compare_eepromMultipleValue(PasswordArr) == 5){

		g_theifFlag = 0;

		/* Correct Password */
		UART_sendByte(M1_READY);

		/* Sending an ACK TO HMI MCU to tell him that i will send commands */

		/* Sending an Correct Password ACK */
		UART_sendByte(CORRECT_PASSWORD);

	}
	else
	{
		/* Sending an ACK TO HMI MCU to tell him that i will send commands */
		UART_sendByte(M1_READY);

		/* Incorrect Password! */
		UART_sendByte(INCORRECT_PASSWORD);

		/* Increament calling flag */
		Flag_of_Calling++;

		/*
		 * Incorrect Password!!
		 * You Still Have 2 times
		 * of putting the password correctly
		 */
		if(Flag_of_Calling <= 2){


			/*
			 * Not the best practice because if user put the password
			 * wrong a lot of time, all this context switching
			 * may goes into stackoverflow.
			 * So, i should think about another way
			 */
			Check_passwordEEPROM();
		}
		else
		{
			/* Clearing the recalling flag */
			Flag_of_Calling = 0;
			/* Three times password wrong consecutively */

			/* Sending an ACK TO HMI MCU to tell him that i will send commands */
			UART_sendByte(M1_READY);

			/* Incorrect Password for 3 times ! */
			UART_sendByte(PASSWORD_WRONG_3_TIMES);

			/* Set the theif flag to 1*/
			g_theifFlag = 1;

			/* Turn On Alarm for 1 Minute */
			Theif_onTheSystem();

		}
	}

}

/********************************************************************************
 [Function Name]:	Theif_onTheSystem

 [Description]:		Used as a helper function to do the proper proceudre if there
 	 	 	 	 	is a theif in the system (Password is wrong 3 times consecutively)
 	 	 	 	 	This fuction will display a theif word on the lcd and a counter
 	 	 	 	 	of 1 minute with HMI ECU
 	 	 	 	 	and also a buzzer will turn on through the control ECU

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Theif_onTheSystem(void){

	/*
	 * You entered the password 3 times consecutively wrong
	 * Alarm will turn on for 1 min
	 */
	/* Variable used to save global counter flag in ISR */
	uint8_t save_globalNow = 0;

	/* Turn On Buzzer */
	PORTC |= (1 << PC2);

	/* Saving timer global variable now */
	save_globalNow = g_timer1_1MinFlag;

	/* Wait till 1 Min finishes */
	while(g_timer1_1MinFlag < (save_globalNow + 60)){
	}

	/* Turn Off Buzzer */
	PORTC &= (~(1 << PC2));
}

/********************************************************************************
 [Function Name]:	EEPROM_overwritePassword

 [Description]:		Used as a helper function to write the new password on the
 	 	 	 	 	EEPROM

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void EEPROM_overwritePassword(void){

	/* Array to hold the password */
	uint8 passwordArray[5] = {0};

	/* Iterator Variable */
	uint8 i = 0;

	/* Check if theif occured to start the program from first */
	if(g_theifFlag == 1){

		/* Clear the flag */
		g_theifFlag = 0;

	}

	/* Wait untill HMI ECU get the password from the user */
	while(UART_recieveByte() != NEW_PASSWORD_TO_BE_SAVED);

	/* Recive the password byte by byte */
	for(i = 0; i < 5; i++){

		/*
		 * Recive the password byte by byte
		 * and save it on passwordArray
		 */
		passwordArray[i] = UART_recieveByte();
	}

	/* Start saving the password on EEPROM */
	Write_eepromValue(passwordArray);

	/* Sending to HMI ECU an ACK */
	UART_sendByte(M1_READY);

	/* Check if byte saved successfully or not */
	if(Compare_eepromMultipleValue(passwordArray) == 5){

		/* Send an ACK to HMI ECU That password saved successfully */
		UART_sendByte(NEW_PASSWORD_SAVED_SUCCESSFULLY);

	}

	else{
		/* Byte did NOT Saved successfully */
		UART_sendByte(PASSWORD_NOT_SAVED);
	}

}

/********************************************************************************
 [Function Name]:	EEPROM_writeNewPassword

 [Description]:		Used as a helper function to check if there is a previos
 	 	 	 	 	password detected or not, if not so save the new one

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void EEPROM_writeNewPassword(void){

	/*
	 * Read First 5 Bytes at first
	 * to check if there is no previos
	 * passwords saved
	 * Default value = 0xFF
	 */

	/* Check if 5 bytes are all equal to 0xFF */
	if(	Read_eepromEmptyValue() == 5){

		/* Send an ACK to HMI ECU to know that there is not a previous passowrd */
		UART_sendByte(NO_PREVIOS_PASSWORDS);

		/* Write the password */
		EEPROM_overwritePassword();
	}
	else{

		/* There is an old password detected ACK */
		UART_sendByte(PREVIOUS_PASSWORD_DETECTED);

	}

}

/********************************************************************************
 [Function Name]:	Read_eepromEmptyValue

 [Description]:		Used as a helper function to read the empty value (0xFF)

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			Count of emptyflag (it should be 5 if all 5 slots are empty)
 **********************************************************************************/
uint8 Read_eepromEmptyValue(void){

	uint8 valArray[5] = {0};
	uint8 i = 0;
	uint8 eepromEmptyFlag = 0;

	//Read byte by byte from EEPROM
	EEPROM_readByte(0x0311, &valArray[0]);
	_delay_ms(10);

	EEPROM_readByte(0x0312, &valArray[1]);
	_delay_ms(10);

	EEPROM_readByte(0x0313, &valArray[2]);
	_delay_ms(10);

	EEPROM_readByte(0x0314, &valArray[3]);
	_delay_ms(10);

	EEPROM_readByte(0x0315, &valArray[4]);
	_delay_ms(10);

	for(i = 0 ;i < 5; i++){
		if(valArray[i] == 255){
			eepromEmptyFlag += 1;
		}
	}


	return eepromEmptyFlag;
}

/********************************************************************************
 [Function Name]:	Write_eepromValue

 [Description]:		Used as a helper function to write the value needed on EEPROM
 	 	 	 	 	Byte by byte HARD CODED

 [Args]:			arr

 [in]				arr: Pointer to unsigned char which has the value needed
 	 	 	 	 	to be saved

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Write_eepromValue(uint8* arr){

	/* Write Password Byte by byte */
	EEPROM_writeByte((0x0311), arr[0]);

	_delay_ms(10);

	/* Write Password Byte by byte */
	EEPROM_writeByte((0x0312), arr[1]);

	_delay_ms(10);


	/* Write Password Byte by byte */
	EEPROM_writeByte((0x0313), arr[2]);

	_delay_ms(10);

	/* Write Password Byte by byte */
	EEPROM_writeByte((0x0314), arr[3]);

	_delay_ms(10);


	/* Write Password Byte by byte */
	EEPROM_writeByte((0x0315), arr[4]);

	_delay_ms(10);
}

/********************************************************************************
 [Function Name]:	Compare_eepromMultipleValue

 [Description]:		Used as a helper function compare a given value with the value
 	 	 	 	 	saved now on EEPROM

 [Args]:			arr

 [in]				arr: Pointer to unsiged char which has the values to be compared

 [out]				None

 [in/out]			None

 [Returns]:			Count of equal Flag (it should be 5 if all 5 slots are the same)
 **********************************************************************************/
uint8 Compare_eepromMultipleValue(uint8* arr){

	uint8 valArray[5] = {0};
	uint8 i = 0;
	uint8 eepromEqualityFlag = 0;
	//Read byte by byte from EEPROM
	EEPROM_readByte(0x0311, &valArray[0]);
	_delay_ms(10);

	EEPROM_readByte(0x0312, &valArray[1]);
	_delay_ms(10);

	EEPROM_readByte(0x0313, &valArray[2]);
	_delay_ms(10);

	EEPROM_readByte(0x0314, &valArray[3]);
	_delay_ms(10);

	EEPROM_readByte(0x0315, &valArray[4]);
	_delay_ms(10);

	for(i = 0 ;i < 5; i++){
		if(valArray[i] == arr[i]){
			eepromEqualityFlag += 1;
		}
	}
	return eepromEqualityFlag;
}

/********************************************************************************
 [Function Name]:	Timer1_INT

 [Description]:		Used as the Handler function givin to Timer1 CTC Channel A ISR
 	 	 	 	 	by callback technique

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Timer1_INT(void){

	/* To count seconds increament till 10 */
	g_timer1_10SecFlag++;

	/* Counter to count 1 Minute */
	g_timer1_1MinFlag++;

	PORTD ^= (1 << 7);

}
