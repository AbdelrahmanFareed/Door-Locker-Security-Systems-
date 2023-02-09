/*
 * DC_Motor.h
 *
 *  Created on: Oct 8, 2022
 *      Author: Eng AFS
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include "std_types.h"
 typedef enum
{
	 stop,
	 clockwise,
	 anticlockwise
}DcMotor_State;

/* The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver */
void DcMotor_Init(void);


/*The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
 Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
