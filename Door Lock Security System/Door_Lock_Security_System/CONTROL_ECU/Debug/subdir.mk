################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PIR.c \
../buzzer.c \
../control_ecu.c \
../dc_motor.c \
../external_eeprom.c \
../gpio.c \
../pwm_timer0.c \
../timer.c \
../twi.c \
../uart.c 

OBJS += \
./PIR.o \
./buzzer.o \
./control_ecu.o \
./dc_motor.o \
./external_eeprom.o \
./gpio.o \
./pwm_timer0.o \
./timer.o \
./twi.o \
./uart.o 

C_DEPS += \
./PIR.d \
./buzzer.d \
./control_ecu.d \
./dc_motor.d \
./external_eeprom.d \
./gpio.d \
./pwm_timer0.d \
./timer.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


