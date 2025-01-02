#include "led.h"
#include "gpio.h"
void LEDS_init(void) {
	GPIO_setupPinDirection(LED_PORT_ID, RED_LED, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_PORT_ID, BLUE_LED, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_PORT_ID, GREEN_LED, PIN_OUTPUT);

	GPIO_writePin(LED_PORT_ID, RED_LED, LOGIC_LOW);
	GPIO_writePin(LED_PORT_ID, BLUE_LED, LOGIC_LOW);
	GPIO_writePin(LED_PORT_ID, GREEN_LED, LOGIC_LOW);


}
void LED_on(LED_ID id) {
#if LED_POSITIVE_LOGIC
	GPIO_writePin(LED_PORT_ID, id, LOGIC_HIGH);
#else
	GPIO_writePin(LED_PORT_ID, RED_LED, LOGIC_LOW);
#endif
}
void LED_off(LED_ID id) {
#if LED_POSITIVE_LOGIC
	GPIO_writePin(LED_PORT_ID, id, LOGIC_LOW);
#else
	GPIO_writePin(LED_PORT_ID, RED_LED, LOGIC_HIGH);
#endif
}
