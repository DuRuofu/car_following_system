#ifndef __PID_H_
#define __PID_H_
#include "main.h"
#include "motor.h"
#include "encoder.h"
typedef struct{
	float target_val;
	float actual_val;
	float err;
	float err_last;
	float err_sum;
	float kp,ki,kd;
	
}Pid;

void PID_INIT(void);
float P_realize(Pid *pid,float actual_val,float target_val);
float PI_realize(float actual_val,float target_val);
float PD_realize(Pid *pid,float actual_val);
float PID_realize(Pid *pid,float actual_val);

#endif

