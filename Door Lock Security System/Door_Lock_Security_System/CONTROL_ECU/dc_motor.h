#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

typedef enum{
	OFF,CW,ACW
}DcMotor_state;

#define FULL_SPEED 100
#define STOP 0

#define DC_MOTOR_IN1_PORT_ID PORTD_ID
#define	DC_MOTOR_IN1_PIN_ID PIN6_ID

#define	DC_MOTOR_IN2_PORT_ID PORTD_ID
#define	DC_MOTOR_IN2_PIN_ID PIN7_ID






void DcMotor_init(void);

void Dc_Motor_Rotate(DcMotor_state state,uint8 speed);

#endif /* DC_MOTOR_H_ */
