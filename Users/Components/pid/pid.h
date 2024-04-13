#ifndef __PID_H_
#define __PID_H_
#include "main.h"
#include "motor.h"
#include "encoder.h"

int32_t PID_Speed_A(int32_t target_speed, int32_t fact_speed);
int32_t PID_Speed_B(int32_t target_speed, int32_t fact_speed);

void Car_PID_Ctrl(void);
#endif

