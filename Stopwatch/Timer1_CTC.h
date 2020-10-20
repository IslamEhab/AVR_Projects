/*
 * Timer1_CTC.h
 *
 *  Created on: Aug 23, 2020
 *      Author: Islam Ehab
 */

#ifndef TIMER1_CTC_H_
#define TIMER1_CTC_H_

#include "avr/io.h"

void Timer1_CTC_Init(void);
void delay(void);
void Timer1_Stop(void);
void Timer1_Resume(void);


#endif /* TIMER1_CTC_H_ */
