#ifndef BUZZERH
#define BUZZERH

#include "gpio.h"
#include "std_types.h"
#define BUZZER_PORT_ID PORTD_ID
#define BUZZER_PIN_ID PIN3_ID


void BUZZER_init(void);
void BUZZER_on(void);
void BUZZER_off(void);


#endif /* BUZZER_H */
