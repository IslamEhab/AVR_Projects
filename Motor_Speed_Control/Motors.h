 /******************************************************************************
 *
 * Module: Motors
 *
 * File Name: Motors.h
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



#ifndef MOTORS_H_
#define MOTORS_H_


#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

#define MOTOR_DDR	DDRB
#define MOTOR_PORT	PORTB

#define IN1			PB0
#define IN2			PB1

/*
 * Description: Function to initiate Motor
 * @Param:		None
 * @Return:		None
 */
void Motor_init(void);

/*
 * Description: Function to Rotate Motor Clock Wise
 * @Param:		None
 * @Return:		None
 */
void Motor_clockWise(void);

/*
 * Description: Function to Rotate Motor Anti Clock Wise
 * @Param:		None
 * @Return:		None
 */
void Motor_antiClockWise(void);

#endif /* MOTORS_H_ */
