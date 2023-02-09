/*
 * Micro2.c
 *
 *  Created on: Nov 6, 2022
 *      Author: Eng AFS
 */

#include "Uart.h"
#include "twi.h"
#include "Timer1.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "DC_Motor.h"
#include "avr/interrupt.h"

#define	Password_Is_Set_Address		0x30
#define passwordsize                5
#define Password_Address			0x31
#define Micro_is_ready              0x80
#define Setpassword                 0x05
#define CHeckPassword               0x10
#define matched                     0x81
#define unmatched                   0x82
#define PasswordSET                 0x40
#define OPEN_DOOR                   0x90
#define TurnOnBuzzer                0x50


uint8 PasswordState=0;
uint8 Password[passwordsize];
uint8 ticks=0;
uint8 MotorFlag=0;
uint8 AlarmFlag=0;
uint8 StartMotor=0;


void isPasswordSet(void);
void ReadPassword(void);
void UpdatePassword(void);
void CheckPassword(void);
void Alarm(void);
void OpenDoor(void);
void Alarm_Timer(void);
void WaitTimer(void);
void OpenDoorTimer(void);


Timer1_ConfigType TIMER1_Configuration = {0,8000,TIMER1_FCPU_1024,COMPARE};
UART_ConfigType UART ={Asynchronous,Disabled,oneBit,EightBit,BaudRate_9600};
TWI_ConfigType TWI_Configuration = {1,400}; /* Slave Address = 1 , Baud rate = 400 Kbps */

int main(void)
{
	TWI_init(&TWI_Configuration);
	Buzzer_init();
	DcMotor_Init();
	UART_init(&UART);
	SREG |=(1<<7);
	while(UART_recieveByte() != Micro_is_ready){}
	 isPasswordSet();

	while(1)
	{
		/* waiting MCU1 to send the message */
				uint8 MSG = UART_recieveByte();
				switch(MSG)
				{
				/* In case MCU1 wants to set the password or update it */
				case  Setpassword:
					UpdatePassword();
					break;
				/* In case MCU1 wants to enter ERROR state and turn the alarm on */
				case TurnOnBuzzer:
					Alarm(); /* call the function that is responsible of turning alarm on for 60 seconds */
					break;
				/* In case MCU1 wants to enter open the door */
				case OPEN_DOOR:
					OpenDoor();/* call the function that is responsible of opening the door */
					break;
				/* In case MCU1 wants to know the password saved in EEPROM */
				case CHeckPassword:
					CheckPassword();
					break;
				}
	}
}

void isPasswordSet(void)
{
	/* To check if password is set in the EEPROM or not */
	EEPROM_readByte(Password_Is_Set_Address, &PasswordState);
	/* send the password state to MCU1 to handle the different cases */
	UART_sendByte(PasswordState);
}

void ReadPassword(void)
{
	for(uint8 i =0;i<passwordsize;i++)
	{
		EEPROM_readByte(Password_Address+i,(Password+i));
	}
}

void UpdatePassword(void)
{
	for(uint8 i = 0 ; i < passwordsize ; i++)
	{
		Password[i] = UART_recieveByte();
		EEPROM_writeByte(Password_Address+i, Password[i]);
	}
	EEPROM_writeByte(Password_Is_Set_Address, PasswordSET);
}



void CheckPassword(void)
{
	uint8 checkPassword[passwordsize];
	for(uint8 i = 0 ; i < passwordsize ; i++)
	{
		checkPassword[i] = UART_recieveByte();
		UART_sendByte(Micro_is_ready);
	}
	for(uint8 j = 0 ; j < passwordsize ; j++)
	{
		if(Password[j] != checkPassword[j])
		{
			UART_sendByte(matched);
			return;
		}
	}
	UART_sendByte(unmatched);
}
void Alarm(void)
{
	ticks = 0;
	AlarmFlag = 0;
	Timer1_comp_SetCallBack(Alarm_Timer);
	Timer1_init(&TIMER1_Configuration);
	buzzerOn();
	while(AlarmFlag == 0){}
	buzzerOff();
}

void OpenDoor(void)
{

	ticks = 0;
	Timer1_comp_SetCallBack(OpenDoorTimer); /* unlock the door takes 15 seconds */
	Timer1_init(&TIMER1_Configuration);
	DcMotor_Rotate(clockwise,120);
	MotorFlag = 0;
	while(MotorFlag == 0){}


}

void Alarm_Timer(void)
{
	ticks++;
	if(ticks==60)
	{
		ticks=0;
		AlarmFlag=1;
		Timer1_deInit();

	}
}


void WaitTimer(void)
{
	ticks++;
	if(ticks == 3)
	{
		ticks = 0;
		UART_recieveByte();
		DcMotor_Rotate(anticlockwise,120);
		Timer1_comp_SetCallBack(OpenDoorTimer);
	}
}


void OpenDoorTimer(void)
{
	ticks++;
	if(ticks == 15)
	{
		ticks = 0;
		if(StartMotor == 0)
		{
			UART_recieveByte();
			DcMotor_Rotate(stop,120);
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
