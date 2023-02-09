/*
 * DC_Motor.c
 *
 *  Created on: Oct 8, 2022
 *      Author: Eng AFS
 */

#include "DC_Motor.h"
#include "gpio.h"
#include "PWM.h"

void DcMotor_Init(void)
{
	GPIO_setupPinDirection(PORTB_ID,PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID,PIN1_ID,PIN_OUTPUT);

}

void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	switch(state)
	{
	case stop:
		GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);
		break;
	case clockwise:
		GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_HIGH);
		GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);
		break;
	case anticlockwise:
		GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
		GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_HIGH);
		break;

	}
	PWM_Timer0_Init(speed);
}
