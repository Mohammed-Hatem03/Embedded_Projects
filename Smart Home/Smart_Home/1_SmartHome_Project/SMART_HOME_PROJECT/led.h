#ifndef LED_H_
#define LED_H_

#include "std_types.h"

typedef enum {
	RED_LED = 5, GREEN_LED, BLUE_LED
} LED_ID;


#define LED_PORT_ID PORTB_ID
#define LED_POSITIVE_LOGIC TRUE


void LEDS_init(void);
void LED_on(LED_ID id);
void LED_off(LED_ID id);

#endif /* LED_H_ */
