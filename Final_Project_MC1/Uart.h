/*
 * Uart.h
 *
 *  Created on: Oct 11, 2022
 *      Author: Eng AFS
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

typedef enum{
	Asynchronous,
	Synchronous,

}Mode_Select;

typedef enum{
	Disabled,
	EnabledEvenParity=2,
	EnabledOddParity,
}Parity_Mode;

typedef enum{
	oneBit,
	twoBit,

}Stop_Bit;

typedef enum{
	FiveBit,
	SixBit,
	SevenBit,
	EightBit,
	NineBit=7,

}Data_Size;

typedef enum{
	BaudRate_2400=2400,
	BaudRate_4800=4800,
	BaudRate_9600=9600,
	BaudRate_14400=14400,
	BaudRate_19200=19200,
	BaudRate_28800=28800,

}Baud_Rate;
typedef struct
{
	Mode_Select ModeSelect ;
	Parity_Mode ParityMode ;
	Stop_Bit StopBit       ;
	Data_Size DataSize     ;
	Baud_Rate BaudRate     ;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
