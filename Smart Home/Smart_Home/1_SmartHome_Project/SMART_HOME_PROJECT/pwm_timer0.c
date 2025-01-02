#include "pwm_timer0.h"
#include <avr/io.h>
void PWM_Timer0_Start(uint8 duty_cycle) {
	TCNT0 = 0;
	switch(duty_cycle)
	{
	case 0:
		OCR0 = 0;
		break;
	case 25:
		OCR0 = 64;
		break;
	case 50:
		OCR0 = 128;
		break;
	case 75:
		OCR0 = 192;
		break;
	case 100:
		OCR0 = 255;
		break;
	}
	DDRB |=(1<<PB3);
	TCCR0 |= (1 << COM01) | (1 << WGM01) | (1 << WGM00) | (1 << CS02) | (1 << CS00);
}

