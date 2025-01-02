#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"
#include "common_macros.h"

#define PIR_SENSOR_PORT_ID PORTC_ID
#define PIR_SENSOR_PIN_ID PIN2_ID

void PIR_init(void);

uint8 PIR_getState(void);

#endif /* PIR_H_ */
