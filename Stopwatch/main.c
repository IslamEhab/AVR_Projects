/*
 * main.c
 *
 *  Created on: Aug 23, 2020
 *      Author: Islam Ehab
 */

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "Timer1_CTC.h"


/******************************************* Macros *********************************************/

#define SEG_CTRL	PORTC

/************************************************************************************************/


/************************************* Functions Prototype **************************************/

void Seven_Segment_Draw(char num);
void Seven_Segment_Put_Numbers(void);
void Push_Buttons_Interrupt_Init(void);
void Seven_Segment_Init(void);

/************************************************************************************************/


/********************************* Global Variables Definition **********************************/

/* Global Variables used as INT0 , INT1, INT2 Flags */
unsigned char g_INT0 = 0;
unsigned char g_INT1 = 0;
unsigned char g_INT2 = 0;

/* Global Variables for Counting the time */
unsigned char seconds  = 0;
unsigned char hours = 0;
unsigned char minutes  = 0;
int hold = 0;

/* Global Variable used as Ticks for Timer1 */
unsigned char g_tick = 0;

/************************************************************************************************/


/***************************************** ISRs ************************************************/
/* Timer 1 CTC Channel A Interrupt */
ISR(TIMER1_COMPA_vect)
{
	g_tick = 1;
}

/* Reset Interrupt */
ISR(INT0_vect)
{
	g_INT0 = 1;
}

/* Pause Interrupt */
ISR(INT1_vect)
{
	g_INT1 = 1;
}

/* Resume Interrupt */
ISR(INT2_vect)
{
	g_INT2 = 1;
}

/************************************************************************************************/

/*************************************** Functions Body *****************************************/

/* Description: Function used to draw the number
 * 				on the 7 Segment with BCD Decoder
 * @Param:		None
 * @Return:		None
 * */
void Seven_Segment_Draw(char num){

	if((PORTA & num) > 9)
	{
		num = 0;
		PORTA = (PORTA & 0xF0) | (num & 0x0F);
	}
	else
	{
		// increment 7-segment every press
		PORTA = (PORTA & 0xF0) | (num & 0x0F);
	}
}

/* Description: Function used to Put Numbers on 7 Segment
 * 				_delay_ms() used because i need to have another timer
 * 				to tick for 7 segment delay
 * 				Update: Using Timer1 CTC Channel B did NOT work
 * 				so now _delay_ms() is used till troubleshoot this problem
 * @Param:		None
 * @ReturnL:	None
 * */
void Seven_Segment_Put_Numbers(void){
	Seven_Segment_Draw( (seconds % 10) );
	SEG_CTRL = 0x01;
	_delay_ms(5);
	SEG_CTRL = 0x00;

	Seven_Segment_Draw( (seconds / 10) );
	SEG_CTRL = 0x02;
	_delay_ms(5);
	SEG_CTRL = 0x00;

	Seven_Segment_Draw( (minutes % 10) );
	SEG_CTRL = 0x04;
	_delay_ms(5);
	SEG_CTRL = 0x00;

	Seven_Segment_Draw( (minutes / 10) );
	SEG_CTRL = 0x08;
	_delay_ms(5);
	SEG_CTRL = 0x00;

	Seven_Segment_Draw( (hours % 10) );
	SEG_CTRL = 0x10;
	_delay_ms(5);
	SEG_CTRL = 0x00;

	Seven_Segment_Draw( (hours / 10) );
	SEG_CTRL = 0x20;
	_delay_ms(5);
	SEG_CTRL = 0x00;
}

/* Description: Function to initialize DIO & Interrupt Settings for INT0, 1, and 2
 * 				INT0 -> Falling Edge, Internal Pull Up Enable, Used to RESET  the Stop Watch
 * 				INT1 -> Rising  Edge, External Pull down CT,   Used to PAUSE  the Stop Watch
 * 				INT2 -> Falling Edge, Internal Pull Up Enable, Used to RESUME the Stop Watch
 * @Param:		None
 * @Return:		None
 * */
void Push_Buttons_Interrupt_Init(void){

	/* INT 0 Settings
	 * PD2 I/P
	 * Internal Pull Up Enable
	 * Interrupt with falling edge
     */

	/* Set PD2 as I/p */
	DDRD   &= ~(1<<PD2);

	/* Enable Internal Pull up Resistor */
	PORTD |= (1<<PD2);

	/* Detect Interrupt with falling edge */
	MCUCR = (1<<ISC01);

	/* Enable INT0 */
	GICR  = (1<<INT0);

	/* INT 1 Settings
	 * PD3 I/P
	 * External Pull Down Circuit
	 * Interrupt with rising edge
	 */

	/* Set PD3 as I/P */
	DDRD &= ~(1<<PD3);

	/* Detect Interrupt with rising edge */
	MCUCR |= ( (1<<ISC11) | (1<<ISC10) );

	/* Enable INT1 */
	GICR  |= (1<<INT1);

	/* INT 2 Settings
	 * PB2 I/P
	 * Internal Pull Up Enable
	 * Interrupt with Falling edge
	 */

	/* Set PB2 as I/P */
	DDRB &= ~(1<<PB2);

	/* Enable Internal Pull Up */
	PORTB |= (1<<PB2);

	/* Detect Interrupt with falling edge */
	MCUCSR &= ~(1<<ISC2);

	/* Enable INT2 */
	GICR |= (1<<INT2);
}

/* Description: Function to initialize DIO Settings for Seven Segments Data & Control lines
 * @Param:		None
 * @Return:		None
 * */
void Seven_Segment_Init(void)
{
	//First 4 pins of Port A are O/Ps for Decoder
	DDRA = 0x0F;

	//First 6 pins of Port C are O/Ps for NPN of 7 Segment
	DDRC  |= 0x3F;

	//Initial Value = 0
	PORTA = 0x00;
	PORTC = 0x00;
}

/************************************************************************************************/

/**************************************** Main Program ******************************************/

/* Description: Main Function
 * @Param:		None
 * @Return:		None
 * */
int main(void){

	// Seven Segment Init
	Seven_Segment_Init();

	//Interrupt Pins Configuration
	Push_Buttons_Interrupt_Init();

	//Timer1 Initialization Function
	Timer1_CTC_Init();

	hold = 0;

	while(1){

		if(g_INT0 == 1)
		{
			/* Reset Sequence */
			g_INT0 = 0;

			delay();

			hours = 0;
			seconds  = 0;
			minutes  = 0;
			hold 	 = 0;
		}
		if(g_INT1 == 1)
		{
			/* Pause Sequence */
			g_INT1 = 0;

			hold = 1;


		}
		if(g_INT2 == 1)
		{
			/* Resume Sequence */
			g_INT2 = 0;

			hold = 0;
		}

		/* Wait till Global 1 Sec flag enables */
		if(g_tick == 1){
			g_tick = 0;

			if(hold == 0)
			{
				seconds++;

				if(seconds == 60){
					seconds = 0;
					minutes++;
				}

				if(minutes == 60){
					minutes = 0;
					hours++;
				}
				if(hours > 99){
					hours = 0;
				}
			}
		}

		Seven_Segment_Put_Numbers();

	}

}

/************************************************************************************************/



