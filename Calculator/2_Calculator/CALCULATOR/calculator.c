#include "keypad.h"
#include "lcd.h"
#include <util/delay.h>

float32 num1 = 0;
float32 num2 = 0;
float32 result = 0;
uint8 operator = 0;
uint8 key;
uint8 g_math_error = 0;

//void CALCULATOR_operation(float32 num1, float32 num2,uint8 operator)
//{
//	switch(operator)
//	{
//	case ADDITION:
//		g_math_error = 0;
//		result = num1 + num2;
//		break;
//	case SUBTRACTION:
//		g_math_error = 0;
//		result = num1-num2;
//		break;
//	case MULTIPLICATION:
//		g_math_error = 0;
//		result = num1*num2;
//		break;
//	case DIVISION:
//		if(num2 == 0)
//		{
//			g_math_error = 1;
//			LCD_clearScreen();
//			LCD_displayString("MATH ERROR!");
//		}
//		else
//		{
//			g_math_error = 0;
//			result = num1/num2;
//		}
//		break;
//	default:
//		LCD_displayStringRowColumn(1,0,"          ");
//		LCD_displayStringRowColumn(1,0,"NO OPERATOR!");
//	}
//}

int main(void){

	LCD_init();
	LCD_displayStringRowColumn(0,2,"Calculator ON");
	_delay_ms(1000);
	LCD_clearScreen();

	while(1)
	{
		key = KEYPAD_getPressedKey();
		if((key<=9) && (key>=0))
		{
			LCD_intgerToString(key);
			num2=(num2*10)+key;
		}
		else if ((key == ADDITION) || (key == SUBTRACTION) || (key == MULTIPLICATION) || (key == DIVISION))
		{
			LCD_displayCharacter(key);
			operator = key;
			num1 = num2;
			num2=0;

		}
		else if(key == EQUAL)
		{
			switch(operator)
			{
			case ADDITION:
				g_math_error = 0;
				result = num1 + num2;
				break;
			case SUBTRACTION:
				g_math_error = 0;
				result = num1-num2;
				break;
			case MULTIPLICATION:
				g_math_error = 0;
				result = num1*num2;
				break;
			case DIVISION:
				if(num2 == 0)
				{
					g_math_error = 1;
					LCD_clearScreen();
					LCD_displayString("MATH ERROR!");
				}
				else
				{
					g_math_error = 0;
					result = num1/num2;
				}
				break;
			default:
				LCD_displayStringRowColumn(1,0,"          ");
				LCD_displayStringRowColumn(1,0,"NO OPERATOR!");
			}
//			CALCULATOR_operation(num1,num2,operator);
			if(!g_math_error)
			{
				LCD_displayStringRowColumn(1,0,"Answer = ");
				LCD_floatToString(result);
			}
		}
		else if(key == ALL_CLEAR)
		{
			LCD_clearScreen();
			num1 = num2 = 0;
			operator = 0;
			result = 0;
		}
		_delay_ms(500);
	}


}
