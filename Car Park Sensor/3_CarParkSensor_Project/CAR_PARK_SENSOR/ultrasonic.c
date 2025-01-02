#include "ultrasonic.h"
#include "icu.h"
#include "gpio.h"
#include <avr/io.h>
#include <util/delay.h>
#include "math.h"

uint16 g_timeHigh = 0;
uint8 g_edge_counts = 0;
uint8 g_distance = 0;
void Ultrasonic_init(void){
	ICU_ConfigType ICU_Configurations = {F_CPU_8,RAISING};
	ICU_init(&ICU_Configurations);
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(ULTRASONIC_PORT_ID,ULTRASONIC_TRIGGER_PIN_ID,PIN_OUTPUT);

}
void Ultrasonic_trigger(void){
	GPIO_writePin(ULTRASONIC_PORT_ID,ULTRASONIC_TRIGGER_PIN_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_PORT_ID,ULTRASONIC_TRIGGER_PIN_ID,LOGIC_LOW);
}
uint16 Ultrasonic_readDistance(void){
	Ultrasonic_trigger();
	g_distance = (uint16)(round(g_timeHigh*0.0085));
	return g_distance;
}
void Ultrasonic_edgeProcessing(void){
	if(g_edge_counts == 0)
	{
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);
		g_edge_counts++;
	}
	else if(g_edge_counts == 1)
	{
		g_timeHigh = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(RAISING);
		g_edge_counts = 0;

	}
}
