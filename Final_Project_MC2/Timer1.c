/*
 * Timer1.c
 *
 *  Created on: Oct 30, 2022
 *      Author: Eng AFS
 */

#include "Timer1.h"
#include "gpio.h"
#include <avr/io.h>
#include "avr/interrupt.h"


static volatile void (*g_callBackPtr1)(void) = NULL_PTR; /* to store the address of the function */
static volatile void (*g_callBackPtr2)(void) = NULL_PTR; /* to store the address of the function */

/*
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);//Force Output Compare for Channel A and B
	TCCR1B = (Config_Ptr->mode << WGM12);
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);

	TCNT1 = Config_Ptr->initial_value;
	if(Config_Ptr->mode == COMPARE)
	{
		OCR1A = Config_Ptr->compare_value; //set compare value
	}
	TIMSK |= (1<<OCIE1A) ;//Output Compare A Match Interrupt Enable
}


/*
 * Function to disable the Timer1.
 */
void Timer1_deInit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
}
void Timer1_ovf_SetCallBack(void(*a_ptr)(void))
{
	g_callBackPtr1 = a_ptr;

}

void Timer1_comp_SetCallBack(void(*a_ptr)(void))
{
	g_callBackPtr2 = a_ptr;

}


ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		(*g_callBackPtr1)();
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		(*g_callBackPtr2)();
	}
}
