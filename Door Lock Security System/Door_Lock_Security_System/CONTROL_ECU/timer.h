#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
typedef enum
{
	TIMER_0 , TIMER_1 , TIMER_2

}Timer_ID_Type;

typedef enum
{
	 NO_CLOCK,
	 F_CPU_1,
	 F_CPU_8,
	 F_CPU_64,
	 F_CPU_256,
	 F_CPU_1024,
	 EXTERNAL_CLOCK_FALLING_EDGE,
	 EXTERNAL_CLOCK_RISING_EDGE

}Timer_ClockType;

typedef enum
{
	NORMAL_MODE,
	COMPARE_MODE

}Timer_ModeType;

typedef struct
{

uint16 timer_InitialValue;
uint16 timer_compare_MatchValue;     /*it will be used in compare mode only*/
Timer_ID_Type  timer_ID;
Timer_ClockType timer_clock;
Timer_ModeType  timer_mode;

}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*  Initializes the timer with the specified settings. */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/* Disables the specified timer and resets its settings. */
void Timer_deInit(Timer_ID_Type timer_type);

/* Sets a callback function for the specified timer's interrupt. */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );




#endif /* TIMER_H_ */
