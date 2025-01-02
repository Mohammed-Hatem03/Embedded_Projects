#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>

#define FAILED 0
#define SUCCESS 1
#define PIR_DONE 2

#define PASSWORD_CHARACTERS 5
#define KEYPAD_ENTER '='
#define OPEN_DOOR '+'
#define CHANGE_PASSWORD '-'


uint8 created_password[PASSWORD_CHARACTERS];
uint8 re_password[PASSWORD_CHARACTERS];

uint8 password_entered_by_user[PASSWORD_CHARACTERS];
uint8 new_password[PASSWORD_CHARACTERS];

uint8 g_key;
uint8 g_count = 0;
uint8 g_minutes = 60;
uint8 g_timer_flag = 0;
uint8 g_attempts = 3;

void HMI_createPassword();
void HMI_sendPassword(uint8*);
void HMI_receiveConfirmation(void);
void HMI_enterPassword(void);
void HMI_openDoor(void);
void HMI_changePassword(void);
void HMI_lockScreen(void);
void HMI_doorUnlockingScreen(void);
void HMI_doorLockingScreen(void);


int main(void){

	/*ENABLE I-BIT FOR INTERRUPTS*/
	SREG |= (1<<7);

	/*UART CONFIGURATIONS*/
	UART_ConfigType UART_Configurations = {ASYNC,_8_BIT_MODE,EVEN_PARITY,_1_BIT,BAUD_RATE_9600};

	/*UART INITIALIZATION*/
	UART_init(&UART_Configurations);

	/*LCD INITIALIZATION*/
	LCD_init();

	/*DISPLAY WELCOME MESSAGE */
	LCD_displayStringRowColumn(0,0,"DOOR LOCK");
	LCD_displayStringRowColumn(1,0,"SECURITY SYSTEM");

	_delay_ms(1000);

	/*CLEAR THE LCD SCREEN*/
	LCD_clearScreen();

	/*CREATE & SUBMIT PASSWORDS*/
	HMI_createPassword();

	/*SEND CREATED PASSWORD*/
	HMI_sendPassword(created_password);

	/*SEND SUBMITED PASSWORD*/
	HMI_sendPassword(re_password);

	/*KNOWING THAT THE PASSWORDS ARE MATCHED OR NOT*/
	HMI_receiveConfirmation();


	while(1)
	{
		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*DISPLAY MENU SCREEN*/
		LCD_displayStringRowColumn(0,0,"+ : OPEN DOOR");
		LCD_displayStringRowColumn(1,0,"- : CHANGE PASS");

		/*STORING THE PRESSED KEY IN g_key*/
		g_key = KEYPAD_getPressedKey();

		/*OPEM DOOR COMMAND*/
		if(g_key == '+')
		{
			UART_sendByte(OPEN_DOOR);

			/*OPEN DOOR REQUEST*/
			HMI_openDoor();
		}

		/*CHANGE PASSWORD COMMAND*/
		else if (g_key == '-')
		{
			UART_sendByte(CHANGE_PASSWORD);

			/*CHANGE PASSWORD REQUEST*/
			HMI_changePassword();
		}

	}
}

void HMI_createPassword(void)
{
	uint8 i;

	/*CLEAR THE LCD SCREEN*/
	LCD_clearScreen();

	/*DISPLAY ENTERING PASSWORD SCREEN*/
	LCD_displayString("PLZ ENTER PASS:");
	LCD_moveCursor(1,0);

	for(i = 0; i < PASSWORD_CHARACTERS;++i)
	{
		_delay_ms(275);
		g_key = KEYPAD_getPressedKey();
		if((g_key>=0) && (g_key<=9))
		{
			*(created_password+i) = g_key;
			LCD_displayCharacter('*');
		}
	}
	while(!(g_key == KEYPAD_ENTER))
	{
		g_key = KEYPAD_getPressedKey();
	}

	/*CLEAR THE LCD SCREEN*/
	LCD_clearScreen();

	/*DISPLAY SUBMITING PASSWORD SCREEN*/
	LCD_displayString("PLZ RE-ENTER THE:");
	LCD_moveCursor(1, 0);
	LCD_displayString("SAME PASS:");

	for(i = 0; i<PASSWORD_CHARACTERS;++i)
	{
		_delay_ms(275);
		g_key = KEYPAD_getPressedKey();
		if ((g_key >= 0) && (g_key <= 9))
		{
			*(re_password+i) = g_key;
			LCD_displayCharacter('*');
		}
	}
	while(!(g_key == KEYPAD_ENTER))
	{
		g_key = KEYPAD_getPressedKey();
	}
}

void HMI_sendPassword(uint8 *password)
{
	uint8 i;
	for(i = 0; i < PASSWORD_CHARACTERS; ++i)
	{
		UART_sendByte(*(password+i));
	}
}

void HMI_receiveConfirmation(void){
	uint8 recieve;

	recieve = UART_recieveByte();

	if(recieve == FAILED)
	{
		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*DISPLAY THAT PASSWORDS AREN'T MATCHED*/
		LCD_displayStringRowColumn(0,0,"NOT MATCHED"); /* Display failure message */
		_delay_ms(500);  /* Wait for a moment */

		/*MAKE THE USER RE-ENTER PASSWORDS BECAUSE THET AREN'T MATCHED*/
		HMI_createPassword();

		/*SEND CREATED PASSWORD*/
		HMI_sendPassword(created_password);

		/*SEND SUBMITED PASSWORD*/
		HMI_sendPassword(re_password);

		/*KNOWING THAT THE PASSWORDS ARE MATCHED OR NOT*/
		HMI_receiveConfirmation();
	}
	else if(recieve == SUCCESS)
	{
		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*DISPLAY THAT PASSWORDS ARE MATCHED*/
		LCD_displayStringRowColumn(0,0,"MATCHED PASSWORD");
		_delay_ms(500);
	}
}

void HMI_openDoor(void)
{
	uint8 receive;
	uint8 PIR_sensor;

	/*MAKE THE USER ENTERS HIS PASSWORD*/
	HMI_enterPassword();

	/*SEND THE PASSWORD ENTERED BY USER*/
	HMI_sendPassword(password_entered_by_user);

	receive = UART_recieveByte();

	if(receive == SUCCESS)
	{
		/*WHEN PASSWORD ENTERED BY USER IS CORRECT*/

		/*TIMER CONFIGURATIONS*/
		Timer_ConfigType Timer1_Configurations_1 = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

		/*TIMER INITIALIZATION*/
		Timer_init(&Timer1_Configurations_1);

		/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT OPENS DOOR*/
		Timer_setCallBack(HMI_doorUnlockingScreen,TIMER_1);

		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*DISPLAY THAT THE DOOR IS OPENING*/
		LCD_displayStringRowColumn(0,0,"OPENING DOOR");

		/*ATTEMPTS THAT THE USER HAVE TO ENTER THE PASSWORD ARE 3 ATTEMPTS*/
		g_attempts = 3;

		/*WAIT FOR TIMER TO FINISH COUNTING*/
		while(!(g_timer_flag));

		/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
		g_timer_flag = 0;

		/*PIR SENSOR READING IF PEOPLE ARE ENTERING OR NO*/
		receive = UART_recieveByte();
		if(receive == SUCCESS)
		{
			/*CLEAR THE LCD SCREEN*/
			LCD_clearScreen();

			/*DISPLAY THAT PEOPLE ARE ENTERING AND MAKE THE DOOR OPENED UNTIL PEOPLE ENTER THE HOME*/
			LCD_displayStringRowColumn(0,0,"WAIT FOR PEOPLE");
			LCD_displayStringRowColumn(1,3,"TO ENTER");

			/*READING PIR SENSOR VALUE*/
			PIR_sensor = UART_recieveByte();

			while (PIR_sensor != PIR_DONE)
			{
				/*CHECKING FOR PIR SENSOR VALUE*/
				PIR_sensor = UART_recieveByte();
			}
		}

		/*TIMER CONFIGURATIONS*/
		Timer_ConfigType Timer1_Configurations_2 = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

		/*TIMER INITIALIZATION*/
		Timer_init(&Timer1_Configurations_2);

		/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT CLOSES THE DOOR*/
		Timer_setCallBack( HMI_doorLockingScreen , TIMER_1);

		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*DISPLAY THAT THE DOOR IS CLOSING*/
		LCD_displayStringRowColumn(0,0,"CLOSING DOOR");

		/*WAIT FOR TIMER TO FINISH COUNTING*/
		while(!(g_timer_flag));

		/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
		g_timer_flag = 0;
	}
	else if(receive == FAILED)
	{
		/*WHEN PASSWORD ENTERED BY USER IS WRONG*/

		/*ONE ATTEMPT IS USED*/
		g_attempts--;

		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*DISPLAY THAT THE PASSWORD ENTERED BY THE USER IS WRONG*/
		LCD_displayStringRowColumn(0,0,"WRONG PASSWORD");

		/*SHOWS THE REMAINIG ATTEMPTS FOR THE USER*/
		LCD_displayStringRowColumn(1,0,"ATTEMPTS LEFT ");
		LCD_intgerToString(g_attempts);

		_delay_ms(500);

		/*LOCK THE SYSTEM AFTER 3 WRONG ATTEMPTS*/
		if(g_attempts == 0)
		{
			/*TIMER CONFIGURATIONS*/
			Timer_ConfigType Timer1_Configurations_3 = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

			/*TIMER INITIALIZATION*/
			Timer_init(&Timer1_Configurations_3);

			/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT LOCKS THE SYSTEM*/
			Timer_setCallBack( HMI_lockScreen , TIMER_1);

			/*CLEAR THE LCD SCREEN*/
			LCD_clearScreen();

			/*DISPLAY THAT THE SYSTEM IS LOCKED FOR ONE MINUTE*/
			LCD_displayStringRowColumn(0,0,"SYSTEM LOCKED");

			/*WAIT FOR TIMER TO FINISH COUNTING*/
			while(!g_timer_flag);

			/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
			g_timer_flag=0;

			/*RETURN USER'S ATTEMPTS FOR 3 ATTEMPTS AGAIN*/
			g_attempts=3;
		}
	}
}

void HMI_changePassword(void)
{
	uint8 receive;

	/*MAKE THE USER ENTERS HIS PASSWORD*/
	HMI_enterPassword();

	/*SEND THE PASSWORD ENTERED BY USER*/
	HMI_sendPassword(password_entered_by_user);

	receive = UART_recieveByte();

	if ( receive == SUCCESS )
	{
		/*WHEN PASSWORD ENTERED BY USER IS CORRECT*/

		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*DISPLAYS THAT THE USER IS GOING TO CHANGE HIS/HER PASSWORD*/
		LCD_displayStringRowColumn(0, 0, "CHANGING PASS");

		_delay_ms(500);

		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*MAKE THE USER CREATES NEW PASSWORD*/
		HMI_createPassword();

		/*SEND CREATED PASSWORD*/
		HMI_sendPassword(created_password);

		/*SEND SUBMITED PASSWORD*/
		HMI_sendPassword(re_password);

		/*KNOWING THAT THE PASSWORDS ARE MATCHED OR NOT*/
		HMI_receiveConfirmation();

		/*ATTEMPTS THAT THE USER HAVE TO ENTER THE PASSWORD ARE 3 ATTEMPTS*/
		g_attempts = 3;
	}

	else if(receive == FAILED)
	{
		/*WHEN PASSWORD ENTERED BY USER IS WRONG*/

		/*ONE ATTEMPT IS USED*/
		g_attempts--;


		LCD_clearScreen();

		/*DISPLAY THAT THE PASSWORD ENTERED BY THE USER IS WRONG*/
		LCD_displayStringRowColumn(0,0,"WRONG PASSWORD");

		/*SHOWS THE REMAINIG ATTEMPTS FOR THE USER*/
		LCD_displayStringRowColumn(1,0,"ATTEMPTS LEFT ");

		/*SHOWS THE REMAINIG ATTEMPTS FOR THE USER*/
		LCD_intgerToString(g_attempts);

		_delay_ms(500);

		/*LOCK THE SYSTEM AFTER 3 WRONG ATTEMPTS*/
		if (g_attempts == 0)
		{
			/*TIMER CONFIGURATIONS*/
			Timer_ConfigType Timer1_Configurations = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

			/*TIMER INITIALIZATION*/
			Timer_init(&Timer1_Configurations);

			/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT LOCKS THE SYSTEM*/
			Timer_setCallBack(HMI_lockScreen , TIMER_1);

			/*CLEAR THE LCD SCREEN*/
			LCD_clearScreen();

			/*DISPLAY THAT THE SYSTEM IS LOCKED FOR ONE MINUTE*/
			LCD_displayStringRowColumn(0,0,"SYSTEM LOCKED");

			/*WAIT FOR TIMER TO FINISH COUNTING*/
			while(!g_timer_flag);

			/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
			g_timer_flag=0;

			/*RETURN USER'S ATTEMPTS FOR 3 ATTEMPTS AGAIN*/
			g_attempts=3;
		}
	}
}
void HMI_lockScreen(void)
{
	/*INCREMENT COUNTS FOR EACH TIMER TICK*/
	g_count++;

	/*DECREMENT MINUTES*/
	g_minutes--;

	/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
	g_timer_flag = 0;

	/*DISPLAY THAT THE SYSTEM IS LOCKED FOR ONE MINUTE*/
	LCD_displayStringRowColumn(0,0,"SYSTEM LOCKED");

	/*DISPLAY THE REMAINIG TIME FOR SYSTEM TO BE UNLOCKED*/
	LCD_displayStringRowColumn(1,0,"COUNTING ");

	/*DISPLAY THE REMAINING TIME FOR SYSTEM TO BE UNLOCKED*/
	if(g_minutes >= 100)
	{
		LCD_intgerToString(g_minutes);
	}
	else
	{
		LCD_intgerToString(g_minutes);

		/*TO SOLVE THE OVERWRITE ISSUE*/
		LCD_displayCharacter(' ');
	}

	/*CHECKS IF ONE MINUTE HAS BEEN FINISHED OR NO*/
	if (g_count == 60)
	{
		/*STOP THE TIMER*/
		Timer_deInit(TIMER_1);

		/*RESET COUNTER*/
		g_count = 0;

		/*RESET MINUTES FOR THE NEXT EXECUTION*/
		g_minutes = 60;

		/*SET TIMER FLAG TO 1 TO MAKE THE CONTROL KNOW THAT TIMER FINISHED HIS MISSIOM*/
		g_timer_flag = 1;


		/*CLEAR THE LCD SCREEN*/
		LCD_clearScreen();

		/*TELL THE USER THAT THE SYSTEM HAS BEEN UNLOCKED AND HE CAN TRY AGAIN ENTERING PASSWORD OR CHANGE IT*/
		LCD_displayStringRowColumn(0, 0, "TRY AGAIN NOW");

		_delay_ms(500);
	}

}
void HMI_doorUnlockingScreen(void)
{
	/*INCREMENT TIMER COUNTS*/
	g_count++;

	/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
	g_timer_flag=0;

	/*DISPLAY THAT THE DOOR IS OPENING*/
	LCD_displayStringRowColumn(0,0,"OPENING DOOR");

	/*DISPLAY TIME FOR OPENING DOOR*/
	LCD_displayStringRowColumn(1,0,"COUNTING ");
	LCD_intgerToString(g_count);

	/*CHECKS IF 15 SECONDS HAVE BEEN FINISHED OR NO*/
	if (g_count == 16)
	{
		/*STOP TIMER*/
		Timer_deInit(TIMER_1);

		/*RESET COUNTER*/
		g_count = 0;

		/*SET TIMER FLAG TO 1 TO MAKE THE CONTROL KNOW THAT TIMER FINISHED HIS MISSIOM*/
		g_timer_flag = 1;
	}
}

void HMI_doorLockingScreen(void)
{
	/*INCREMENT TIMER COUNTS*/
	g_count++;

	/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
	g_timer_flag=0;

	/*DISPLAY THAT THE DOOR IS CLOSING*/
	LCD_displayStringRowColumn(0,0,"CLOSING DOOR");

	/*DISPLAY TIME FOR CLOSING DOOR*/
	LCD_displayStringRowColumn(1,0,"COUNTING ");
	LCD_intgerToString(g_count);

	/*CHECKS IF 15 SECONDS HAVE BEEN FINISHED OR NO*/
	if (g_count == 16)
	{
		/*STOP TIMER*/
		Timer_deInit(TIMER_1);

		/*RESET COUNTER*/
		g_count = 0;

		/*SET TIMER FLAG TO 1 TO MAKE THE CONTROL KNOW THAT TIMER FINISHED HIS MISSIOM*/
		g_timer_flag = 1;
	}
}


void HMI_enterPassword(){
	uint8 i;

	LCD_clearScreen();
	LCD_displayString("PLEASE ENTER");
	LCD_moveCursor(1,0);
	LCD_displayString("YOUR PASS:");

	for(i = 0; i < PASSWORD_CHARACTERS;++i)
	{
		_delay_ms(275);

		g_key = KEYPAD_getPressedKey();
		if((g_key>=0) && (g_key<=9))
		{
			*(password_entered_by_user+i) = g_key;
			LCD_displayCharacter('*');

		}
	}
	while(!(g_key == KEYPAD_ENTER))
	{
		g_key = KEYPAD_getPressedKey();
	}
}
