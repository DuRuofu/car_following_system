#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "main.h"
#include "tim.h"


void MOTOR_INIT(void);
void LeftMotor_Go(void);
void LeftMotor_Back(void);
void RightMotor_Go(void) ;
void RightMotor_Back(void);
void RightMotor_Stop(void);
void LeftMotor_Stop(void);
void MotorControl(char motorDirection, int leftMotorPWM, int rightMotorPWM);
void motorset(float leftMotorPWM, float rightMotorPWM);





#endif

