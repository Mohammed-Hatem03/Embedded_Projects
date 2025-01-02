#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

typedef enum{
	OFF,CW,ACW
}DcMotor_state;

#define DC_MOTOR_IN1_PORT_ID PORTB_ID
#define	DC_MOTOR_IN1_PIN_ID PIN0_ID

#define	DC_MOTOR_IN2_PORT_ID PORTB_ID
#define	DC_MOTOR_IN2_PIN_ID PIN1_ID






void DcMotor_init(void);

void Dc_Motor_Rotate(DcMotor_state state,uint8 speed);

#endif /* DC_MOTOR_H_ */
