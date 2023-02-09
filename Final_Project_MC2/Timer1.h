/*
 * Timer1.h
 *
 *  Created on: Oct 30, 2022
 *      Author: Eng AFS
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"
/*******************************************************************************
*                         Types Declaration                                   *
*******************************************************************************/
/*
 * Data type to describe the timer modes
 */
typedef enum
{
	NORMAL,
	COMPARE
}Timer1_Mode;

/*
 * Data type to describe the timer prescaler
 */
typedef enum
{
	TIMER1_OFF,
	TIMER1_FCPU_1,
	TIMER1_FCPU_8,
	TIMER1_FCPU_64,
	TIMER1_FCPU_256,
	TIMER1_FCPU_1024
}Timer1_Prescaler;

/*
 * Data type to dynamic configuration
 */

typedef struct {
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only.
Timer1_Prescaler prescaler;
Timer1_Mode mode;
}Timer1_ConfigType;




/*******************************************************************************
*                      Functions Prototypes                                   *
*******************************************************************************/
/*
 * Function to initialize the Timer driver
 */

void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Function to disable the Timer1.
 */

void Timer1_deInit(void);

/*
 * Functions to set the Call Back function address.
 */

void Timer1_ovf_SetCallBack(void(*a_ptr)(void));

void Timer1_comp_SetCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
