#ifndef PWM_TIMER0_H_
#define PWM_TIMER0_H_

#include "std_types.h"

#define _0_SPEED 0
#define _25_SPEED 25
#define _50_SPEED 50
#define _75_SPEED 75
#define FULL_SPEED 100

void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_TIMER0_H_ */
