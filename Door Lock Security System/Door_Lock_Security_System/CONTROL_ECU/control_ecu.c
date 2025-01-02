#include "uart.h"
#include "twi.h"
#include "timer.h"
#include "PIR.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include <util/delay.h>
#include <avr/io.h>

#define FAILED 0
#define SUCCESS 1
#define PIR_DONE 2

#define EEPROM_ADDRESS 0x0311

#define PASSWORD_CHARACTERS 5
#define KEYPAD_ENTER '='
#define OPEN_DOOR '+'
#define CHANGE_PASSWORD '-'

uint8 created_password[PASSWORD_CHARACTERS];
uint8 re_password[PASSWORD_CHARACTERS];

uint8 password_entered_by_user[PASSWORD_CHARACTERS];
uint8 saved_password[PASSWORD_CHARACTERS];

uint8 g_mode = 0;
uint8 g_check = 0;
uint8 g_key;
uint8 g_count = 0;
uint8 g_minutes = 60;
uint8 g_timer_flag = 0;
uint8 g_attempts = 3;

void CONTROL_ECU_receivePassword(uint8*);
void CONTROL_ECU_checkReceivedPasswords(void);
void CONTROL_ECU_storingAtEEPROM(void);
void CONTROL_ECU_receiveOpenDoor(void);
void CONTROL_ECU_rotateMotor_CW(void);
void CONTROL_ECU_rotateMotor_ACW(void);
void CONTROL_ECU_TURN_BUZZER(void);
void CONTROL_ECU_receiveChangedPassword(void);

int main(void){

	/*ENABLE I-BIT FOR INTERRUPTS*/
	SREG = (1<<7);

	/*PIR SENSOR INITIALIZATION*/
	PIR_init();

	/*DC MOTOR INITIALIZATION*/
	DcMotor_init();

	/*BUZZER INITIALIZATION*/
	BUZZER_init();

	/*I2C CONFIGURATIONS*/
	TWI_ConfigType TWI_Configurations = {0b00000110,400000};

	/*I2C INITIALIZATION*/
	TWI_init(&TWI_Configurations);

	/*UART CONFIGURATIONS*/
	UART_ConfigType UART_Configurations = {ASYNC,_8_BIT_MODE,EVEN_PARITY,_1_BIT,BAUD_RATE_9600};

	/*UART Initialization*/
	UART_init(&UART_Configurations);

	/*RECEIVE CREATED PASSWORD*/
	CONTROL_ECU_receivePassword(created_password);

	/*RECEIVE SUBMITED PASSWORD*/
	CONTROL_ECU_receivePassword(re_password);

	/*CHECK IF THE PASSWORD ARE MATCHED OR NO*/
	CONTROL_ECU_checkReceivedPasswords();

	/*STORE THE CREATED PASSWORD AFTER KNOWING THEY ARE MATCHED IN EEPROM*/
	CONTROL_ECU_storingAtEEPROM();

	while(1)
	{
		g_mode = UART_recieveByte();

		if(g_mode == OPEN_DOOR)
		{
			CONTROL_ECU_receiveOpenDoor();
		}
		else if (g_mode == CHANGE_PASSWORD)
		{
			CONTROL_ECU_receiveChangedPassword();
		}
	}
}
void CONTROL_ECU_receivePassword(uint8* received_password)
{
	uint8 i;
	for(i = 0; i < PASSWORD_CHARACTERS; ++i)
	{
		*(received_password+i) = UART_recieveByte();
	}
}
void CONTROL_ECU_checkReceivedPasswords(void)
{
	uint8 i ;
	do
	{
		g_check = 1;


		for (i = 0; i < PASSWORD_CHARACTERS; i++)
		{
			if (created_password[i] != re_password[i])
			{
				g_check = 0;
				break;
			}
		}


		if (g_check)
		{
			UART_sendByte(SUCCESS);
		}
		else
		{
			UART_sendByte(FAILED);
			CONTROL_ECU_receivePassword(created_password);
			CONTROL_ECU_receivePassword(re_password);

		}

	} while (!g_check);
}
void CONTROL_ECU_storingAtEEPROM(void)
{
	uint8 i;
	if(g_check)
	{
		for(i =0; i<PASSWORD_CHARACTERS; ++i)
		{
			EEPROM_writeByte(EEPROM_ADDRESS+i,*(created_password+i));
			_delay_ms(10);
		}
		_delay_ms(10);

		for(i = 0; i<PASSWORD_CHARACTERS; ++i)
		{
			EEPROM_readByte(EEPROM_ADDRESS+i,&saved_password[i]);
		}
	}
}
void CONTROL_ECU_receiveOpenDoor(void)
{
	/*RECEIVE THE PASSWORD ENTERED BY USER*/
	CONTROL_ECU_receivePassword(password_entered_by_user);

	uint8 i;

	/*ASSUME THAT THE PASSWORD IS MATCHED WITH THE SAVED PASSWORD IN THE EEPROM*/
	g_check = 1;
	for(i = 0; i < PASSWORD_CHARACTERS; ++i)
	{
		if(*(saved_password+i) != *(password_entered_by_user+i))
		{
			g_check = 0;
			break;
		}
	}
	if(g_check)
	{
		/*IF PASSWORD ENTERED BY USER MATCHES THE ONE SAVED IN THE EEPROM*/

		UART_sendByte(SUCCESS);

		/*TIMER CONFIGURATIONS*/
		Timer_ConfigType Timer1_Configurations_1 = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

		/*TIMER INITIALIZATION*/
		Timer_init(&Timer1_Configurations_1);

		/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT OPENS DOOR*/
		Timer_setCallBack(CONTROL_ECU_rotateMotor_CW,TIMER_1);

		/*WAIT FOR TIMER TO FINISH COUNTING*/
		while(!g_timer_flag);

		/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
		g_timer_flag=0;

		/*ATTEMPTS THAT THE USER HAVE TO ENTER THE PASSWORD ARE 3 ATTEMPTS*/
		g_attempts=3;

		/*CHECK IF THERE ARE PEOPLE ENTERING OR NO*/
		if (PIR_getState())
		{
			UART_sendByte(SUCCESS);

			/*WAIT UNTIL PEOPLE ENTER HOME*/
			while(PIR_getState());

			/*PEOPLE ENTERED HOME*/
			UART_sendByte(PIR_DONE);
		}
		else
		{
			/*THERE IS NO PEOPLE ENTER*/
			UART_sendByte(FAILED);
		}

		/*TIMER CONFIGURATIONS*/
		Timer_ConfigType Timer1_Configurations_2 = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

		/*TIMER INITIALIZATION*/
		Timer_init(&Timer1_Configurations_2);

		/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT CLOSES DOOR*/
		Timer_setCallBack( CONTROL_ECU_rotateMotor_ACW,TIMER_1);

		/*WAIT FOR TIMER TO FINISH COUNTING*/
		while(!g_timer_flag);

		/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
		g_timer_flag=0;
	}
	else
	{
		/*IF PASSWORD ENTERED BY USER DOESN'T MATCH THE ONE SAVED IN THE EEPROM*/

		UART_sendByte(FAILED);

		/*ONE ATTEMPT IS USED*/
		g_attempts--;

		/*LOCK THE SYSTEM AFTER 3 WRONG ATTEMPTS*/
		if (g_attempts == 0)
		{
			/*TIMER CONFIGURATIONS*/
			Timer_ConfigType Timer1_Configurations_3 = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

			/*TIMER INITIALIZATION*/
			Timer_init(&Timer1_Configurations_3);

			/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT ALERTS AND TURN ON THE BUZZER*/
			Timer_setCallBack(CONTROL_ECU_TURN_BUZZER, TIMER_1);

			/*WAIT FOR TIMER TO FINISH COUNTING*/
			while(!g_timer_flag);

			/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
			g_timer_flag=0;

			/*RETURN USER'S ATTEMPTS FOR 3 ATTEMPTS AGAIN*/
			g_attempts=3;
		}
	}
}


void CONTROL_ECU_rotateMotor_CW(void)
{
	/*INCREMENT COUNTS FOR EACH TIMER TICK*/
	g_count++;

	/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
	g_timer_flag=0;

	/*ROTATE THE DC MOTOR CW WITH FULL SPEED TO OPEN THE DOOR*/
	Dc_Motor_Rotate(CW , FULL_SPEED);

	/*CHECKS IF 15 SECONDS HAVE BEEN FINISHED OR NO*/
	if (g_count == 16)
	{
		/*STOP THE MOTOR*/
		Dc_Motor_Rotate(OFF , STOP);

		/*STOP THE TIMER*/
		Timer_deInit(TIMER_1);

		/*RESET COUNTER*/
		g_count = 0;

		/*SET TIMER FLAG TO 1 TO MAKE THE CONTROL KNOW THAT TIMER FINISHED HIS MISSIOM*/
		g_timer_flag=1;
	}
}
void CONTROL_ECU_rotateMotor_ACW(void)
{
	/*INCREMENT COUNTS FOR EACH TIMER TICK*/
	g_count++;

	/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
	g_timer_flag=0;

	/*ROTATE THE DC MOTOR A-CW WITH FULL SPEED TO CLOSE THE DOOR*/
	Dc_Motor_Rotate(ACW , FULL_SPEED);

	/*CHECKS IF 15 SECONDS HAVE BEEN FINISHED OR NO*/
	if (g_count == 16)
	{
		/*STOP THE MOTOR*/
		Dc_Motor_Rotate(OFF , STOP);

		/*STOP THE TIMER*/
		Timer_deInit(TIMER_1);

		/*RESET COUNTER*/
		g_count = 0;

		/*SET TIMER FLAG TO 1 TO MAKE THE CONTROL KNOW THAT TIMER FINISHED HIS MISSIOM*/
		g_timer_flag=1;
	}
}
void CONTROL_ECU_TURN_BUZZER(void)
{
	/*INCREMENT COUNTS FOR EACH TIMER TICK*/
	g_count++;

	/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
	g_timer_flag=0;

	/*TURN ON THE BUZZER*/
	BUZZER_on();

	/*CHECKS IF ONE MINUTE HAS BEEN FINISHED OR NO*/
	if (g_count == 62)
	{
		/*TURN OFF THE BUZZER*/
		BUZZER_off();

		/*STOP THE TIMER*/
		Timer_deInit(TIMER_1);

		/*RESET COUNTER*/
		g_count = 0;

		/*SET TIMER FLAG TO 1 TO MAKE THE CONTROL KNOW THAT TIMER FINISHED HIS MISSIOM*/
		g_timer_flag=1;
	}
}
void CONTROL_ECU_receiveChangedPassword(void)
{
	/*RECEIVE THE PASSWORD ENTERED BY USER*/
	CONTROL_ECU_receivePassword(password_entered_by_user);

	uint8 i;

	/*ASSUME THAT THE PASSWORD IS MATCHED WITH THE SAVED PASSWORD IN THE EEPROM*/
	g_check = 1;


	for (i = 0; i < PASSWORD_CHARACTERS; i++)
	{
		if (saved_password[i] != password_entered_by_user[i])
		{
			g_check = 0;
			break;
		}
	}


	if (g_check)
	{
		/*IF USER ENTERES HIS/HER PASSWORD CORRECTLY*/
		UART_sendByte(SUCCESS);

		/*RECEIVE THE NEW PASSWORD AND ITS SUBMISSION*/
		CONTROL_ECU_receivePassword(created_password);
		CONTROL_ECU_receivePassword(re_password);

		/*CHECK IF THE NEW PASSWORD AND ITS SUBMISSION ARE MATCHED OR NO*/
		CONTROL_ECU_checkReceivedPasswords();

		/*STORE THE NEW PASSWORD IN EEPROM IF THE NEW PASSWORD AND ITS SUBMISSION ARE MATCHED*/
		CONTROL_ECU_storingAtEEPROM();

		/*RETURN USER'S ATTEMPTS FOR 3 ATTEMPTS AGAIN*/
		g_attempts=3;
	}
	else
	{
		/*IF USER ENTERES HIS/HER PASSWORD WRONGLY*/

		UART_sendByte(FAILED);

		/*ONE ATTEMPT IS USED*/
		g_attempts--;

		/*LOCK THE SYSTEM AFTER 3 WRONG ATTEMPTS*/
		if (g_attempts == 0)
		{
			/*TIMER CONFIGURATIONS*/
			Timer_ConfigType Timer1_Configurations_1 = {0,3910,TIMER_1,F_CPU_1024,COMPARE_MODE};

			/*TIMER INITIALIZATION*/
			Timer_init(&Timer1_Configurations_1);

			/*CALL BACK FUNCTION THAT EXECUTED IN THE ISR THAT ALERTS AND TURN ON THE BUZZER*/
			Timer_setCallBack( CONTROL_ECU_TURN_BUZZER , TIMER_1);

			/*WAIT FOR TIMER TO FINISH COUNTING*/
			while(!g_timer_flag);

			/*CLEAR THE TIMER FLAG TO USE IT ANOTHER TIME IF WE NEED THAT*/
			g_timer_flag=0;

			/*RETURN USER'S ATTEMPTS FOR 3 ATTEMPTS AGAIN*/
			g_attempts=3;
		}
	}
}
