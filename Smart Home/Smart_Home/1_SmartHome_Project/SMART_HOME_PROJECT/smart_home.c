#include "dc_motor.h"
#include "flame_sensor.h"
#include "lcd.h"
#include "ldr_sensor.h"
#include "led.h"
#include "lm35_sensor.h"
#include "buzzer.h"

void checkFlame();
static uint8 g_flag = 0;

int main(void) {
	LCD_init();
	LEDS_init();
	BUZZER_init();
	DcMotor_init();
	FlameSensor_init();

	LCD_displayStringRowColumn(0,4,"FAN is ");
	LCD_displayStringRowColumn(1,0,"Temp=");
	LCD_displayStringRowColumn(1,8, "LDR=   %");
	uint8 temperature;
	uint8 intensity;
	while (1) {
		if(!(g_flag))
		{
			LCD_moveCursor(1, 5);
			temperature = LM35_getTemperature();
			if (temperature >= 100)
			{
				LCD_intgerToString(temperature);
			}
			else
			{
				LCD_intgerToString(temperature);
				/* In case the digital value is two or one digits print space in the next digit place */
				LCD_displayCharacter(' ');
			}
			if(temperature<25)
			{
				LCD_displayStringRowColumn(0,11,"OFF");
				Dc_Motor_Rotate(OFF,0);
			}
			else if(temperature>=25 && temperature<30)
			{
				LCD_displayStringRowColumn(0,11,"ON");
				LCD_displayCharacter(' ');
				Dc_Motor_Rotate(CW,25);

			}
			else if(temperature>=30 && temperature<35)
			{
				LCD_displayStringRowColumn(0,11,"ON");
				LCD_displayCharacter(' ');
				Dc_Motor_Rotate(CW,50);

			}
			else if(temperature>=35 && temperature<40)
			{
				LCD_displayStringRowColumn(0,11,"ON");
				LCD_displayCharacter(' ');
				Dc_Motor_Rotate(CW,75);
			}
			else if(temperature>=40)
			{
				LCD_displayStringRowColumn(0,11,"ON");
				LCD_displayCharacter(' ');
				Dc_Motor_Rotate(CW,100);
			}

			LCD_moveCursor(1,12);
			intensity = LDR_getLightIntensity();
			if (intensity >= 100)
			{
				LCD_intgerToString(intensity);
			}
			else
			{
				LCD_intgerToString(intensity);
				/* In case the digital value is two or one digits print space in the next digit place */
				LCD_displayCharacter(' ');
			}
			if(intensity<15)
			{
				LED_on(RED_LED);
				LED_on(GREEN_LED);
				LED_on(BLUE_LED);
			}
			else if(intensity<=50)
			{
				LED_on(RED_LED);
				LED_on(GREEN_LED);
				LED_off(BLUE_LED);
			}
			else if(intensity<=70)
			{
				LED_on(RED_LED);
				LED_off(GREEN_LED);
				LED_off(BLUE_LED);
			}
			else
			{
				LED_off(RED_LED);
				LED_off(GREEN_LED);
				LED_off(BLUE_LED);
			}
		}
		checkFlame();
	}
}
void checkFlame()
{
	static uint8 critical_alert_flag = 0;
	static uint8 normal_mode_flag=0;
	if(FlameSensor_getValue())
	{
		if(!critical_alert_flag)
		{
			LCD_clearScreen();
			g_flag =1;
			LCD_displayStringRowColumn(0,0,"Critical Alert!");
			BUZZER_on();
			critical_alert_flag=1;
			normal_mode_flag=0;
		}
	}
	else
	{
		critical_alert_flag =0;
		if(!normal_mode_flag)
		{
			normal_mode_flag=1;
			g_flag = 0;

			LCD_clearScreen();
			LCD_displayStringRowColumn(0,4,"FAN is ");
			LCD_displayStringRowColumn(1,0,"Temp=");
			LCD_displayStringRowColumn(1,8, "LDR=   %");
			BUZZER_off();
		}
	}
}
