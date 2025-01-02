#include "Timer.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static void (*g_callBackPtr0)(void) = NULL_PTR;
static void (*g_callBackPtr1)(void) = NULL_PTR;
static void (*g_callBackPtr2)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		(*g_callBackPtr0)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		(*g_callBackPtr0)();
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{

		(*g_callBackPtr1)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		(*g_callBackPtr1)();
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{

		(*g_callBackPtr2)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		(*g_callBackPtr2)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*  Initializes the timer with the specified settings. */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	uint8 timer_ID = Config_Ptr -> timer_ID ;
	uint8 timer_Mode = Config_Ptr -> timer_mode;


	/* Timer0 configuration */
	if (timer_ID == TIMER_0)
	{
		/* normal mode configuration */
		if(timer_Mode == NORMAL_MODE)
		{
			TCNT0 = Config_Ptr -> timer_InitialValue ; /* Set Timer initial value */

			TIMSK = (1<<TOIE0); /* Enable Timer0 Overflow Interrupt */

			/* configure the timer
			 * 1. Non PWM mode FOC0=1
			 * 2. Normal Mode WGM01=0 & WGM00=0
			 * 3. Normal Mode COM00=0 & COM01=0
			 * 4. clock = F_CPU/prescaler
			 */
			TCCR0 = (1<<FOC0) ;
			TCCR0 = (TCCR0 & 0XF8) | (Config_Ptr->timer_clock);
		}

		/* compare mode configuration */
		else if (timer_Mode == COMPARE_MODE)
		{
			TCNT0 = Config_Ptr -> timer_InitialValue ; /* Set Timer Initial Value 0 */
			OCR0 = Config_Ptr -> timer_compare_MatchValue; /* Set Compare Value */

			TIMSK |= (1<<OCIE0); /* Enable Timer0 Compare Interrupt */

			/* Configure timer control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0 in this example so COM00=0 & COM01=0
			 * 4. clock = F_CPU/prescaler
			 */
			TCCR0 = (1<<FOC0) | (1<<WGM01);
			TCCR0 = (TCCR0 & 0XF8) | (Config_Ptr->timer_clock);
		}
	}

	/* Timer1 configuration */
	else if (timer_ID == TIMER_1)
	{
		/* normal mode configuration */
		if(timer_Mode == NORMAL_MODE)
		{
			TCNT1 = Config_Ptr -> timer_InitialValue ;		/* Set timer1 initial count  */

			TIMSK |= (1<<TOIE1) ; /* Enable Timer1 Overflow Interrupt */

			/* Configure timer control register TCCR1A
			 * 1. Normal Mode COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
			 * 2. Non PWM FOC1A=1 FOC1B=1
			 * 3. Normal Mode WGM10=0 WGM11=0
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);

			/* Configure timer control register TCCR1B
			 * 1. Normal Mode WGM12=0 WGM13=0
			 * 2. Prescaler = F_CPU/Prescaler
			 */
			TCCR1B = (TCCR1B & 0XF8) | (Config_Ptr->timer_clock);
		}

		/* compare mode configuration */
		else if (timer_Mode == COMPARE_MODE)
		{
			TCNT1 = Config_Ptr -> timer_InitialValue ;		/* Set timer1 initial count  */

			OCR1A = Config_Ptr -> timer_compare_MatchValue;    /* Set the Compare value */

			TIMSK |= (1<<OCIE1A) ; /* Enable Timer1 Compare A Interrupt */

			/* Configure timer control register TCCR1A
			 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
			 * 2. FOC1A=1 FOC1B=1
			 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);

			/* Configure timer control register TCCR1B
			 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
			 * 2. Prescaler = F_CPU/Prescaler
			 */
			TCCR1B = (1<<WGM12);
			TCCR1B = (TCCR1B & 0XF8) | (Config_Ptr->timer_clock);
		}
	}

	/* Timer2 configuration */
	else if (timer_ID == TIMER_2)
	{
		/* normal mode configuration */
		if(timer_Mode == NORMAL_MODE)
		{
			TCNT2 = Config_Ptr -> timer_InitialValue ; /* Set Timer initial value */

			TIMSK = (1<<TOIE2); /* Enable Timer2 Overflow Interrupt */

			/* configure the timer
			 * 1. Non PWM mode FOC2=1
			 * 2. Normal Mode WGM21=0 & WGM20=0
			 * 3. Normal Mode COM20=0 & COM21=0
			 * 4. clock = F_CPU/prescaler
			 */
			TCCR2 = (1<<FOC2) ;
			TCCR2 = (TCCR2 & 0XF8) | (Config_Ptr->timer_clock);
		}

		/* compare mode configuration */
		else if (timer_Mode == COMPARE_MODE)
		{
			TCNT2 = Config_Ptr -> timer_InitialValue ; /* Set Timer Initial Value  */
			OCR2 = Config_Ptr -> timer_compare_MatchValue; /* Set Compare Value */

			TIMSK |= (1<<OCIE2); /* Enable Timer2 Compare Interrupt */

			/* Configure timer control register
			 * 1. Non PWM mode FOC2=1
			 * 2. CTC Mode WGM21=1 & WGM20=0
			 * 3. No need for OC2 so COM20=0 & COM21=0
			 * 4. clock = F_CPU/prescaler
			 */
			TCCR2 = (1<<FOC2) | (1<<WGM21);
			TCCR2 = (TCCR2 & 0XF8) | (Config_Ptr->timer_clock);
		}
	}
}

/* Disables the specified timer and resets its settings. */
void Timer_deInit(Timer_ID_Type timer_type)
{
	uint8 timer_ID = timer_type;

	/* Timer0 deinit */
	if (timer_ID == TIMER_0)
	{
		TCNT0 = 0; /* Reset Timer value */
		OCR0  = 0; /* Reset Compare value */
		TIMSK &=~ (1<<TOIE0); /* Disable Timer0 Overflow Interrupt */
		TIMSK &=~ (1<<OCIE0); /* Disable Timer0 Compare Interrupt */
		TCCR0 = 0 ; /* Clear Register */
	}

	/* Timer1 deinit */
	else if (timer_ID == TIMER_1)
	{
		TCNT1 = 0;		 /* Reset Timer value */
		OCR1A = 0;       /* Reset Compare value */
		TIMSK &=~ (1<<TOIE1) ;   /* Disable Timer1 Overflow Interrupt */
		TIMSK &=~ (1<<OCIE1A) ;  /* Disable Timer1 Compare A Interrupt */
		TCCR1A = 0; /* Clear Register */
		TCCR1B = 0; /* Clear Register */
	}

	/* Timer2 deinit */
	else if (timer_ID == TIMER_2)
	{
		TCNT2 = 0; /* Reset Timer value */
		OCR2  = 0; /* Reset Compare value */
		TIMSK &=~ (1<<TOIE2); /* Disable Timer2 Overflow Interrupt */
		TIMSK &=~ (1<<OCIE2); /* Disable Timer2 Compare Interrupt */
		TCCR2 = 0 ; /* Clear Register */
	}
}

/* Sets a callback function for the specified timer's interrupt. */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
{
	uint8 timer_ID = a_timer_ID;

	/* Timer0 callback */
	if (timer_ID == TIMER_0)
	{
		g_callBackPtr0 = a_ptr;
	}

	/* Timer1 callback */
	else if (timer_ID == TIMER_1)
	{
		g_callBackPtr1 = a_ptr;

	}

	/* Timer2 callback */
	else if (timer_ID == TIMER_2)
	{
		g_callBackPtr2 = a_ptr;

	}
}
