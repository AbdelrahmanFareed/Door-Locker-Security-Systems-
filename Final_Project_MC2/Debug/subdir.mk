################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DC_Motor.c \
../Micro2.c \
../PWM.c \
../Timer1.c \
../Uart.c \
../buzzer.c \
../external_eeprom.c \
../gpio.c \
../twi.c 

OBJS += \
./DC_Motor.o \
./Micro2.o \
./PWM.o \
./Timer1.o \
./Uart.o \
./buzzer.o \
./external_eeprom.o \
./gpio.o \
./twi.o 

C_DEPS += \
./DC_Motor.d \
./Micro2.d \
./PWM.d \
./Timer1.d \
./Uart.d \
./buzzer.d \
./external_eeprom.d \
./gpio.d \
./twi.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


