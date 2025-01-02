#include <avr/io.h>

#include "pwm_timer0.h"
void PWM_Timer0_Start(uint8 duty_cycle) {
	TCNT0 = 0;
	switch(duty_cycle)
	{
	case _0_SPEED:
		OCR0 = 0;
		break;
	case _25_SPEED:
		OCR0 = 64;
		break;
	case _50_SPEED:
		OCR0 = 128;
		break;
	case _75_SPEED:
		OCR0 = 192;
		break;
	case FULL_SPEED:
		OCR0 = 255;
		break;
	}
	DDRB |=(1<<PB3);
	TCCR0 |= (1 << COM01) | (1 << WGM01) | (1 << WGM00) | (1 << CS02) | (1 << CS00);
}

