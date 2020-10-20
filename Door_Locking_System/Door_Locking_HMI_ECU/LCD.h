 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.h
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


#ifndef LCD_H_
#define LCD_H_

#include "stdint.h"
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


#define LCD_PORT_COMMAND_REG	PORTD
#define LCD_DDR_COMMAND_REG		DDRD

#define LCD_DDR_DATA_REG		DDRC
#define LCD_PORT_DATA_REG		PORTC


/* LCD HW Pins */
#define RS PD2
#define RW PD3
#define E  PD4
#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_PORT_DIR DDRD

#define LCD_DATA_PORT PORTC
#define LCD_DATA_PORT_DIR DDRC

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80


void LCD_Init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 chr);
void LCD_displayString(const uint8 *Str);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_intgerToString(int data);
#endif /* LCD_H_ */
