 /******************************************************************************
 *
 * Module: Motors
 *
 * File Name: Motors.c
 *
 * Description: Source file for the Motors Driver
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


#include "Motors.h"

/*
 * Description: Function to initiate Motor
 * @Param:		None
 * @Return:		None
 */
void Motor_init(void){

	/* H-Bridge Enable */
	MOTOR_DDR |= ( (1 << IN1) | (1 << IN2) );

	/* Motors Initially in Stop mode */
	MOTOR_PORT &= ~( ( (1 << IN1) | (1 << IN2) ) );

}

/*
 * Description: Function to Rotate Motor Clock Wise
 * @Param:		None
 * @Return:		None
 */
void Motor_clockWise(void){

	MOTOR_PORT &= (~(1 << IN1));
	MOTOR_PORT |= (1 << IN2);
}

/*
 * Description: Function to Rotate Motor Anti Clock Wise
 * @Param:		None
 * @Return:		None
 */
void Motor_antiClockWise(void){

	MOTOR_PORT &= (~(1 << IN2));
	MOTOR_PORT |= (1 << IN1);
}
