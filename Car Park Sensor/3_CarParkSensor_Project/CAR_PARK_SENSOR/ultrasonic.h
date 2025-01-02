#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "std_types.h"

#define ULTRASONIC_PORT_ID PORTD_ID

#define ULTRASONIC_ECHO_PIN_ID PIN6_ID
#define ULTRASONIC_TRIGGER_PIN_ID PIN7_ID


void Ultrasonic_init(void);
void Ultrasonic_trigger(void);
uint16 Ultrasonic_readDistance(void);
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */
