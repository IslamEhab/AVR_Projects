 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.c
 *
 * Description: Source file for the LCD Driver
 *
 * Author: Islam Ehab
 *
 * Date: 22/9/2020
 *
 * Version:	1.0.0
 *
 * Note: Still Needs some updates
 *
 *******************************************************************************/


#include "LCD.h"


void LCD_Init(void){

	LCD_DDR_COMMAND_REG |= (1 << RS) | (1 << RW) | (1 << E);

	LCD_DDR_DATA_REG = 0xFF;

	/* 2 Lines, 8 Bit Mode */
	LCD_sendCommand(0x38);

	/* Cursor Off */
	LCD_sendCommand(0x0C);

	/* Clear Screen */
	LCD_sendCommand(0x01);
}


void LCD_sendCommand(uint8 command){

	/* RS = 0 */
	CLEAR_BIT(LCD_PORT_COMMAND_REG, RS);

	/* R/W = 0 */
	CLEAR_BIT(LCD_PORT_COMMAND_REG, RW);

	/* delay 50 ns */
	_delay_ms(1);

	/* ENABLE = 1 */
	SET_BIT(LCD_PORT_COMMAND_REG, E);

	/* delay 190 ns */
	_delay_ms(1);

	/* DATA BUS = Command  */
	LCD_PORT_DATA_REG = command;

	/* delay 100 ns */
	_delay_ms(1);

	/* Enable = 0 */
	CLEAR_BIT(LCD_PORT_COMMAND_REG, E);

	_delay_ms(1);
}

void LCD_displayCharacter(uint8 chr){

	/* RS = 1 */
	SET_BIT(LCD_PORT_COMMAND_REG, RS);

	/* R/W = 0 */
	CLEAR_BIT(LCD_PORT_COMMAND_REG, RW);

	/* delay 50 ns */
	_delay_ms(1);

	/* ENABLE = 1 */
	SET_BIT(LCD_PORT_COMMAND_REG, E);

	/* delay 190 ns */
	_delay_ms(1);

	/* DATA BUS = Command  */
	LCD_PORT_DATA_REG = chr;

	/* delay 100 ns */
	_delay_ms(1);

	/* Enable = 0 */
	CLEAR_BIT(LCD_PORT_COMMAND_REG, E);

	_delay_ms(1);
}

void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
	/***************** Another Method ***********************
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
	}
	*********************************************************/
}

void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;

	/* first of all calculate the required address */
	switch(row)
	{
		case 0:
				Address=col;
				break;
		case 1:
				Address=col+0x40;
				break;
		case 2:
				Address=col+0x10;
				break;
		case 3:
				Address=col+0x50;
				break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | SET_CURSOR_LOCATION);
}

void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}

void LCD_intgerToString(int data)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* 10 for decimal */
   LCD_displayString(buff);
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND); //clear display
}

