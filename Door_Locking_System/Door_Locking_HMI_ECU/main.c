//HMI
/*
 * main.c
 *
 *  Created on: Sep 27, 2020
 *      Author: Islam Ehab
 */

/***********************************************************************
								Includes
 ***********************************************************************/
#include "timers.h"
#include "uart.h"
#include "LCD.h"
#include "keypad.h"

/***********************************************************************
 	 	 	 	 	 	 	 Preprocessor Macros
 ***********************************************************************/
#define M1_READY 							0x10
#define NO_PREVIOS_PASSWORDS 				0x20
#define NEW_PASSWORD_TO_BE_SAVED			0x21
#define NEW_PASSWORD_SAVED_SUCCESSFULLY		0x22
#define PREVIOUS_PASSWORD_DETECTED			0x23
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
void Timer1_INT(void);
void Step1_setPassword(void);
void Step2_mainCode(void);
void Step3_doorManipulation(void);
uint8 Enter_password(uint8* arr1, uint8* arr2);
void New_passwordCorrectCode(uint8* arr);
void Password_check(void);
void Theif_onTheSystem(void);
void Door_off(void);
void Door_on(void);

/************************************************************************
  	  	  	  	  	  	  	  Global Variables
 ***********************************************************************/

/* Counter to count 10 seconds */
volatile uint8 g_timer1_10SecFlag = 0;

/* Counter to count 1 Minute */
volatile uint8 g_timer1_1MinFlag = 0;

/* Flag to find the theif */
uint8 g_theifFlag = 0;

/* Flag to step 2 finishing the code */
uint8 g_step2FinishFlag = 0;

/* Flag to step 1 finishing the code */
uint8 g_step1FinishFlag = 0;


/************************************************************************
  	  	  	  	  	  	  	  Main Function
 ***********************************************************************/
int main(void){

	/* Timer Structure for timer 1 configurations */
	Timer_ConfigType timer1_Strut;

	/* UART Structure for its configurations */
	UART_ConfigType uart_Struct;

	/* Timer1 Tick every 1 Second */
	timer1_Strut.TimerID 			= TIMER_ID_1;
	timer1_Strut.TimerMode			= CTC_MODE_CHANNEL_A;
	timer1_Strut.CompareMatchMode	= NORMAL;
	timer1_Strut.OutputPin			= NONE;
	timer1_Strut.Prescaler			= FCPU_OVER_1024;
	timer1_Strut.InitialValue		= 0;
	timer1_Strut.CompareValue		= 7900;

	uart_Struct.BaudRate			= 9600;
	uart_Struct.InterruptMode		= POLLING;
	uart_Struct.ParityMode			= NO_PARIT_BITS;
	uart_Struct.StopBit				= STOP_BIT_1_BIT;

	/* LCD Initialization */
	LCD_Init();

	/* Led for Testing */
	DDRD |= (1 << PD7);

	/* Initial State OFF */
	PORTD &= ~(1 << PD7);

	/* Enable Global Interrupt */
	SREG |= (1<<7);

	/* Timer 1 Initialization */
	Timer_init(&timer1_Strut);

	/* CallBack Function for timer 1 CTC Channel A Interrupt Function */
	Timer1_setCallBack(Timer1_INT);

	/* UART Initialization */
	UART_init(&uart_Struct);


	/* Send an ACK For other MC to start UART */
	UART_sendByte(M1_READY) ;

	/* Start from first Pixel */
	LCD_goToRowColumn(0,0);


	LCD_clearScreen();

	LCD_displayString((uint8*)"Application Code");

	_delay_ms(500);

	/* Check if there are no previos password saved */
		if(UART_recieveByte() == NO_PREVIOS_PASSWORDS)
		{
			/*
			 * Go to Step 1 of Application code
			 * - Set New Password
			 * - Set it again
			 * - Check if both passwords are same or not
			 */
			do{
				/*
				 * Execute at least once
				 * if the password is incorrect
				 * you will hault here
				 */
				Step1_setPassword();
			}while(g_step1FinishFlag == 1);
		}

		/* There is a previos password saved */
		else if(UART_recieveByte() == PREVIOUS_PASSWORD_DETECTED){
			LCD_displayString((uint8*)"Previous password");
		}

	while(1){

		/*
		 * Go to main Code
		 * Execute at least once
		 * if the password is incorrect
		 * you will hault here
		*/
		do{
			Step2_mainCode();
		}while(g_theifFlag == 1);

	}

}

/********************************************************************************
 [Function Name]:	Step1_setPassword

 [Description]:		Application code of step 1 which is:
			 	 	- Set New Password
					- Set it again
			 	 	- Check if both passwords are same or not

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Step1_setPassword(void){

	/* Array to hold keypad pressed buttons */
	uint8 keypadPassword[5] = {0};

	/* Array to check if both passwords are correct or not */
	uint8 keypadCheckPassword[5] = {0};

	/* Counter Variable */
	uint8 i = 0;

	/* Counter flag for password checking */
	uint8 flag = 0;

	/* Clear Screen */
	LCD_clearScreen();

	LCD_goToRowColumn(0,0);

	/*
	 * There are no previos password saved
	 * So, we need to save a new one
	 */
	LCD_displayString((uint8*)"Enter New Pass");

	/* Go to Second row on the screen */
	LCD_goToRowColumn(1,0);

	/* Function to get the password twice to check it */
	flag = Enter_password(keypadPassword, keypadCheckPassword);

	/* Check if both passwords are correct or not */
	if(flag == 5){
		/* Both Passwords are correct */

		/* Clear calling flag */
		g_step1FinishFlag = 0;

		/* Clear the screen */
		LCD_clearScreen();

		LCD_displayString((uint8*)"Correct!!");

		_delay_ms(1000);

		/* Send Ack to UART to start receiving the new password */
		UART_sendByte(NEW_PASSWORD_TO_BE_SAVED);

		/* Send Password to Control ECU by UART */
		for(i = 0; i < 5; i++){

			/* Send Password Byte by byte to Control ECU by UART*/
			UART_sendByte(keypadPassword[i]);
		}

		/* Waiting for Control ECU to finish computation on passwords */
		while(UART_recieveByte() != M1_READY );

		/* Check if password saved successfully on eeprom or not */
		if(UART_recieveByte() == NEW_PASSWORD_SAVED_SUCCESSFULLY){

			/* Password Saved Successfully */
			LCD_clearScreen();

			LCD_displayString((uint8*)"Password Saved!");

			_delay_ms(1000);
		}

		else{
			/* Password Not Saved! */
			LCD_clearScreen();

			LCD_displayString((uint8*)"Password NOT");

		}
	}

	/* Both Passwords not matched */
	else{

		/*
		 * Set a calling flag to get back to this function
		 * if both passwords are not matched
	     */
		g_step1FinishFlag = 1;
	}

}

/********************************************************************************
 [Function Name]:	Step2_mainCode

 [Description]:		Application code of step 2 which is:
 	 	 	 	 	- Showing '+' on LCD to change password
 	 	 	 	 	- Showing '-' on LCD to Open Door which is step 3

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Step2_mainCode(void){

uint8 keyPressed = 0;

/* Array to hold keypad pressed buttons */
uint8 keypadPassword[5] = {0};

/* Array to check if both passwords are correct or not */
uint8 keypadCheckPassword[5] = {0};

/* Counter flag for password checking */
uint8 flag = 0;

/* Clear Screen */
LCD_clearScreen();

/* '+' Sign to change Password */
LCD_displayString((uint8*)"+: Change Pass");

/* '-' Sign to Open Door (Step 3) */
LCD_displayStringRowColumn(1,0, "-: Open Door");

/* Saving Input from User */
keyPressed = KeyPad_getPressedKey();

/* Delay to avoid inputting 2 times */
_delay_ms(300);

/* Check for '+' Sign which is change password */
if(keyPressed == '+'){

	UART_sendByte(M1_READY);

	UART_sendByte(OVER_WRITE_OLD_PASSWORD);

	LCD_clearScreen();

	LCD_displayString((uint8*)"Change Password");

	_delay_ms(1000);

	/* Go to check password function */
	 Password_check();

	 /*
	  * Check for theif flag
	  * (if Password is wrong 3 times consecutively)
	 */
	 if(g_theifFlag == 0){

		LCD_clearScreen();

		LCD_displayString((uint8*)"Correct Pass");

		_delay_ms(500);

		/* Proceed to get the new password */
		LCD_clearScreen();

		LCD_displayString((uint8*)"Enter New Pass");

		/* Go to Second row on the screen */
		LCD_goToRowColumn(1,0);

		/* Function to get the password twice to check it */
		flag = Enter_password(keypadPassword, keypadCheckPassword);

		/* Check if both passwords are correct or not */
		if(flag == 5){
			New_passwordCorrectCode(keypadPassword);

			/*
			 * Setting Step2FinishFlag to 1
			 * This Flag to indicate that step 2 finished
			 * if door manipulation executed after
			 * change password function, i should send
			 * M1_READY ACK 3 times due to system latency
			 * so this flag is indication for me that i
			 * finished Step 2 Function code
			*/
			g_step2FinishFlag = 1;

		}
		/* Both Passwords not matched */
		else{

			/*
			 * Not the best practice because if user put the password
			 * wrong a lot of time, all this context switching
			 * may goes into stackoverflow.
			 * So, i should think about another way
			 */
			Step2_mainCode();
		}
	 }
	 else{
		 return;
	 }

}
/* Check for '+' Sign which is Open Door (Step 3) */
else if(keyPressed == '-'){
	Step3_doorManipulation();
}
}
/********************************************************************************
 [Function Name]:	Step3_doorManipulation

 [Description]:		Application code of step 2 which is:
					- Opening and closing the door

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Step3_doorManipulation(void){

	LCD_clearScreen();

	LCD_displayString((uint8*)"Door Manipul");

	_delay_ms(1000);

	/* ACK for Control ECU */
	UART_sendByte(M1_READY);

	/* Second time sending due to system latency */
	UART_sendByte(M1_READY);

	/*  */
	if(g_step2FinishFlag == 1){

		/*
		 * Clear Step2FinishFlag
		 * This Flag to indicate that step 2 finished
		 * if door manipulation executed after
		 * change password function, i should send
		 * M1_READY ACK 3 times due to system latency
		 * so this flag is indication for me that i
		 * finished Step 2 Function code and now i should
		 * send the ACK for the 3rd time
		*/
		g_step2FinishFlag = 0;

		/* Third time sending due to system latency */
		UART_sendByte(M1_READY);

	}
	/* Telling them that we are in step 3 */
	UART_sendByte(STEP3_DOOR_MANIPULATION);

	/* Check for passowrd */
	Password_check();

	 /*
	  * Check for theif flag
	  * (if Password is wrong 3 times consecutively)
	 */
	if(g_theifFlag == 0){

	LCD_displayStringRowColumn(1,0,"Correct Pass");

	_delay_ms(500);

	LCD_clearScreen();

	LCD_displayStringRowColumn(1,0,"Door Opening");

	/* Correct Password Received */

	/* Open Door for 10 seconds */
	Door_on();

	/* Close Door for 10 Seconds */
	Door_off();
	}

	/*
	 * Password is wrong 3 times consecutively
	 * Here i will go back to main to execute
	 * the code from first (Change Password or open door)
	*/
	else{
		return;

	}

}

/********************************************************************************
 [Function Name]:	Enter_password

 [Description]:		Used as a helper function to Enter the password
 	 	 	 	 	twice and check if both time are the same or not

 [Args]:			arr1, arr2

 [in]				arr1: First Password to be filled
 	 	 	 	 	arr2: Second time password filled to check both of them

 [out]				None

 [in/out]			None

 [Returns]:			Counter of correct times
 	 	 	 	 	(It should be 5 because it's a 5 digit password)
 **********************************************************************************/
uint8 Enter_password(uint8* arr1, uint8* arr2){

	uint8 i = 0;

	uint8 FlagtoCheck = 0;
	/*
	 * We need to enter a new password from 5 digits
	 * so, we will receive the 5 digits from keypad,
	 * then it will be sent to the Control ECU UsingF UART
	 */
	for(i = 0; i < 5; i++){

		/* Receive password digit by digit */
		arr1[i] = KeyPad_getPressedKey();

		/* Press time */
		_delay_ms(300);

		/* Indicator for reciving the digit */
		LCD_displayCharacter('*');
	}

	/* Clear Screen to send password again */
	LCD_clearScreen();

	/* Telling the user to enter the password again */
	LCD_displayString((uint8*)"Enter pass again: ");

	/* Go to Second row on the screen */
	LCD_goToRowColumn(1,0);

	/*
	 * Getting the  password again to check if both passwords
	 * are correct or not
	 */
	for(i = 0; i < 5; i++){

		/* Receive password digit by digit */
		arr2[i] = KeyPad_getPressedKey();

		/* Press time */
		_delay_ms(300);

		/* Indicator for reciving the digit */
		LCD_displayCharacter('*');

		/* Check if both passwords are correct or not digit by digit */
		if(arr1[i] == arr2[i]){

			/* Increamt the flag, it should be 5 if both are correct */
			FlagtoCheck += 1;
		}
	}
	return FlagtoCheck;
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

	/* Variable used to save global counter flag in ISR */
	uint8_t save_globalNow = 0;

	LCD_clearScreen();

	LCD_displayString((uint8*)"Door Opening");

	/* Getting the value of the Counter flag now */
	save_globalNow = g_timer1_10SecFlag;

	/* Send an ACK to the Control ECU to start opening the door */
	UART_sendByte(DOOR_OPEN);

	/*
	 * delay for 10 Seconds to open the door
	 * Here i check for the global counter flag value
	 * if less than the first value of it before a
	 * 1 second from this time + 10 seconds
	*/
	while(g_timer1_10SecFlag < (save_globalNow + 10)){

		LCD_goToRowColumn(1,0);

		/* Showing the counter value on the LCD */
		LCD_intgerToString((g_timer1_1MinFlag - save_globalNow));
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

	LCD_clearScreen();

	LCD_displayString((uint8*)"Door Closing");


	/* Getting the value of the Counter flag now */
	save_globalNow = g_timer1_10SecFlag;

	/* Send an ACK to the Control ECU to start Closing the door */
	UART_sendByte(DOOR_CLOSE);

	/*
	 * delay for 10 Seconds to close the door
	 * Here i check for the global counter flag value
	 * if less than the first value of it before a
	 * 1 second from this time + 10 seconds
	*/
	while(g_timer1_10SecFlag < (save_globalNow + 10)){

		LCD_goToRowColumn(1,0);

		/* Showing the counter value on the LCD */
		LCD_intgerToString((g_timer1_1MinFlag - save_globalNow));
	}
}


/********************************************************************************
 [Function Name]:	Password_check

 [Description]:		Used as a helper function to check if password is
 	 	 	 	 	correct or not

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void Password_check(void){

	/* Variable to save the status of the operations */
	uint8 status = 0;

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

	/* Enter old password to check */
	LCD_clearScreen();

	LCD_displayString((uint8*)"Enter Password:");

	LCD_goToRowColumn(1,0);

	/* Asking to enter password */
	for(i = 0; i < 5; i++){

		/* Receive password digit by digit */
		PasswordArr[i] = KeyPad_getPressedKey();

		/* Press time */
		_delay_ms(300);

		/* Indicator for reciving the digit */
		LCD_displayCharacter('*');
	}

	/* Send Ack to UART to start receiving the password to check it*/
	UART_sendByte(CHECK_PASSWORD);

	/* Send Password to Control ECU by UART */
	for(i = 0; i < 5; i++){

		/* Send Password Byte by byte to Control ECU by UART*/
		UART_sendByte(PasswordArr[i]);
	}

	/* Wait till Control ECU finishes calculations */
	while(UART_recieveByte() != M1_READY);

	/* Expected to be CORRECT_PASSWORD or INCORRECT_PASSWORD */
	status = UART_recieveByte();

	/* Password checked by Control ECU and it is correct */
	if(status == CORRECT_PASSWORD){

		/*
		 * Clear g_theifFlag which is responsible of checking
		 * if password is 3 times consecutively wrong
	     */
		g_theifFlag = 0;

		/*
		 * Correct Password
		 * Proceed to door manipulation
		 * or change password
		 */

		_delay_ms(500);

	}

	/* Password checked by Control ECU and it is incorrect */
	else if(status == INCORRECT_PASSWORD){
		/*
		 * Incorrect Password!!
		 * You Still Have 2 times
		 * of putting the password correctly
		 */

		/* Increament Calling counter */
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
			Password_check();
		}

		/*
		 * You entered the password wrong 3 times consecutively
		 * Alarm (Buzzer) will turn on for 1 minute
		 */
		else{

			/* Clearing the recalling flag */
			Flag_of_Calling = 0;


			/* Waiting till Control ECU ACK Which means that it finished its computation */
			while(UART_recieveByte() != M1_READY);

			/*
			 * Get a response from Control ECU to check if it knows that
			 * there is a theif on the system or not
			*/
			if(UART_recieveByte() == (PASSWORD_WRONG_3_TIMES)){

			/* Set the theif flag to 1*/
			g_theifFlag = 1;

			/* Theif Function */
			Theif_onTheSystem();


			}
		}
	}

}


/********************************************************************************
 [Function Name]:	Theif_onTheSystem

 [Description]:		Used as a helper function to do the proper proceudre if there
 	 	 	 	 	is a theif in the system (Password is wrong 3 times consecutively)
 	 	 	 	 	This fuction will display a theif word on the lcd and a counter
 	 	 	 	 	of 1 minute and also a buzzer will turn on through the control ECU

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
	LCD_clearScreen();

	LCD_displayString((uint8*)"Theif!!");

	/* Getting the value of the Counter flag now */
	save_globalNow = g_timer1_1MinFlag;

	/*
	 * delay for 10 Seconds to close the door
	 * Here i check for the global counter flag value
	 * if less than the first value of it before a
	 * 1 second from this time + 10 seconds
	*/
	while(g_timer1_1MinFlag < (save_globalNow + 60)){

		LCD_goToRowColumn(1,0);

		/* Showing the counter value on the LCD */
		LCD_intgerToString((g_timer1_1MinFlag - save_globalNow));
	}

}

/********************************************************************************
 [Function Name]:	New_passwordCorrectCode

 [Description]:		Used as a helper function to check if new password is saved
 	 	 	 	 	correctly or not

 [Args]:			arr

 [in]				arr: Pointer to char which holds the address of the array
 	 	 	 	 	which holds the password

 [out]				None

 [in/out]			None

 [Returns]:			None
 **********************************************************************************/
void New_passwordCorrectCode(uint8* arr){

	uint8 i = 0;
	/* Clear the screen */
	LCD_clearScreen();

	LCD_displayString((uint8*)"Correct!!");

	_delay_ms(1000);

	/* Send Ack to UART to start receiving the new password */
	UART_sendByte(NEW_PASSWORD_TO_BE_SAVED);

	/* Send Password to Control ECU by UART */
	for(i = 0; i < 5; i++){

		/* Send Password Byte by byte to Control ECU by UART*/
		UART_sendByte(arr[i]);
	}

	/* Waiting for Control ECU to finish computation on passwords */
	while(UART_recieveByte() != M1_READY );

	/* Check if password saved successfully on eeprom or not */
	if(UART_recieveByte() == NEW_PASSWORD_SAVED_SUCCESSFULLY){

		/* Password Saved Successfully */
		LCD_clearScreen();

		LCD_displayString((uint8*)"Password Saved!");
		_delay_ms(500);

		LCD_displayStringRowColumn(1,0, "hELLO");
		return;
	}

	else{
		/* Password Not Saved! */
		LCD_clearScreen();

		LCD_displayString((uint8*)"Password NOT");

	}
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
