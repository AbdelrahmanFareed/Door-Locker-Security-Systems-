/*
 * main.c
 *
 *  Created on: Oct 31, 2022
 *      Author: Eng AFS
 */

#include "LCD.h"
#include "Uart.h"
#include "Timer1.h"
#include "Keypad.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define password_size              5
#define Micro_is_ready             0x80
#define Setpassword                0x05
#define CHeckPassword              0x10
#define matched                    0x81
#define unmatched                  0x82
#define ALLOWED_TRIES              3
#define OPEN_DOOR                  0x90
#define TurnOnBuzzer               0x50
#define PASSWORD_SET               0x40

uint8 password[password_size];
uint8 password2[password_size];
uint8 passwordMatch=0;
uint8 tries=3;
uint8 ticks = 0;
uint8 MotorFlag=0;
uint8 AlarmFlag=0;
uint8 StartMotor=0;


Timer1_ConfigType TIMER1_Configuration = {0,8000,TIMER1_FCPU_1024,COMPARE};
UART_ConfigType UART ={Asynchronous,Disabled,oneBit,EightBit,BaudRate_9600};

void SetPassword(void);
uint8 ComparePassword(void);
void Alarm(void);
void OpenDoor(void);
void ChangePassword();
void Alarm_Timer(void);
void WaitTimer(void);
void OpenDoorTimer(void);
uint8 MainOption(void);



int main(void)
{
	LCD_init();
	UART_init(&UART);
	SREG |=(1<<7);
	UART_sendByte(Micro_is_ready);
	uint8 PasswordState = UART_recieveByte();
	if(PasswordState != PASSWORD_SET)
	{
		SetPassword();
	}
		while(1)
		{
			uint8 MainOptionKey = MainOption();
			if(MainOptionKey == '+')
			{
				OpenDoor();
			}
			else if(MainOptionKey == '+')
			{
				ChangePassword();
			}

		}


}

void SetPassword(void)
{
	while(1)
	{
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		_delay_ms(1000);
		LCD_clearScreen();
		UART_sendByte(0x99);
		LCD_displayStringRowColumn(0,0,"Plz Enter Pass:");
		LCD_moveCursor(1,0);
		passwordMatch=1;


		for(uint8 i=0;i<password_size;i++)
		{
			uint8 key = KEYPAD_getPressedKey();
			if(key >= 0 && key <=9 )
			{
				password[i]=key;
				LCD_displayCharacter('*');
			}

			_delay_ms(250);
		}
		while(KEYPAD_getPressedKey() != 13){} /* wait tell the user press Enter key on the keypad */

		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Plz Re-Enter the");
		LCD_displayStringRowColumn(1, 0, "Same Pass: ");
		LCD_moveCursor(1,11);

		for(uint8 i=0;i<password_size;i++)
		{
			uint8 key = KEYPAD_getPressedKey();
			if(key >= 0 && key <=9 )
			{
				password2[i]=key;
				LCD_displayCharacter('*');

			}

			_delay_ms(250);
		}
		while(KEYPAD_getPressedKey() != 13){} /* wait tell the user press Enter key on the keypad */

		for(uint8 j=0;j<password_size;j++)
		{
			if(password[j]!=password2[j])
			{
				passwordMatch=0;
				break;

			}
			else
			{
				passwordMatch=1;

			}

		}
		if(passwordMatch==1)
		{
			LCD_displayStringRowColumn(0, 4, "Matched");
			LCD_displayStringRowColumn(1, 0, "Password Updated");
			for(uint8 k = 0 ; k < password_size ; k++)
			{
				UART_sendByte(password[k]);
				_delay_ms(50);
			}
			return ;
		}
		else
		{
			LCD_displayStringRowColumn(0, 0, "UnMatched");

		}
	}

}

uint8 ComparePassword(void)
{
	while(tries>0)
	{
		passwordMatch=1;
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_displayStringRowColumn(1, 0, "Password: ");
		for(uint8 i=0;i<password_size;i++)
		{
			uint8 key = KEYPAD_getPressedKey();
			if(key >= 0 && key <=9 )
			{
				password2[i]=key;
				LCD_displayCharacter('*');

			}
			else
			{
				i--;

			}
			_delay_ms(500);
		}
		while(KEYPAD_getPressedKey() != 13){} /* wait tell the user press Enter key on the keypad */
		UART_sendByte(CHeckPassword);
		for(uint8 j = 0 ; j < password_size	; j++)
		{
			UART_sendByte(password2[j]);
			while(UART_recieveByte() != Micro_is_ready);
		}
		LCD_clearScreen();
		if(UART_recieveByte() == matched)
		{
			LCD_displayStringRowColumn(0, 4, "Matched");
			_delay_ms(1000);
			return matched;
		}
		else
		{
			tries--; /* decrease number of tries by 1 if the user missed */
			LCD_displayStringRowColumn(0, 3, "UnMatched");
			LCD_displayStringRowColumn(1, 0, "Tries left = ");
			LCD_intgerToString(tries);
			_delay_ms(1000);
		}
		return unmatched;

	}

}
void Alarm(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 3, "Error");
	UART_sendByte(TurnOnBuzzer);
	ticks = 0;
	AlarmFlag = 0;
	Timer1_init(&TIMER1_Configuration);
	Timer1_comp_SetCallBack(Alarm_Timer);
	while(AlarmFlag == 0){}
}

void OpenDoor(void)
{
	uint8 compare =ComparePassword();
	if(compare==matched)
	{
		UART_sendByte(OPEN_DOOR);
		ticks = 0;
		Timer1_comp_SetCallBack(OpenDoorTimer); /* unlock the door takes 15 seconds */
		Timer1_init(&TIMER1_Configuration);
		LCD_displayStringRowColumn(0, 5, "The door");
		LCD_displayStringRowColumn(1, 0, "is unlocking");
		MotorFlag = 0;
		while(MotorFlag == 0){}

	}
	if(compare==unmatched)
	{
		Alarm();

	}

}

void ChangePassword()
{
	uint8 compare =ComparePassword();
	if(compare == matched)
	{
		SetPassword(); /* set the new password */
	}
	if(compare == unmatched)
	{
		Alarm(); /* Turn on the alarm */
	}

}

void Alarm_Timer(void)
{
	ticks++;
	if(ticks==60)
	{
		Timer1_deInit();
		ticks=0;
		AlarmFlag=1;
	}
}


void WaitTimer(void)
{
	ticks++;
	if(ticks == 3)
	{
		ticks = 0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,4, "Locking");
		LCD_displayStringRowColumn(1,4, "The Door");
		UART_sendByte(Micro_is_ready);
		Timer1_comp_SetCallBack(OpenDoorTimer);
	}
}


void OpenDoorTimer(void)
{
	ticks++;
	if(ticks == 15)
	{
		ticks = 0;
		LCD_clearScreen();
		if(StartMotor == 0)
		{
			LCD_displayStringRowColumn(0, 0, "Door Is Locked");
			UART_sendByte(Micro_is_ready);
			StartMotor++;
			Timer1_comp_SetCallBack(WaitTimer);
		}
		else if(StartMotor == 1)
		{
			StartMotor = 0;
			MotorFlag = 1;
			Timer1_deInit();
		}
	}
}

uint8 MainOption(void)
{
	uint8 key = 0;
	/* Displays the main options on LCD */
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");
	/* Keeps waiting until an available key is pressed */
	while(key != '+' || key!= '-')
	{
		/* Waits until keypad is pressed and save option in key variable*/
		key = KEYPAD_getPressedKey();
		/* Send key to Control ECU if only available option is pressed*/
		if(key == '+' || key == '-')
		{
			UART_sendByte(key);
			return key;
		}
	}
	return key;
}


