#include "PIR.h"
#include "gpio.h"

void PIR_init(void){
	GPIO_setupPinDirection(PIR_SENSOR_PORT_ID,PIR_SENSOR_PIN_ID,PIN_INPUT);
}

uint8 PIR_getState(void)
{
	return GPIO_readPin(PIR_SENSOR_PORT_ID,PIR_SENSOR_PIN_ID);
}
