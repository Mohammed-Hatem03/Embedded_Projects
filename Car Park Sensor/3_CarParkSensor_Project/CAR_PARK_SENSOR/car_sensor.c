#include "lcd.h"
#include "buzzer.h"
#include "led.h"
#include "ultrasonic.h"
#include <util/delay.h>
#include <avr/io.h>


int main(void)
{
	SREG |=(1<<7);
	LCD_init();
	LEDS_init();
	BUZZER_init();
	Ultrasonic_init();

	uint16 distance;
	LCD_displayString("Distance =    cm");
	while(1)
	{
		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0,11);
		if (distance >= 10)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}
		distance = Ultrasonic_readDistance();
		if (distance > 20)
		{
			LCD_displayStringRowColumn(1,0,"      ");
			LED_off(RED_LED);
			LED_off(GREEN_LED);
			LED_off(BLUE_LED);
			BUZZER_off();
		}
		else if (distance >= 16 && distance <= 20){
			LCD_displayStringRowColumn(1,0,"      ");
			LED_on(RED_LED);
			LED_off(GREEN_LED);
			LED_off(BLUE_LED);
			BUZZER_off();
		}
		else if(distance >= 11 && distance <= 15)
		{
			LCD_displayStringRowColumn(1,0,"      ");
			LED_on(RED_LED);
			LED_on(GREEN_LED);
			LED_off(BLUE_LED);
			BUZZER_off();
		}
		else if(distance >= 6 && distance <= 10)
		{
			LCD_displayStringRowColumn(1,0,"     ");
			LED_on(RED_LED);
			LED_on(GREEN_LED);
			LED_on(BLUE_LED);
			BUZZER_off();
		}
		else if(distance <= 5)
		{
			LCD_displayStringRowColumn(1,0,"STOP!");
			BUZZER_on();
			LED_on(RED_LED);
			LED_on(GREEN_LED);
			LED_on(BLUE_LED);
			_delay_ms(500);
			LED_off(RED_LED);
			LED_off(GREEN_LED);
			LED_off(BLUE_LED);
			_delay_ms(500);
		}

	}

}
